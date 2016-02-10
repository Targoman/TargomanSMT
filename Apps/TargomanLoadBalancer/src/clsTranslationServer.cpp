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
    IsResponseReady(false),
    Configs(gConfigs::TranslationServers[_dir][_configIndex].data()),
    Socket(new QTcpSocket),
    ConfigIndex(_configIndex),
    Dir(_dir),
    LoggedIn(false),
    RequestRPC(_requestRPC),
    RequestArgs(_requestArgs)
{
    int a=2;
    a++;
}

void clsTranslationServer::connect()
{
    this->Socket->setSocketOption(QTcpSocket::KeepAliveOption, true);
    QObject::connect(this->Socket.data(),&QTcpSocket::connected,
                     this, &clsTranslationServer::slotConnected, Qt::DirectConnection);
    QObject::connect(this->Socket.data(),&QTcpSocket::readyRead,
                     this, &clsTranslationServer::slotReadyRead, Qt::DirectConnection);
    QObject::connect(this->Socket.data(),&QTcpSocket::disconnected,
                     this, &clsTranslationServer::sigDisconnected, Qt::DirectConnection);
    QObject::connect(this->Socket.data(),SIGNAL(error(QAbstractSocket::SocketError)),
                     this, SLOT(slotError(QAbstractSocket::SocketError)),Qt::DirectConnection);
    this->Socket->connectToHost(
                this->Configs.Host.value(),
                this->Configs.Port.value());

    QMutexLocker Locker(&this->ResponseLock);
    this->IsResponseReady = false;
}

bool clsTranslationServer::isConnected()
{
    return this->Socket->isValid() && this->Socket->state() == QTcpSocket::ConnectedState;
}

bool clsTranslationServer::getSafeResponse(JSONConversationProtocol::stuResponse &_response)
{
    if (this->ResponseLock.tryLock() == false)
        return false;
    if (this->IsResponseReady)
        _response = this->Response;

    bool ReturnVal = this->IsResponseReady;

    this->ResponseLock.unlock();
    return ReturnVal;
}

qint64 clsTranslationServer::sendRequest(const QString& _rpc, const QVariantMap& _args){
    QByteArray Data = JSONConversationProtocol::prepareRequest(
                JSONConversationProtocol::stuRequest(
                    _rpc,
                    (this->LastRequestUUID = QUuid::createUuid().toString()),
                    _args)).toUtf8();
    TargomanDebug(9,"SentTo["<<this->Configs.Host.value()<<":"<<this->Configs.Port.value()<<"]: "<<Data);
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

void clsTranslationServer::updateStatistics(
        quint32 _load1min,
        quint32 _load15min,
        quint32 _freeMem,
        quint32 _translationQueue,
        quint16 _score)
{
    this->Configs.Statistics.Load1MinPercent.setFromVariant(_load1min);
    this->Configs.Statistics.Load15MinPercent.setFromVariant(_load15min);
    this->Configs.Statistics.FreeMemoryPercent.setFromVariant(_freeMem);
    this->Configs.Statistics.TranslationQueuePercent.setFromVariant(_translationQueue);
    this->TotalScore = _score;
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
    TargomanLogInfo(4,"New connection to: "<<this->Configs.Host.value()<<":"<<this->Configs.Port.value()<<" Stablished");
}

void clsTranslationServer::slotReadyRead()
{
    QByteArray ReceivedBytes = this->Socket->readLine();
    if (ReceivedBytes.trimmed().isEmpty())
        return;
    TargomanDebug(9,"Received["<<this->Configs.Host.value()<<":"<<this->Configs.Port.value()<<"]: "<<ReceivedBytes);
    JSONConversationProtocol::stuResponse Response =
            JSONConversationProtocol::parseResponse(ReceivedBytes);

    if (Response.Type != JSONConversationProtocol::stuResponse::Ok){
        TargomanError("Invalid response: " + Response.Args.value("Message").toString());
    }else if (Response.CallUID != this->LastRequestUUID){
        TargomanWarn(3, "Ignoring response with old UUID: "+ Response.CallUID);
    }else if (this->LoggedIn){
        QMutexLocker Locker(&this->ResponseLock);
        this->IsResponseReady = true;
        this->Response = Response;
        emit sigResponse(this->Response);
    }else if (Response.Result.toUInt() >= 1){
        this->LoggedIn = true;
        emit sigReadyForFirstRequest();
    }
}

void clsTranslationServer::slotError(QAbstractSocket::SocketError _socketError){
    Q_UNUSED(_socketError)
   TargomanError("[%s:%d]: %s",qPrintable(this->Configs.Host.value()),this->Configs.Port.value(),qPrintable(this->Socket->errorString()));
}


void clsTranslationServer::slotDisconnected()
{
    QMutexLocker Locker(&this->ResponseLock);
    this->IsResponseReady = true;
    this->Response = JSONConversationProtocol::stuResponse(
                JSONConversationProtocol::stuResponse::Pong,
                SERVER_DISCONNECTED);
    emit this->sigDisconnected();
    TargomanWarn(4,"Connection to: "<<this->Configs.Host.value()<<":"<<this->Configs.Port.value()<<" Closed");
}

}
}
