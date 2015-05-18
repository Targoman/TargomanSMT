/*************************************************************************
 * Copyright © 2012-2015, Targoman.com
 *
 * Published under the terms of TCRL(Targoman Community Research License)
 * You can find a copy of the license file with distributed source or
 * download it from http://targoman.com/License.txt
 *
 *************************************************************************/
/**
 @author S. Mohammad M. Ziabary <smm@ziabary.com>
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
                this->pPrivate->Servers.append(new TSMonitorPrivate::clsServerConnection(i));
        this->exec();
    }catch(...){

    }
}

const gConfigs::stuServer& TSMonitor::bestServer()
{
    if (this->pPrivate.isNull())
        throw exTSMonitor("Not initialized yet.");

    TSMonitorPrivate::clsServerConnection* BestServer;
    qint16 BestServerScore = 0;
    qint16 BestServerIndex;

    foreach (TSMonitorPrivate::clsServerConnection* Server, this->pPrivate->Servers){
        QMutexLocker Locker(&Server->Lock);
        if (Server->TotalScore > BestServerScore){
            BestServerIndex = Server->ConfigIndex;
            BestServerScore = Server->TotalScore;
        }
    }

    if (BestServerScore == 0)
        throw exTSMonitor("No server available.");

    return gConfigs::TranslationServers.at(BestServerIndex).constData();
}

void TSMonitorPrivate::slotUpdateInfo()
{
    TargomanDebug(1, "Called By Thread"<<this->thread()->currentThreadId());
    return;
    try{
        foreach(clsServerConnection* Server, this->Servers){
            Server->TotalScore = 0;
            if (Server->isValid() == false ||
                Server->state() != QTcpSocket::ConnectedState) {
                Server->connectToHost(
                            gConfigs::TranslationServers.at(Server->ConfigIndex).constData().Host.value(),
                            gConfigs::TranslationServers.at(Server->ConfigIndex).constData().Port.value());
                Server->setSocketOption(QTcpSocket::KeepAliveOption, true);
                Server->LoggedIn = false;
                connect(Server,SIGNAL(connected()), this, SLOT(slotSendRequest()), Qt::DirectConnection);
                connect(Server,SIGNAL(readyRead()), this, SLOT(slotProcessDataOnReceive()), Qt::DirectConnection);
            }else
                this->slotSendRequest();
        }
    }catch(...){

    }
}

void TSMonitorPrivate::slotSendRequest()
{
    clsServerConnection* Server = dynamic_cast<clsServerConnection*>(sender());
    if(Server){
        if (Server->LastRequestTime.isValid() &&
            Server->LastRequestTime.secsTo(QTime::currentTime()) < TSMonitor::UpdtaeInterval.value())
            return;
        Server->LastRequestUUID = QUuid::createUuid().toString();
        Server->LastRequestTime.restart();
        Server->TotalScore = 0;
        if (Server->LoggedIn == false){
            QVariantMap LoginArgs;
            LoginArgs.insert("l",
                             gConfigs::TranslationServers.at(
                                 Server->ConfigIndex).constData().UserName.value());
            LoginArgs.insert("p",
                             SimpleAuthentication::hashPass(
                                 Server->LastRequestUUID,
                                 gConfigs::TranslationServers.at(
                                     Server->ConfigIndex).constData().Password.value()));
                             ;
            Server->write(JSONConversationProtocol::prepareRequest(
                                     JSONConversationProtocol::stuRequest(
                                         "login",
                                         Server->LastRequestUUID,
                                         LoginArgs)).toUtf8());
        }else
            Server->write(JSONConversationProtocol::prepareRequest(
                                     JSONConversationProtocol::stuRequest(
                                         "rpcGetStatistics",
                                         Server->LastRequestUUID)).toUtf8());
    }
}

void TSMonitorPrivate::slotProcessDataOnReceive()
{
    clsServerConnection* Server = dynamic_cast<clsServerConnection*>(sender());
    if (Server){
        QByteArray ReceivedBytes = Server->readLine();
        TargomanDebug(9,"Received: "+  ReceivedBytes);
        JSONConversationProtocol::stuResponse Response =
                JSONConversationProtocol::parseResponse(ReceivedBytes);

        if (Response.Type != JSONConversationProtocol::stuResponse::Ok){
            TargomanError("Invalid response: " + Response.Args.value("Message").toString());
        }else if (Response.CallUID != Server->LastRequestUUID){
            TargomanWarn(3, "Ignoring response with old UUID: "+ Response.CallUID);
        }else if (Server->LoggedIn){
            quint32 Load1min = Response.Args.value("L1", 100).toInt();
            quint32 Load15min = Response.Args.value("L15", 100).toInt();
            quint32 FreeMem = Response.Args.value("FM", 0).toInt();
            quint32 TranslationQueue = Response.Args.value("TQ", 100).toInt();
            quint16 Score = (gConfigs::TranslationServers.at(
                                 Server->ConfigIndex).constData().AbsoluteScore.value() * 10L) +
                           ((100 - Load1min) * 8) +
                           ((100 - TranslationQueue) * (Load15min < 70 ? 6 : 4)) +
                           FreeMem;

            gConfigs::stuServer& ServerConfigs =
                    *((gConfigs::stuServer*)&gConfigs::TranslationServers[Server->ConfigIndex]);
            QMutexLocker Locker(&Server->Lock);
            ServerConfigs.Statistics.Load1MinPercent.setFromVariant(Load1min);
            ServerConfigs.Statistics.Load15MinPercent.setFromVariant(Load15min);
            ServerConfigs.Statistics.FreeMemoryPercent.setFromVariant(FreeMem);
            ServerConfigs.Statistics.TranslationQueuePercent.setFromVariant(TranslationQueue);
            Server->TotalScore = Score;
        }else{
            if (Response.Result.toUInt() >= 1){
                Server->LoggedIn = true;
                this->slotSendRequest(); //Send get information request
            }
        }
    }
}



TSMonitorPrivate::TSMonitorPrivate(QObject* _parent) :
    QObject(_parent)
{
    this->startTimer(TSMonitor::UpdtaeInterval.value() * 1000);
}

void TSMonitorPrivate::timerEvent(QTimerEvent *)
{
    this->slotUpdateInfo();
}

}
}
}
