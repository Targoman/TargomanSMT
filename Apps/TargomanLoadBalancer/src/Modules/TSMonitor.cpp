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

tmplConfigurable<quint16> TSMonitor::UpdtaeInterval(
        TSMonitor::instance().baseConfigPath() + "/UpdtaeInterval",
        "Interval to collect information from servers in seconds must be less than 100",
        1,
        Validators::tmplNumericValidator<quint8,1,100>,
        "","","",
        Common::Configuration::enuConfigSource::File
        );

tmplConfigurable<quint16> TSMonitor::WaitOnUpdtae(
        TSMonitor::instance().baseConfigPath() + "/WaitOnUpdtae",
        "miliseconds to wait before new update request this must be less than half of UpdateInterval",
        300,
        [] (const Common::Configuration::intfConfigurable& _item, QString& _errorMessage) {
    if(_item.toVariant().toUInt() < TSMonitor::UpdtaeInterval.value() * 500)
        return true;
    _errorMessage = _item.configPath() + " must be less than half of UpdateInterval";
    return false;
},
"","","",
Common::Configuration::enuConfigSource::File
);

void TSMonitor::run()
{
    try{
        this->pPrivate.reset(new TSMonitorPrivate);
        for(size_t i=0; i<gConfigs::TranslationServers.size(); ++i)
            if (gConfigs::TranslationServers.at(i).constData().Active.value() == true)
                this->pPrivate->Servers.append(new clsTranslationServer(i));
        this->exec();
    }catch(exTargomanBase &e){
        TargomanError(e.what());
    }catch(...){
        TargomanError("%d: FATAL Exception", __LINE__);
    }
}

quint16 TSMonitor::bestServerIndex()
{
    if (this->pPrivate.isNull())
        throw exTSMonitor("Not initialized yet.");

    qint16 BestServerScore = 0;
    qint16 BestServerIndex;

    foreach (clsTranslationServer* Server, this->pPrivate->Servers){
        QReadLocker Locker(&Server->RWLock);
        if (Server->TotalScore > BestServerScore){
            BestServerIndex = Server->configIndex();
            BestServerScore = Server->TotalScore;
        }
    }

    if (BestServerScore == 0)
        throw exTSMonitor("No server available.");

    return BestServerIndex;
}

void TSMonitorPrivate::slotUpdateInfo()
{
    try{
        size_t TempConnectedServers = 0;
        QMutexLocker Locker(&this->ListLock);
        foreach(clsTranslationServer* Server, this->Servers){
            Server->TotalScore = 0;
            if (Server->isConnected() == false) {
                ++TempConnectedServers;
                Server->connect();
                connect(Server,SIGNAL(sigResponse(Common::JSONConversationProtocol::stuResponse)),
                        this, SLOT(slotProcessResponse(Common::JSONConversationProtocol::stuResponse)),
                        Qt::DirectConnection);
                connect(Server,SIGNAL(sigDisconnected()), this, SLOT(slotServerDisconnected()));
                connect(Server, SIGNAL(sigNextRequest()), this, SLOT(slotSendRequest()),Qt::DirectConnection);
            }else
                emit Server->sigNextRequest();
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
        this->Servers[Server->configIndex()] = new clsTranslationServer(Server->configIndex());
        Server->deleteLater();
    }
}

void TSMonitorPrivate::slotSendRequest()
{
    try{
        clsTranslationServer* Server = dynamic_cast<clsTranslationServer*>(sender());
        if(Server){
            if (Server->LastRequestTime.isValid() &&
                    Server->LastRequestTime.secsTo(QTime::currentTime()) < TSMonitor::UpdtaeInterval.value())
                return;

            Server->LastRequestTime.restart();
            Server->TotalScore = 0;
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
            quint16 Score = (gConfigs::TranslationServers.at(
                                 Server->configIndex()).constData().AbsoluteScore.value() * 10L) +
                    ((100 - Load1min) * 8) +
                    ((100 - TranslationQueue) * (Load15min < 70 ? 6 : 4)) +
                    FreeMem;

            gConfigs::stuServer& ServerConfigs =
                    *((gConfigs::stuServer*)&gConfigs::TranslationServers[Server->configIndex()]);
            QWriteLocker Locker(&Server->RWLock);
            ServerConfigs.Statistics.Load1MinPercent.setFromVariant(Load1min);
            ServerConfigs.Statistics.Load15MinPercent.setFromVariant(Load15min);
            ServerConfigs.Statistics.FreeMemoryPercent.setFromVariant(FreeMem);
            ServerConfigs.Statistics.TranslationQueuePercent.setFromVariant(TranslationQueue);
            Server->TotalScore = Score;
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
    this->startTimer(TSMonitor::UpdtaeInterval.value() * 1000);
}

void TSMonitorPrivate::timerEvent(QTimerEvent *)
{
    this->slotUpdateInfo();
}

}
}
}
