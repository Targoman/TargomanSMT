/******************************************************************************
 * Targoman: A robust Statistical Machine Translation framework               *
 *                                                                            *
 * Copyright 2014-2015 by ITRC <http://itrc.ac.ir>                            *
 *                                                                            *
 * This file is part of Targoman.                                             *
 *                                                                            *
 * Targoman is free software: you can redistribute it and/or modify           *
 * it under the terms of the GNU Lesser General Public License as published   *
 * by the Free Software Foundation, either version 3 of the License, or       *
 * (at your option) any later version.                                        *
 *                                                                            *
 * Targoman is distributed in the hope that it will be useful,                *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of             *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              *
 * GNU Lesser General Public License for more details.                        *
 * You should have received a copy of the GNU Lesser General Public License   *
 * along with Targoman. If not, see <http://www.gnu.org/licenses/>.           *
 *                                                                            *
 ******************************************************************************/
/**
 * @author S. Mohammad M. Ziabary <ziabary@targoman.com>
 */

#include <unistd.h>
#include "TSMonitor.h"
#include "libTargomanCommon/JSONConversationProtocol.h"
#include "libTargomanCommon/SimpleAuthentication.h"

namespace Targoman {
namespace Apps{
namespace Modules {

using namespace Common;
using namespace Common::Configuration;

tmplRangedConfigurable<quint16> TSMonitor::UpdateInterval(
        MAKE_CONFIG_PATH("UpdateInterval"),
        "Interval to collect information from servers in seconds must be less than 100",
        1,100,
        1,
        ReturnTrueCrossValidator,
        "","","",
        Common::Configuration::enuConfigSource::File
        );

tmplRangedConfigurable<quint16> TSMonitor::WaitOnUpdtae(
        MAKE_CONFIG_PATH("WaitOnUpdtae"),
        "miliseconds to wait before new update request this must be less than half of UpdateInterval",
        0,50000,
        300,
        [] (const Common::Configuration::intfConfigurable& _item, QString& _errorMessage) {
            if(_item.toVariant().toUInt() < TSMonitor::UpdateInterval.value() * 500)
                return true;
            _errorMessage = _item.configPath() + " must be less than half of UpdateInterval";
            return false;
        },
        "","","",
        Common::Configuration::enuConfigSource::File
        );

//TODO when No network is active application starts but does not work
void TSMonitor::run()
{
    try{
        this->pPrivate.reset(new TSMonitorPrivate);
        foreach (const QString& Key,gConfigs::TranslationServers.keys()){
            const tmplConfigurableArray<gConfigs::stuServer>& ServersConfig =
                    gConfigs::TranslationServers.values(Key);
            QMutexLocker Locker(&this->pPrivate->ListLock);
            for(size_t i=0; i<ServersConfig.size(); ++i){
                QPointer<clsTranslationServer> Server(new clsTranslationServer(Key, i));
                connect(Server.data(),&clsTranslationServer::sigResponse,
                        this->pPrivate.data(), &TSMonitorPrivate::slotProcessResponse,
                        Qt::DirectConnection);
                connect(Server.data(),&clsTranslationServer::sigDisconnected,
                        this->pPrivate.data(), &TSMonitorPrivate::slotServerDisconnected);
                connect(Server.data(), &clsTranslationServer::sigReadyForFirstRequest,
                        this->pPrivate.data(), &TSMonitorPrivate::slotSendRequest,
                        Qt::DirectConnection);

                this->pPrivate->Servers.insertMulti(Key,Server);
            }
            Locker.unlock();
        }
        this->pPrivate->slotUpdateInfo();

        this->exec();
    }catch(exTargomanBase &e){
        TargomanError(e.what());
    }catch(...){
        TargomanError("%d: FATAL Exception", __LINE__);
    }
}

quint16 TSMonitor::bestServerIndex(const QString &_dir)
{
    if (this->pPrivate.isNull())
        throw exTSMonitor("Not initialized yet.");

    qint16 BestServerScore = 0;
    qint16 BestServerIndex = 0;
    qint16 NextBestServerIndex = -1;
    static QMap<QString, qint16> LastUsedServer;
    static QMutex LastUsedServerLock;

    foreach (clsTranslationServer* Server, this->pPrivate->Servers.values(_dir)){
        if (Server->totalScore() > BestServerScore){
            NextBestServerIndex = BestServerIndex;
            BestServerIndex = Server->configIndex();
            BestServerScore = Server->totalScore();
        }
        TargomanDebug(9, "bestServerIndex()["<<_dir<<":"<<Server->configIndex()<<"] TotalScore:"<<Server->totalScore());
    }

    if (BestServerScore == 0)
        throw exTSMonitor("No server available.");

    QMutexLocker Locker(&LastUsedServerLock);
    LastUsedServer.insert(_dir, LastUsedServer.value(_dir,-1) == BestServerIndex ?
                (NextBestServerIndex < 0 ? BestServerIndex : NextBestServerIndex) : BestServerIndex);
    return LastUsedServer.value(_dir, 0);
}

void TSMonitor::wait4AtLeastOneServerAvailable()
{
    static int count=0;

    count++;
    TargomanLogInfo(1,"Waiting for at least one translation server to be ready.")
    quint16 ReadyServers = 0;
    while(ReadyServers == 0){
        QMutexLocker Locker(&this->pPrivate->ListLock);
        auto Servers = this->pPrivate->Servers;
        Locker.unlock();
        foreach (clsTranslationServer* Server, Servers){
            if (Server->totalScore() > 0){
                TargomanLogInfo(
                            1,
                            "Server "<<Server->dir()<<
                            " ID: "<<Server->configIndex()<<
                            " :"<<gConfigs::TranslationServers[Server->dir()][Server->configIndex()].Host.value()<<
                            " is ready");
                ++ReadyServers;
            }
        }
        sleep (1);
    }
    TargomanLogHappy(1, "Currently active servers: "<<ReadyServers)

}

void TSMonitorPrivate::slotUpdateInfo()
{
    try{
        size_t TempConnectedServers = 0;
        QMutexLocker Locker(&this->ListLock);
        auto Servers = this->Servers;
        Locker.unlock();

        foreach(clsTranslationServer* Server, Servers){
            Server->TotalScore = 0;
            if (Server->isConnected() == false) {
                Server->reset();
                Server->connect();
            }else if (Server->isLoggedIn()){
                emit Server->sigReadyForFirstRequest();
                ++TempConnectedServers;
            }
        }
        this->ConnectedServers = TempConnectedServers;
    }catch(exTargomanBase &e){
        TargomanError(e.what());
    }catch(...){
        TargomanError("%d: FATAL Exception", __LINE__);
    }
}

void TSMonitorPrivate::slotServerDisconnected()
{
    clsTranslationServer* Server = dynamic_cast<clsTranslationServer*>(sender());
    if(Server){
        QMutexLocker Locker(&this->ListLock);
        Server->reset();
        Locker.unlock();
        TargomanLogWarn(4,(void*)Server<<"Connection to "<<Server->dir()<<":"<<Server->configIndex()<<" has been lost.")
    }
}

void TSMonitorPrivate::slotSendRequest()
{
    try{
        clsTranslationServer* Server = dynamic_cast<clsTranslationServer*>(sender());
        if(Server){
            if (Server->LastRequestTime.isValid() &&
                    Server->LastRequestTime.secsTo(QTime::currentTime()) < TSMonitor::UpdateInterval.value())
                return;

            Server->resetScore();
            Server->LastRequestTime.restart();
            Server->sendRequest("rpcGetStatistics");
        }
    }catch(exTargomanBase &e){
        TargomanError(e.what());
    }catch(...){
        TargomanError("%d: FATAL Exception", __LINE__);
    }
}

void TSMonitorPrivate::slotProcessResponse(Common::JSONConversationProtocol::stuResponse _response)
{
    try{
        clsTranslationServer* Server = dynamic_cast<clsTranslationServer*>(sender());
        if (Server){
            quint32 Load1min = _response.Args.value("L1", 100).toInt();
            quint32 Load15min = _response.Args.value("L15", 100).toInt();
            quint32 FreeMem = _response.Args.value("FM", 0).toInt();
            quint32 TranslationQueue = _response.Args.value("TQ", 100).toInt();
            quint16 Score = (gConfigs::TranslationServers.values(Server->dir()).at(
                                 Server->configIndex()).constData().AbsoluteScore.value() * 10L) +
                    ((100 - Load1min) * 8) +
                    ((100 - TranslationQueue) * (Load15min < 70 ? 6 : 4)) +
                    FreeMem;

            Server->updateStatistics(Load1min, Load15min, FreeMem, TranslationQueue, Score);
            TargomanDebug(9, "slotProcessResponse("<<(void*)Server<<")["<<Server->dir()<<":"<<Server->configIndex()<<"] TotalScore:"<<Server->totalScore());
       }
    }catch(exTargomanBase &e){
        TargomanError(e.what());
    }catch(...){
        TargomanError("%d: FATAL Exception", __LINE__);
    }
}

TSMonitorPrivate::TSMonitorPrivate(QObject* _parent) :
    QObject(_parent)
{
    this->ConnectedServers = 0;
    this->startTimer(TSMonitor::UpdateInterval.value() * 1000);
}

void TSMonitorPrivate::timerEvent(QTimerEvent *)
{
    this->slotUpdateInfo();
}

}
}
}
