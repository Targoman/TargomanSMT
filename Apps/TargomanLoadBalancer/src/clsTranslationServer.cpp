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
 @author Behrooz Vedadian <vedadian@gmail.com>
 */

#include "clsTranslationServer.h"
#include "libTargomanCommon/SimpleAuthentication.h"

namespace Targoman {
namespace Apps{

using namespace Common;

clsTranslationServer::clsTranslationServer(size_t _configIndex):
    TotalScore(0),
    Configs(gConfigs::TranslationServers.at(_configIndex).constData()),
    ConfigIndex(_configIndex),
    LoggedIn(false)
{ }

void clsTranslationServer::connect()
{
    this->Socket.connectToHost(
                this->Configs.Host.value(),
                this->Configs.Port.value());
    this->Socket.setSocketOption(QTcpSocket::KeepAliveOption, true);
    QObject::connect(&this->Socket,SIGNAL(connected()), this, SLOT(slotConnected()), Qt::DirectConnection);
    QObject::connect(&this->Socket,SIGNAL(readyRead()), this, SLOT(slotReadyRead()), Qt::DirectConnection);
    QObject::connect(&this->Socket,SIGNAL(disconnected()), this, SLOT(slotDisconnected()), Qt::DirectConnection);
}

bool clsTranslationServer::isConnected()
{
    return this->Socket.isValid() && this->Socket.state() == QTcpSocket::ConnectedState && this->LoggedIn;
}

qint64 clsTranslationServer::sendRequest(const QString& _name, const QVariantMap& _args)
{
    QByteArray Data = JSONConversationProtocol::prepareRequest(
                JSONConversationProtocol::stuRequest(
                    _name,
                    (this->LastRequestUUID = QUuid::createUuid().toString()),
                    _args)).toUtf8();
    TargomanDebug(8,"SendTo["<<this->Configs.Host.value()<<":"<<this->Configs.Port.value()<<"]: "<<Data);
    return this->Socket.write(Data);
}

void clsTranslationServer::slotConnected()
{
    this->LastRequestUUID = QUuid::createUuid().toString();
    QVariantMap LoginArgs;
    QString UserName = this->Configs.UserName.value();
    LoginArgs.insert("l", UserName);
    LoginArgs.insert("p",
                     SimpleAuthentication::hashPass(
                         UserName,
                         this->Configs.Password.value(),
                         this->LastRequestUUID));
    this->Socket.write(JSONConversationProtocol::prepareRequest(
                           JSONConversationProtocol::stuRequest(
                               "login",
                               this->LastRequestUUID,
                               LoginArgs)).toUtf8());
}

void clsTranslationServer::slotReadyRead()
{
    QByteArray ReceivedBytes = this->Socket.readLine();
    if (ReceivedBytes.trimmed().isEmpty())
        return;
    TargomanDebug(8,"Received["<<this->Configs.Host.value()<<":"<<this->Configs.Port.value()<<"]: "<<ReceivedBytes);
    JSONConversationProtocol::stuResponse Response =
            JSONConversationProtocol::parseResponse(ReceivedBytes);

    if (Response.Type != JSONConversationProtocol::stuResponse::Ok){
        TargomanError("Invalid response: " + Response.Args.value("Message").toString());
    }else if (Response.CallUID != this->LastRequestUUID){
        TargomanWarn(3, "Ignoring response with old UUID: "+ Response.CallUID);
    }else if (this->LoggedIn){
        emit sigResponse(Response);
    }else if (Response.Result.toUInt() >= 1){
        this->LoggedIn = true;
        emit this->sigNextRequest();
    }
}

void clsTranslationServer::slotDisconnected()
{
    emit this->sigDisconnected();
    this->deleteLater();
}

}
}
