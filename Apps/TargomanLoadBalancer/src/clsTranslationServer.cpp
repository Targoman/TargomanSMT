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

#include "clsTranslationServer.h"
#include "libTargomanCommon/SimpleAuthentication.h"

namespace Targoman {
namespace Apps{

using namespace Common;

clsTranslationServer::clsTranslationServer(const QString &_dir,
                                           size_t _configIndex,
                                           const QString &_requestRPC,
                                           const QVariantMap &_requestArgs):
    TotalScore(0),
    Configs(gConfigs::TranslationServers.values(_dir).at(_configIndex).constData()),
    Socket(new QTcpSocket),
    ConfigIndex(_configIndex),
    Dir(_dir),
    LoggedIn(false),
    RequestRPC(_requestRPC),
    RequestArgs(_requestArgs)
{}

void clsTranslationServer::connect()
{
    this->Socket->connectToHost(
                this->Configs.Host.value(),
                this->Configs.Port.value());
    this->Socket->setSocketOption(QTcpSocket::KeepAliveOption, true);
    QObject::connect(this->Socket.data(),&QTcpSocket::connected,
                     this, &clsTranslationServer::slotConnected, Qt::DirectConnection);
    QObject::connect(this->Socket.data(),&QTcpSocket::readyRead,
                     this, &clsTranslationServer::slotReadyRead, Qt::DirectConnection);
    QObject::connect(this->Socket.data(),&QTcpSocket::disconnected,
                     this, &clsTranslationServer::sigDisconnected, Qt::DirectConnection);
    this->RWLock.lockForWrite();
}

bool clsTranslationServer::isConnected()
{
    return this->Socket->isValid() && this->Socket->state() == QTcpSocket::ConnectedState;
}

qint64 clsTranslationServer::sendRequest(const QString& _rpc, const QVariantMap& _args){
    QByteArray Data = JSONConversationProtocol::prepareRequest(
                JSONConversationProtocol::stuRequest(
                    _rpc,
                    (this->LastRequestUUID = QUuid::createUuid().toString()),
                    _args)).toUtf8();
    TargomanDebug(8,"SentTo["<<this->Configs.Host.value()<<":"<<this->Configs.Port.value()<<"]: "<<Data);
    return this->Socket->write(Data);
}

qint64 clsTranslationServer::slotSendPredefinedRequest(){
    return this->sendRequest(this->RequestRPC, this->RequestArgs);
}

void clsTranslationServer::resetScore() {
    this->TotalScore = 0;
}

void clsTranslationServer::reset(){
    this->Socket.take()->deleteLater();
    this->Socket.reset(new QTcpSocket);
    this->LoggedIn = false;
    this->resetScore();
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
    this->Socket->write(JSONConversationProtocol::prepareRequest(
                           JSONConversationProtocol::stuRequest(
                               "login",
                               this->LastRequestUUID,
                               LoginArgs)).toUtf8());
}

void clsTranslationServer::slotReadyRead()
{
    QByteArray ReceivedBytes = this->Socket->readLine();
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
        this->Response = Response;
        this->RWLock.unlock();
        emit sigResponse(this->Response);
    }else if (Response.Result.toUInt() >= 1){
        this->LoggedIn = true;
        emit sigReadyForFirstRequest();
    }
}

}
}
