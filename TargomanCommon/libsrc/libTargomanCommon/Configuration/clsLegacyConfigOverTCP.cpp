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
 * @author Behrooz Vedadian <vedadian@targoman.com>
 */

#include <QJsonObject>
#include <unistd.h>
#include "Private/clsLegacyConfigOverTCP.h"
#include "JSONConversationProtocol.h"
#include "intfRPCExporter.hpp"
#include "Private/RPCRegistry.hpp"

namespace Targoman {
namespace Common {
namespace Configuration {
namespace Private {

clsLegacyConfigOverTCP::clsLegacyConfigOverTCP(clsConfigManagerPrivate &_configManager) :
    intfConfigManagerOverNet(new QTcpServer),
    ConfigManagerPrivate(_configManager),
    ActorUUID(_configManager.ActorUUID)
{}

clsLegacyConfigOverTCP::~clsLegacyConfigOverTCP()
{
    //Just to suppress compiler error using QScopedPointer
}

void clsLegacyConfigOverTCP::incomingConnection(qintptr _socketDescriptor)
{
    if(this->ConnectedClients > intfConfigManagerOverNet::MaxConnections.value()){
        QTcpSocket* Socket = new QTcpSocket;
        TargomanLogWarn(1, "Ignoring connection as max connections has reached");
        Socket->setSocketDescriptor(_socketDescriptor);
        Socket->close();
        Socket->deleteLater();
        return;
    }

    clsClientThread* CLT = new clsClientThread(_socketDescriptor,
                                               this->ConfigManagerPrivate,
                                               this);

    connect(CLT, SIGNAL(finished()), this, SLOT(slotClientDisconnected()));

    CLT->start();
    ++this->ConnectedClients;
}

void clsLegacyConfigOverTCP::slotClientDisconnected()
{
    clsClientThread* CLT = qobject_cast<clsClientThread*>(this->sender());
    CLT->deleteLater();
    --this->ConnectedClients;
}

/******************************************************************************/

clsClientThread::clsClientThread(qintptr _socketDescriptor,
                                 clsConfigManagerPrivate &_configManager,
                                 QObject* _parent):
    QThread(_parent),
    clsBaseConfigOverNet(_configManager),
    SocketDescriptor(_socketDescriptor)
{
    this->AllowedToChange = false;
    this->AllowedToView   = false;
}

void clsClientThread::slotReadyRead()
{
    try
    {
        if (this->Socket->bytesAvailable() < 3)
            return;

        if (this->Socket->bytesAvailable() > 20000)
            this->sendError(enuReturnType::InvalidStream,"Stream Data is too long");

        QByteArray ReceivedBytes =this->Socket->readLine().trimmed();
        TargomanDebug(9,"Received: "+  ReceivedBytes);

        if(ReceivedBytes.isEmpty())
            return;

        JSONConversationProtocol::stuRequest Request =
                JSONConversationProtocol::parseRequest(ReceivedBytes);
        try{
            if (Request.Name == "simplePing")
                return this->sendResult(JSONConversationProtocol::preparePong());

            if (Request.Name == "ssidPing"){
                return this->sendResult(
                            JSONConversationProtocol::preparePong(
                                this->ssidPing(Request.Args.value("ssid").toString())));
            }
            /************************************************************/
            if (Request.Name == "login"){
                QString Pass;
                if (Request.Args.contains("l")){
                    this->ActorName = Request.Args.value("l").toString();
                    Pass =      Request.Args.value("p").toString();
                }else if (Request.Args.contains("ssid"))
                    this->ActorName = "$SSID$" + Request.Args.value("ssid").toString();

                if (this->ActorName.isEmpty())
                    this->ActorName = "UNKNOWN";
                emit this->ConfigManagerPrivate.Parent.sigValidateAgent(
                            this->ActorName,
                            Pass,
                            this->Socket->peerAddress().toString(),
                            this->AllowedToView,
                            this->AllowedToChange);

                if (this->AllowedToView) {
                    TargomanLogInfo(5, QString("User <%1> Logged in with %2 Access").arg(
                                        this->ActorName).arg(
                                        this->AllowedToChange ? "ReadWrite" : "ReadOnly"));

                    if (this->AllowedToChange)
                        return this->sendResult(JSONConversationProtocol::prepareResult(
                                                    Request.CallBack,
                                                    Request.CallUID,
                                                    3));
                    else
                        return this->sendResult(JSONConversationProtocol::prepareResult(
                                                    Request.CallBack,
                                                    Request.CallUID,
                                                    1));
                } else if (this->ActorName.isEmpty()) {
                    TargomanLogWarn(6, "Attemp to login from <"<<
                                    this->Socket->peerAddress().toString()<<":"<<
                                    this->Socket->peerPort()<<"> Failed");
                    return this->sendError(enuReturnType::InvalidLogin,
                                           "Invalid User/Password");
                } else {
                    TargomanLogWarn(6,
                                    QString("User: %1 attemped to Login but not enough access").arg(
                                        this->ActorName));
                    return this->sendError(enuReturnType::InvalidLogin,
                                           QString("Not enough access"));
                }
            }
            /************************************************************/
            if (this->AllowedToView == false)
                return this->sendError(enuReturnType::InvalidLogin,
                                       QString("Invalid Request. Please login first"));
            /************************************************************/
            try{
                if (Request.Name == "walk")
                {
                    QVariantList Table = clsBaseConfigOverNet::walk(
                                Request.Args.value("dt", false).toBool());
                    QVariantMap ReturnVals;
                    ReturnVals.insert("t", Table);

                    return this->sendResult(
                                JSONConversationProtocol::prepareResult(
                                    Request.CallBack,
                                    Request.CallUID,
                                    Table.size(),
                                    ReturnVals));
                }
                /************************************************************/
                if (Request.Name == "query") {
                        return this->sendResult(
                                    JSONConversationProtocol::prepareResult(
                                        Request.CallBack,
                                        Request.CallUID,
                                        clsBaseConfigOverNet::query(
                                            Request.Args.value("p").toString()).toString()));
                }
                /************************************************************/
                if (Request.Name == "bulkQuery"){
                        QVariantList Table = clsBaseConfigOverNet::bulkQuery(
                                    Request.Args.value("p").toString(),
                                    Request.Args.value("rx",false).toBool(),
                                    Request.Args.value("dt",false).toBool(),
                                    Request.Args.value("up", -2).toInt(),
                                    Request.Args.value("tp").toString(),
                                    Request.Args.value("ss").toString());

                        QVariantMap ReturnVals;
                        ReturnVals.insert("t", Table);

                        return this->sendResult(
                                    JSONConversationProtocol::prepareResult(
                                        Request.CallBack,
                                        Request.CallUID,
                                        Table.size(),
                                        ReturnVals));

                } //if (Request.Name == "bulkQuery")

                /************************************************************/
                if (Request.Name == "set") {
                    return this->sendResult(
                                JSONConversationProtocol::prepareResult(
                                    Request.CallBack,
                                    Request.CallUID,
                                    clsBaseConfigOverNet::set(Request.Args.value("p").toString(),
                                                Request.Args.value("vl")).toString()));
                }// if (Request.Name == "set")
            }catch(exObjectNotFound &e){
                return this->sendResult(
                            JSONConversationProtocol::prepareError(
                                Request.CallBack,
                                Request.CallUID,
                                enuReturnType::ObjectNotFound,
                                e.what().mid(0, e.what().indexOf(">;"))));
            }catch(exInvalidAction &e){
                return this->sendResult(JSONConversationProtocol::prepareError(
                                            Request.CallBack,
                                            Request.CallUID,
                                            enuReturnType::InvalidAction,
                                            e.what().mid(0, e.what().indexOf(">;"))));
            }catch(exInvalidUpdateSource &e){
                return this->sendResult(JSONConversationProtocol::prepareError(
                                            Request.CallBack,
                                            Request.CallUID,
                                            enuReturnType::InvalidUpdateSource,
                                            e.what().mid(0, e.what().indexOf(">;"))));
            }catch(exInvalidData &e){
                return this->sendResult(JSONConversationProtocol::prepareError(
                                            Request.CallBack,
                                            Request.CallUID,
                                            enuReturnType::InvalidData,
                                            e.what().mid(0, e.what().indexOf(">;"))));
            }


            /************************************************************/
            if (Request.Name.startsWith("rpc")) {
                try{
                    stuRPCOutput Return =
                            RPCRegistry::instance().getRPCObject(Request.Name).invoke(Request.Args);

                    return this->sendResult(JSONConversationProtocol::prepareResult(
                                                Request.CallBack,
                                                Request.CallUID,
                                                Return.DirectResult.toString(),
                                                Return.IndirectResult));
                }catch(exRPCReg &e){
                    return this->sendResult(JSONConversationProtocol::prepareError(
                                                Request.CallBack,
                                                Request.CallUID,
                                                enuReturnType::ObjectNotFound,
                                                e.what()));
                }catch(exTargomanBase &e){
                    return this->sendResult(JSONConversationProtocol::prepareError(
                                                Request.CallBack,
                                                Request.CallUID,
                                                enuReturnType::InvalidData,
                                                e.what()));
                }
            }//if (Request.Name == "rpc")

            /************************************************************/
            //Finally seems that request name is invalid
            return this->sendResult(JSONConversationProtocol::prepareError(
                                        Request.CallBack,
                                        Request.CallUID,
                                        enuReturnType::InvalidAction,
                                        "Invalid request " + Request.Name));
        } catch (exTargomanBase &e) {
            this->sendResult(JSONConversationProtocol::prepareError(
                                 Request.CallBack,
                                 Request.CallUID,
                                 enuReturnType::Undefined,
                                 e.what()));
        }
    }catch(exTargomanBase &e){
        this->sendError(enuReturnType::InvalidStream,e.what());
    }catch(...){
        this->sendError(enuReturnType::Unknown,"FATAL unknown error");
    }
}

void clsClientThread::slotDisconnected()
{
    TargomanLogInfo(5, QString("Client with id=%3 disconnected").arg(
                        this->SocketDescriptor));
    this->Socket->deleteLater();
    exit(0);
}

void clsClientThread::run()
{
    this->Socket = new QTcpSocket;
    if(!this->Socket->setSocketDescriptor(this->SocketDescriptor)){
        emit error(this->Socket->error());
        return;
    }

    connect(this->Socket, SIGNAL(readyRead()),
            this,           SLOT(slotReadyRead()), Qt::DirectConnection);
    connect(this->Socket, SIGNAL(disconnected()), this, SLOT(slotDisconnected()));

    TargomanLogInfo(5, QString("New Configuration Admin from %1:%2 with id=%3 connected").arg(
                        this->Socket->peerAddress().toString()).arg(
                        this->Socket->peerPort()).arg(this->SocketDescriptor));

    this->exec();
}

void clsClientThread::sendError(enuReturnType::Type _type, const QString& _message)
{
    QString Message =
            JSONConversationProtocol::prepareError("", "", _type, _message);
    this->Socket->write(Message.toUtf8());

    TargomanDebug(8,"Sent and disconnected ("<<
                  this->ActorName<<"@"<<
                  this->Socket->peerAddress().toString()<<":"<<
                  this->Socket->peerPort()<<"): "<<Message);
    this->Socket->disconnectFromHost();
    this->Socket->waitForDisconnected();
}

void clsClientThread::sendResult(const QString &_data)
{
    this->Socket->write(_data.toUtf8());
    TargomanDebug(8, "Sent to Client ("<<
                  this->ActorName<<"@"<<
                  this->Socket->peerAddress().toString()<<":"<<
                  this->Socket->peerPort()<<"): "<<_data);
}

}
}
}
}
