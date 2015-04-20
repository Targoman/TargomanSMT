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

#include "Private/clsConfigManager_p.h"
#include "JSONConversationProtocol.h"
#include <QJsonObject>
#include <QtNetwork/QTcpSocket>
#include <QtConcurrent/QtConcurrent>
#include <iostream>

namespace Targoman {
namespace Common {
namespace Configuration {
namespace Private {

tmplConfigurable<int> clsConfigManagerPrivate::ListenPort(
        "Config/AdminPort",
        "If set greater than zero Initializes a network channel to monito and program application",
        0,
        [] (const intfConfigurable& _item, QString& _errorMessage){
            if (_item.toVariant().toUInt() > 60000){
                _errorMessage = "Invalid port to listen: " + _item.toVariant().toString();
                return false;
            }
            return true;
        },
        "",
        "PORT",
        "admin-port",
        enuConfigSource::Arg,
        false
        );


void clsConfigManagerPrivate::printHelp(const QString& _license)
{
    std::cout<<_license.toUtf8().constData()<<std::endl;
    std::cout<<"Usage:"<<std::endl;
    std::cout<<"\t-h|--help:\t Print this help"<<std::endl;
    std::cout<<"\t--save:\t Saves new configuration file based on old configs and input arguments"<<std::endl;
    QStringList Keys = this->Configs.keys();
    Keys.sort();
    QString LastModule = "";
    foreach(const QString& Key, Keys){
        QString Module= Key.mid(0, Key.indexOf("/"));
        intfConfigurable* Item = this->Configs.value(Key);
        if (Item && (Item->shortSwitch().size() || Item->longSwitch().size())){
            if (Module != LastModule){
                std::cout<<"\n**** "<<Module.toLatin1().constData()<<" ****";
                LastModule = Module;
            }
            std::cout<<"\n\t";
            if(Item->shortSwitch().size())
                std::cout<<("-" + Item->shortSwitch()).toUtf8().constData();
            if (Item->longSwitch().size()){
                if (Item->shortSwitch().size())
                    std::cout<<"|";
                std::cout<<"--"<<Item->longSwitch().toUtf8().constData();
            }
            if (Item->shortHelp().size())
                std::cout<<"\t"<<Item->shortHelp().toUtf8().constData();
            std::cout<<"\n\t\t"<<Item->description().toUtf8().constData()<<std::endl;
        }
    }
}

void clsConfigManagerPrivate::slotNewConnection()
{
    QTcpSocket* ClientConnection = this->TCPServer.nextPendingConnection();
    TargomanDebug(7,"New Connection from: "<<ClientConnection->peerAddress().toString());
    QtConcurrent::run(this, &clsConfigManagerPrivate::tcpClientManager, ClientConnection->socketDescriptor());
    connect(ClientConnection, SIGNAL(disconnected()), ClientConnection, SLOT(deleteLater()));
}

QList<intfConfigurable *> clsConfigManagerPrivate::configItems(const QString &_parent, bool _isRegEX, bool _reportRemote)
{
    QList<intfConfigurable *> RetVal;
    foreach(intfConfigurable* Item, this->Configs.values())
    {
        if(_isRegEX){
            if (Item->configPath().contains(QRegExp("^"+ _parent))) {
                if (_reportRemote == false && Item->remoteView() == false)
                    continue;
                RetVal.append(Item);
            }
        } else {
            if (Item->configPath().startsWith(_parent)) {
                if (_reportRemote == false && Item->remoteView() == false)
                    continue;
                RetVal.append(Item);
            }
        }
    }

    return RetVal;
}

void clsConfigManagerPrivate::tcpClientManager(int _socketDescriptor)
{
    QTcpSocket Client;
    Client.setSocketDescriptor(_socketDescriptor);
    QString AgentName;
    bool    AllowedToView = false;
    bool    AllowedToChange = false;

    TargomanInfo(5,"New Configuration Client from: "<<Client.peerAddress().toString());
    while(Client.isReadable()){
        ///@note this is not safe on windows as described in Qt's manual
        if(Client.waitForReadyRead() == false)
            break;
        QByteArray BytesReceived = Client.readAll();
        JSONConversationProtocol::stuRequest Request =
                JSONConversationProtocol::parseRequest(BytesReceived);
        try{

            if (Request.Name == "simplePing"){
                JSONConversationProtocol::stuPong Pong;
                emit this->Parent.sigPing(Pong);
                this->send(Client, JSONConversationProtocol::preparePong(Pong));
            } else if (Request.Name == "login"){
                QString Pass;
                if (Request.Args.contains("l")){
                    AgentName = Request.Args.value("l").toString();
                    Pass =      Request.Args.value("p").toString();
                }else if (Request.Args.contains("ssid"))
                    AgentName = "$SSID$" + Request.Args.value("ssid").toString();

                if (AgentName.isEmpty())
                    AgentName = "UNKNOWN";
                emit this->Parent.sigValidateAgent(AgentName,
                                                   Pass,
                                                   Client.peerAddress().toString(),
                                                   AllowedToView,
                                                   AllowedToChange);

                if (AllowedToView) {
                    if (AllowedToChange)
                        this->send(Client,
                                   JSONConversationProtocol::prepareResult(
                                       Request.CallBack,
                                       Request.CallString,
                                       3));
                    else
                        this->send(Client,
                                   JSONConversationProtocol::prepareResult(
                                       Request.CallBack,
                                       Request.CallString,
                                       1));
                    TargomanLogInfo(5, QString("User: %1 Logged in with %2 Access").arg(
                                        AgentName).arg(
                                        AllowedToChange ? "ReadWrite" : "ReadOnly"));
                } else if (AgentName.isEmpty()) {
                    TargomanLogWarn(6, "Attemp to login from"<<Client.peerAddress().toString()<<" Failed")
                            this->sendError(Client, enuReturnType::InvalidLogin, "Invalid User/Password");
                } else {
                    TargomanLogWarn(6, QString("User: %1 attemped to Login but not enough access").arg(AgentName));
                    this->sendError(Client, enuReturnType::InvalidLogin, QString("Not enough access"));
                }
                return;
            }else if (AllowedToView == false){
                this->sendError(Client, enuReturnType::InvalidLogin, QString("Invalid Request. Please login first"));
                break;
            }

            if (Request.Name == "walk")
            {
                bool ShowDetails =  Request.Args.value("dt", false).toBool();

                QVariantList TableRows;
                QVariantList Table;

                if (ShowDetails)
                    TableRows.insert(
                                TableRows.end(),
                                QVariantList()<<"Path"<<"Value"<<"Type"<<"Updatable"<<"Options");
                else
                    TableRows.insert(
                                TableRows.end(),
                                QVariantList()<<"Path"<<"Value");

                Table.append(TableRows);


                foreach(const intfConfigurable* Item, this->Configs.values()){
                    TableRows.clear();
                    if (Item->remoteView() == false)
                        continue;

                    if (ShowDetails)
                        TableRows.insert(TableRows.end(),
                                         QVariantList()<<
                                         Item->configPath()<<
                                         Item->toVariant().toString()<<
                                         Item->typeString()<<
                                         (Item->canBeConfigured(enuConfigSource::Net) &&
                                          AllowedToChange)<<
                                         Item->validValues()
                                         );
                    else
                        TableRows.insert(TableRows.end(),
                                         QVariantList()<<
                                         Item->configPath()<<
                                         Item->toVariant().toString()
                                         );

                    Table.append(TableRows);
                }
                QVariantMap ReturnVals;
                ReturnVals.insert("t", Table);
                this->send(Client,
                           JSONConversationProtocol::prepareResult(
                               Request.CallBack,
                               Request.CallString,
                               Table.length(),
                               ReturnVals));
            } else if (Request.Name == "query") {
                QString ObjectPath =  Request.Args.value("p").toString();
                intfConfigurable* ConfigItem = this->Configs.value(ObjectPath, NULL);
                if (ConfigItem){
                    this->send(Client,
                               JSONConversationProtocol::prepareResult(
                                   Request.CallBack,
                                   Request.CallString,
                                   ConfigItem->toVariant().toString()));
                }else{
                    this->send(Client,
                               JSONConversationProtocol::prepareError(
                                   Request.CallBack,
                                   Request.CallString,
                                   enuReturnType::ObjectNotFound,
                                   ObjectPath));
                }
            }else if (Request.Name == "bulkQuery"){
                QString ParenPath       = Request.Args.value("p").toString();
                bool    IsRegex         = Request.Args.value("rx",false).toBool();
                bool    ShowDetails     = Request.Args.value("dt",false).toBool();
                qint8   JustUpdatable   = Request.Args.value("up", -2).toInt();
                QString JustType        = Request.Args.value("tp").toString();
                QString StripString     = Request.Args.value("ss").toString();

                if (ParenPath.isEmpty()){
                    this->send(Client,
                               JSONConversationProtocol::prepareError(
                                   Request.CallBack,
                                   Request.CallString,
                                   enuReturnType::ObjectNotFound,
                                   ParenPath));
                    continue;
                }

                if (ParenPath.endsWith('/') == false)
                    ParenPath += IsRegex ? "(\\/|$)" : "/";

                ParenPath.replace(QRegExp("//*"),"/");

                QList<intfConfigurable*> ConfigItems = this->configItems(ParenPath,
                                                                         IsRegex,
                                                                         false);

                if (ConfigItems.isEmpty()){
                    this->send(Client,
                               JSONConversationProtocol::prepareError(
                                   Request.CallBack,
                                   Request.CallString,
                                   enuReturnType::ObjectNotFound,
                                   ParenPath));
                    continue;
                }

                QVariantList TableRows;
                QVariantList Table;

                if (ShowDetails)
                    TableRows.insert(
                                TableRows.end(),
                                QVariantList()<<"Path"<<"Value"<<"Type"<<"Updatable"<<"Options");
                else
                    TableRows.insert(
                                TableRows.end(),
                                QVariantList()<<"Path"<<"Value");

                Table.append(TableRows);

                foreach(intfConfigurable* Item, ConfigItems) {
                    if (Item->remoteView() == false
                            || (JustUpdatable != -2
                                && (Item->canBeConfigured(enuConfigSource::Net) &&
                                    AllowedToChange) != JustUpdatable)
                            || (JustType.size() &&
                                JustType != Item->typeString()))
                        continue;

                    TableRows.clear();

                    QString Path = Item->configPath();
                    if (Path.startsWith(StripString))
                        Path = Path.mid(StripString.size());

                    if (ShowDetails)
                        TableRows.insert(TableRows.end(),
                                         QVariantList()<<
                                         Item->configPath()<<
                                         Item->toVariant().toString()<<
                                         Item->typeString()<<
                                         (Item->canBeConfigured(enuConfigSource::Net) &&
                                          AllowedToChange)<<
                                         Item->validValues()
                                         );
                    else
                        TableRows.insert(TableRows.end(),
                                         QVariantList()<<
                                         Item->configPath()<<
                                         Item->toVariant().toString()
                                         );
                    Table.append(TableRows);
                }
                QVariantMap ReturnVals;
                ReturnVals.insert("t", Table);
                this->send(Client,
                           JSONConversationProtocol::prepareResult(
                               Request.CallBack,
                               Request.CallString,
                               Table.length(),
                               ReturnVals));
            } else if (Request.Name == "set") {
                if (AllowedToChange == false){
                    this->send(Client,
                               JSONConversationProtocol::prepareError(
                                   Request.CallBack,
                                   Request.CallString,
                                   enuReturnType::InvalidAction,
                                   "Not Allowed"));
                    continue;
                }

                QString ObjectPath       = Request.Args.value("p").toString();
                if (ObjectPath.isEmpty()){
                    this->send(Client,
                               JSONConversationProtocol::prepareError(
                                   Request.CallBack,
                                   Request.CallString,
                                   enuReturnType::ObjectNotFound,
                                   ObjectPath));
                    continue;
                }

                QVariant NewValue = Request.Args.value("vl");
                if (NewValue.isNull() == false) {
                    intfConfigurable* ConfigItem = this->Configs.value(ObjectPath, NULL);
                    if (ConfigItem){
                        if (ConfigItem->remoteView() == false ||
                                ConfigItem->canBeConfigured(enuConfigSource::Net) == false){
                            this->send(Client,
                                       JSONConversationProtocol::prepareError(
                                           Request.CallBack,
                                           Request.CallString,
                                           enuReturnType::InvalidUpdateSource,
                                           ObjectPath));
                        }else {
                            QString ErrorMessage;
                            QVariant OldValue = ConfigItem->toVariant();
                            try{
                                ConfigItem->setFromVariant(NewValue);
                                if (ConfigItem->crossValidate(ErrorMessage) == false){
                                    this->send(Client,
                                               JSONConversationProtocol::prepareError(
                                                   Request.CallBack,
                                                   Request.CallString,
                                                   enuReturnType::InvalidData,
                                                   ErrorMessage + " On: " + ObjectPath));
                                    ConfigItem->setFromVariant(OldValue);
                                }else{
                                    TargomanLogInfo(5, QString("User: %1 Changed value of %2 to %4").arg(
                                                AgentName).arg(
                                                ObjectPath).arg(
                                                NewValue.toString()));
                                    this->send(Client,
                                               JSONConversationProtocol::prepareResult(
                                                   Request.CallBack,
                                                   Request.CallString,
                                                   NewValue.toString()));
                                }
                            }catch(exConfiguration &e){
                                this->send(Client,
                                           JSONConversationProtocol::prepareError(
                                               Request.CallBack,
                                               Request.CallString,
                                               enuReturnType::InvalidData,
                                               e.what().split(">;ex").first() + " On: " + ObjectPath));
                            }
                        }
                    }
                }
                else
                    this->send(Client,
                               JSONConversationProtocol::prepareError(
                                   Request.CallBack,
                                   Request.CallString,
                                   enuReturnType::InvalidAction,
                                   "Unimplemented Set to NULL Command on: " + ObjectPath));
            } else if (Request.Name == "rpc") {
                QString ProcedureName = Request.Args.value("rpn").toString();

                if (ProcedureName.isEmpty()){
                    this->send(Client,
                               JSONConversationProtocol::prepareError(
                                   Request.CallBack,
                                   Request.CallString,
                                   enuReturnType::ObjectNotFound,
                                   "Invalid RPC call without procedure name"));
                    continue;
                }

                try{
                    QVariant Return;

                    emit this->Parent.sigRPC(ProcedureName, Request.Args, Return);

                    if (Return.isValid() == false){
                        this->send(Client,
                                   JSONConversationProtocol::prepareError(
                                       Request.CallBack,
                                       Request.CallString,
                                       enuReturnType::ObjectNotFound,
                                       "Procedure: " + ProcedureName +" Not Found"));
                        return;
                    }

                    QVariantList TableRows;
                    QVariantList Table;

                    TableRows.insert(
                                TableRows.end(),
                                QVariantList()<<"ArgName"<<"ArgValue");

                    Table.append(TableRows);

                    QVariantMap::iterator ArgumentsIter = Request.Args.begin();
                    while (ArgumentsIter != Request.Args.end())
                    {
                        TableRows.clear();
                        TableRows.insert(TableRows.end(),
                                         QVariantList()<<
                                         ArgumentsIter.key()<<
                                         ArgumentsIter.value());
                        Table.append(TableRows);

                        ArgumentsIter++;
                    }

                    QVariantMap ReturnVals;
                    ReturnVals.insert("t", Table);
                    this->send(Client,
                               JSONConversationProtocol::prepareResult(
                                   Request.CallBack,
                                   Request.CallString,
                                   Return.toString(),
                                   ReturnVals));

                }
                catch(exTargomanBase &e){
                    this->send(Client,
                               JSONConversationProtocol::prepareError(
                                   Request.CallBack,
                                   Request.CallString,
                                   enuReturnType::InvalidData,
                                   e.what()));
                }
            }
            else{
                this->send(Client,
                           JSONConversationProtocol::prepareError(
                               Request.CallBack,
                               Request.CallString,
                               enuReturnType::InvalidAction,
                               "Invalid request " + Request.Name));
            }
        } catch (exTargomanBase &e) {
            this->send(Client,
                       JSONConversationProtocol::prepareError(
                           Request.CallBack,
                           Request.CallString,
                           enuReturnType::Undefined,
                           e.what()));
        }
    }

    Client.disconnectFromHost();
}

void clsConfigManagerPrivate::send(QTcpSocket &_clientSocket, const QString &_data)
{
    _clientSocket.write(_data.toUtf8());
    TargomanDebug(8, "Sent to client["<<_clientSocket.peerAddress().toString()<<"]: "<<_data);
}

void clsConfigManagerPrivate::sendError(QTcpSocket &_clientSocket,
                                        enuReturnType::Type _type,
                                        const QString &_message)
{
    QString Message = JSONConversationProtocol::prepareError(
                "",
                "",
                _type,
                _message);
    _clientSocket.write(Message.toUtf8());

    TargomanDebug(8,"Sent and disconnected: " +  Message);
    _clientSocket.disconnectFromHost();
}

void clsConfigManagerPrivate::startServer()
{
    if (this->ListenPort.value() > 0){
        connect(&this->TCPServer, SIGNAL(newConnection()), this, SLOT(slotNewConnection()));
        if (this->TCPServer.listen(QHostAddress::Any, this->ListenPort.value())){
            TargomanLogHappy(5, "Administration server started on port: "<<this->ListenPort.value());
        }else
            throw exConfiguration("Unable to start administration server on port: "+
                                  this->ListenPort.toVariant().toString());
    }
}
}
}
}
}
