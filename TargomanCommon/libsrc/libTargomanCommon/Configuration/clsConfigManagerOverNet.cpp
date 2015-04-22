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

#include <QJsonObject>
#include <QtConcurrent/QtConcurrent>
#include "Private/clsConfigManagerOverNet.h"
#include "JSONConversationProtocol.h"

namespace Targoman {
namespace Common {
namespace Configuration {
namespace Private {

using namespace Network;

tmplConfigurable<int> clsConfigNetworkServer::ListenPort(
        "ConfigManager/AdminPort",
        "If set greater than zero Initializes a network channel to monitor and control application",
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

tmplConfigurable<bool> clsConfigNetworkServer::AdminLocal(
        "ConfigManager/AdminLocal",
        "If set to true it will just listen to local connections.",
        false,
        [] (const intfConfigurable&, QString&){
            return true;
        },
        "",
        "",
        "admin-just-local",
        enuConfigSource::Arg,
        false
        );

tmplConfigurable<int> clsConfigNetworkServer::MaxSessionTime(
        "ConfigManager/MaxSessiontime",
        "Max allowed time for a session. This is independent from idle time and must be greater. -1 means no limit",
        -1,
        [] (const intfConfigurable& _item, QString& _errorMessage){
            int MaxIdleTime = ConfigManager::instance().getConfig("ConfigManager/MaxIdleTime").toInt();
            if (_item.toVariant().toInt() >= 0 &&
                    (MaxIdleTime <0 ||
                     _item.toVariant().toInt() < MaxIdleTime)){
                _errorMessage = "Invalid Max Session Time. It must be greater than Max IdleTime";
                return false;
            }
            return true;
        },
        "",
        "SECONDS",
        "admin-max-session-time",
        enuConfigSource::Arg,
        false
        );

tmplConfigurable<int> clsConfigNetworkServer::MaxIdleTime(
        "ConfigManager/MaxIdleTime",
        "Max allowed time for a session. This is independent from idle time and must be greater. -1 means no limit",
        -1,
        [] (const intfConfigurable& _item, QString& _errorMessage){
            int MaxSessionTime = ConfigManager::instance().getConfig("ConfigManager/MaxSessiontime").toInt();
            if (_item.toVariant().toInt() == 0 || (
                        _item.toVariant().toInt() > 0 &&
                        (MaxSessionTime > 0 ||
                         _item.toVariant().toInt() < MaxSessionTime))){
                _errorMessage = "Invalid Max Idle Time. It must be greater than zero but less than Max Session Time";
                return false;
            }
            return true;
        },
        "",
        "SECONDS",
        "admin-max-idle-time",
        enuConfigSource::Arg,
        false
        );

tmplConfigurable<quint16> clsConfigNetworkServer::MaxConnections(
        "ConfigManager/MaxConnections",
        "Max administration connections allowed",
        1,
        [] (const intfConfigurable&, QString&){
            return true;
        },
        "",
        "MAX_ALLOWED",
        "max-session-time",
        enuConfigSource::Arg,
        false
        );

clsConfigNetworkServer::clsConfigNetworkServer(
        clsConfigManagerPrivate &_configManager,
        bool _useWebSocketServer) :
    ConfigManagerPrivate(_configManager),
    ActorUUID(_configManager.ActorUUID)
{
    if (_useWebSocketServer)
        throw exTargomanNotImplemented("WebSocketServer will be available in Qt 5.4");
    this->CanStartListening = false;
    //connect(QCoreApplication::instance(), SIGNAL(aboutToQuit()), this, SLOT(slotShutdown()));
}

clsConfigNetworkServer::~clsConfigNetworkServer()
{
    //Just to suppress compiler erro using QScopedPointer
}

void clsConfigNetworkServer::checkPortAvailability()
{
    QTcpServer DummyTCPServer;

    if (!DummyTCPServer.listen(this->AdminLocal.value() ? QHostAddress::LocalHost : QHostAddress::Any,
                               this->ListenPort.value()))
        throw exConfigurationServer(QString("Seems that port: %1 is in use").arg(
                                        this->ListenPort.value()));

    DummyTCPServer.close();
}

void clsConfigNetworkServer::slotShutdown()
{
    TargomanLogWarn(5,"Shutting down configuration Network Server");

    try{
        this->TCPServer->close();
        this->exit();
    }catch(...){
    }
}

void clsConfigNetworkServer::run()
{
    QThread::setTerminationEnabled(false);

    TargomanDebug(5, "ConfigurationAdminServer Thread Started");

    this->TCPServer.reset(new clsTCPServer(this->ConfigManagerPrivate, this->MaxConnections.value()));
    connect(this->TCPServer.data(), SIGNAL(sigValidateAgent(QString&,const QString&,const QString&,bool&,bool&)),
            this,                   SIGNAL(sigValidateAgent(QString&,const QString&,const QString&,bool&,bool&)),
            Qt::DirectConnection);
    connect(this->TCPServer.data(),SIGNAL(sigRPC(QString,QVariantMap&, QVariant&)),
            this,                  SIGNAL(sigRPC(QString,QVariantMap&, QVariant&)),
            Qt::DirectConnection);
    connect(this->TCPServer.data(),  SIGNAL(sigPing(JSONConversationProtocol::stuPong&)),
            this,                    SIGNAL(sigPing(JSONConversationProtocol::stuPong&)),
            Qt::DirectConnection);

    this->setTerminationEnabled(true);

    while(this->CanStartListening == false)
        sleep(1);

    if (!TCPServer->listen(this->AdminLocal.value() ? QHostAddress::LocalHost : QHostAddress::Any,
                           this->ListenPort.value()))
        throw exConfigurationServer(QString("Unable to Start Server listening on: %1:%2").arg(
                                        this->AdminLocal.value() ? "localhost" : "0.0.0.0").arg(
                                        this->ListenPort.value()));
    TargomanInfo(5, QString("Configuration Network Server listening on: %1:%2").arg(
                        this->AdminLocal.value() ? "localhost" : "0.0.0.0").arg(
                        this->ListenPort.value()));

    try{
        this->exec();
    }
    catch(...)
    {}
}

/******************************************************************************/
clsTCPServer::clsTCPServer(clsConfigManagerPrivate &_configManager,
                           int _maxConnections):
    Network::clsBaseTCPServer("OMC", new clsConnectionManager(_maxConnections)),
    ConfigManagerPrivate(_configManager),
    ActorUUID(_configManager.ActorUUID)
{
    connect(this, SIGNAL(sigNewClient(QHostAddress,quint16,QString)),
            this,  SLOT(slotNewClient(QHostAddress,quint16,QString)));
    connect(this, SIGNAL(sigClientRemoved(QHostAddress,quint16,QString)),
            this,  SLOT(slotClientRemoved(QHostAddress,quint16,QString)));
}

clsBaseClientManager* clsTCPServer::newClientInstance(const QString &, qint32 )
{
    clsClientManager* CLM = new clsClientManager(this->ConnectionManager->clientId(),
                                                 this->ConfigManagerPrivate,
                                                 this);

    connect(CLM, SIGNAL(sigValidateAgent(QString&,const QString&,const QString&,bool&,bool&)),
            this,SIGNAL(sigValidateAgent(QString&,const QString&,const QString&,bool&,bool&)),
            Qt::DirectConnection);
    connect(CLM, SIGNAL(sigRPC(QString,QVariantMap&, QVariant&)),
            this,SIGNAL(sigRPC(QString,QVariantMap&, QVariant&)),
            Qt::DirectConnection);
    connect(CLM, SIGNAL(sigPing(JSONConversationProtocol::stuPong&)),
            this,SIGNAL(sigPing(JSONConversationProtocol::stuPong&)),
            Qt::DirectConnection);

    return CLM;
}

void clsTCPServer::slotNewClient(const QHostAddress& _host, quint16 _port, const QString& _id)
{
    TargomanLogInfo(5, QString("New Configuration Admin from %1:%2 with id=%3 connected").arg(
                        _host.toString()).arg(_port).arg(_id));
}

void clsTCPServer::slotClientRemoved(const QHostAddress& _host, quint16 _port, const QString& _id)
{
    TargomanLogInfo(5, qPrintable(QString("Configuration Admin from %1:%2 with id=%3 Closed").arg(
                                      _host.toString()).arg(_port).arg(_id)));
}

/******************************************************************************/
const QByteArray clsConnectionManager::clientId() const
{
    return QByteArray::number(++this->TotalConnections);
}

/******************************************************************************/

clsClientManager::clsClientManager(const QByteArray& _id,
                                   clsConfigManagerPrivate &_configManager,
                                   QObject* _parent):
    clsBaseClientManager(_id, _parent),
    ConfigManagerPrivate(_configManager),
    ActorUUID(_configManager.ActorUUID)
{
    this->AllowedToChange = false;
    this->AllowedToView   = false;
}

void clsClientManager::processIncomingData()
{
    try
    {
        if (this->bytesAvailable() < 3)
            return;

        if (this->bytesAvailable() > 20000)
            this->sendError(enuReturnType::InvalidStream,"Stream Data is too long");

        QByteArray ReceivedBytes =this->getAllData();
        TargomanDebug(9,"Received: "+  ReceivedBytes);

        JSONConversationProtocol::stuRequest Request =
                JSONConversationProtocol::parseRequest(ReceivedBytes);
        try{
            if (Request.Name == "simplePing"){
                JSONConversationProtocol::stuPong Pong;
                emit this->ConfigManagerPrivate.Parent.sigPing(Pong);
                return this->sendResult(JSONConversationProtocol::preparePong(Pong));
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
                            this->peerAddress().toString(),
                            this->AllowedToView,
                            this->AllowedToChange);

                if (this->AllowedToView) {
                    TargomanLogInfo(5, QString("User: %1 Logged in with %2 Access").arg(
                                        this->ActorName).arg(
                                        this->AllowedToChange ? "ReadWrite" : "ReadOnly"));

                    if (this->AllowedToChange)
                        return this->sendResult(JSONConversationProtocol::prepareResult(
                                              Request.CallBack,
                                              Request.CallString,
                                              3));
                    else
                        return this->sendResult(JSONConversationProtocol::prepareResult(
                                              Request.CallBack,
                                              Request.CallString,
                                              1));
                } else if (this->ActorName.isEmpty()) {
                    TargomanLogWarn(6, "Attemp to login from"<<
                                    this->peerAddress().toString()<<":"<<
                                    this->peerPort()<<" Failed");
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

                foreach(const intfConfigurable* Item, this->ConfigManagerPrivate.Configs.values()){
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
                                          this->AllowedToChange)<<
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
                return this->sendResult(JSONConversationProtocol::prepareResult(
                                      Request.CallBack,
                                      Request.CallString,
                                      Table.length(),
                                      ReturnVals));
            }
            /************************************************************/
            if (Request.Name == "query") {
                QString ObjectPath =  Request.Args.value("p").toString();
                intfConfigurable* ConfigItem = this->ConfigManagerPrivate.Configs.value(ObjectPath, NULL);
                if (ConfigItem)
                    return this->sendResult(JSONConversationProtocol::prepareResult(
                                          Request.CallBack,
                                          Request.CallString,
                                          ConfigItem->toVariant().toString()));
                else
                    return this->sendResult(JSONConversationProtocol::prepareError(
                                          Request.CallBack,
                                          Request.CallString,
                                          enuReturnType::ObjectNotFound,
                                          ObjectPath));
            }
            /************************************************************/
            if (Request.Name == "bulkQuery"){
                QString ParenPath       = Request.Args.value("p").toString();
                bool    IsRegex         = Request.Args.value("rx",false).toBool();
                bool    ShowDetails     = Request.Args.value("dt",false).toBool();
                qint8   JustUpdatable   = Request.Args.value("up", -2).toInt();
                QString JustType        = Request.Args.value("tp").toString();
                QString StripString     = Request.Args.value("ss").toString();

                if (ParenPath.isEmpty())
                    return this->sendResult(JSONConversationProtocol::prepareError(
                                          Request.CallBack,
                                          Request.CallString,
                                          enuReturnType::ObjectNotFound,
                                          ParenPath));

                if (ParenPath.endsWith('/') == false)
                    ParenPath += IsRegex ? "(\\/|$)" : "/";

                ParenPath.replace(QRegExp("//*"),"/");

                QList<intfConfigurable*> ConfigItems =
                        this->ConfigManagerPrivate.configItems(ParenPath, IsRegex, false);

                if (ConfigItems.isEmpty())
                    return this->sendResult(JSONConversationProtocol::prepareError(
                                          Request.CallBack,
                                          Request.CallString,
                                          enuReturnType::ObjectNotFound,
                                          ParenPath));

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
                                    this->AllowedToChange) != JustUpdatable)
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
                                          this->AllowedToChange)<<
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
                return this->sendResult(JSONConversationProtocol::prepareResult(
                                      Request.CallBack,
                                      Request.CallString,
                                      Table.length(),
                                      ReturnVals));
            } //if (Request.Name == "bulkQuery")

            /************************************************************/
            if (Request.Name == "set") {
                if (this->AllowedToChange == false)
                    return this->sendResult(JSONConversationProtocol::prepareError(
                                          Request.CallBack,
                                          Request.CallString,
                                          enuReturnType::InvalidAction,
                                          "Not Allowed"));

                QString ObjectPath       = Request.Args.value("p").toString();
                if (ObjectPath.isEmpty())
                    return this->sendResult(JSONConversationProtocol::prepareError(
                                          Request.CallBack,
                                          Request.CallString,
                                          enuReturnType::ObjectNotFound,
                                          ObjectPath));

                QVariant NewValue = Request.Args.value("vl");
                if (NewValue.isNull() == false) {
                    intfConfigurable* ConfigItem =
                            this->ConfigManagerPrivate.Configs.value(ObjectPath, NULL);
                    if (ConfigItem){
                        if (ConfigItem->remoteView() == false ||
                                ConfigItem->canBeConfigured(enuConfigSource::Net) == false)
                            return this->sendResult(JSONConversationProtocol::prepareError(
                                                  Request.CallBack,
                                                  Request.CallString,
                                                  enuReturnType::InvalidUpdateSource,
                                                  ObjectPath));

                        QString ErrorMessage;
                        QVariant OldValue = ConfigItem->toVariant();
                        try{
                            ConfigItem->setFromVariant(NewValue);
                            if (ConfigItem->crossValidate(ErrorMessage) == false){
                                ConfigItem->setFromVariant(OldValue);
                                return this->sendResult(JSONConversationProtocol::prepareError(
                                                      Request.CallBack,
                                                      Request.CallString,
                                                      enuReturnType::InvalidData,
                                                      ErrorMessage + " On: " + ObjectPath));
                            }else{
                                TargomanLogInfo(5, QString("User: %1 Changed value of %2 to %4").arg(
                                                    this->ActorName).arg(
                                                    ObjectPath).arg(
                                                    NewValue.toString()));
                                return this->sendResult(
                                                  JSONConversationProtocol::prepareResult(
                                                      Request.CallBack,
                                                      Request.CallString,
                                                      NewValue.toString()));
                            }//else
                        }catch(exConfiguration &e){
                            return this->sendResult(JSONConversationProtocol::prepareError(
                                                  Request.CallBack,
                                                  Request.CallString,
                                                  enuReturnType::InvalidData,
                                                  e.what().split(">;ex").first() + " On: " + ObjectPath));
                        }//catch
                    }else
                        return this->sendResult(JSONConversationProtocol::prepareError(
                                              Request.CallBack,
                                              Request.CallString,
                                              enuReturnType::ObjectNotFound,
                                              ObjectPath));
                } else
                    this->sendResult(JSONConversationProtocol::prepareError(
                                   Request.CallBack,
                                   Request.CallString,
                                   enuReturnType::InvalidAction,
                                   "Unimplemented Set to NULL Command on: " + ObjectPath));
            }// if (Request.Name == "set")

            /************************************************************/
            if (Request.Name == "rpc") {
                QString ProcedureName = Request.Args.value("rpn").toString();

                if (ProcedureName.isEmpty())
                    return this->sendResult(JSONConversationProtocol::prepareError(
                                          Request.CallBack,
                                          Request.CallString,
                                          enuReturnType::ObjectNotFound,
                                          "Invalid RPC call without procedure name"));

                try{
                    QVariant Return;
                    emit this->ConfigManagerPrivate.Parent.sigRPC(ProcedureName, Request.Args, Return);

                    if (Return.isValid() == false){
                        this->sendResult(JSONConversationProtocol::prepareError(
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
                    return this->sendResult(JSONConversationProtocol::prepareResult(
                                          Request.CallBack,
                                          Request.CallString,
                                          Return.toString(),
                                          ReturnVals));

                }catch(exTargomanBase &e){
                    return this->sendResult(JSONConversationProtocol::prepareError(
                                          Request.CallBack,
                                          Request.CallString,
                                          enuReturnType::InvalidData,
                                          e.what()));
                }
            }//if (Request.Name == "rpc")

            /************************************************************/
            //Finally seems that request name is invalid
            return this->sendResult(JSONConversationProtocol::prepareError(
                                  Request.CallBack,
                                  Request.CallString,
                                  enuReturnType::InvalidAction,
                                  "Invalid request " + Request.Name));
        } catch (exTargomanBase &e) {
            this->sendResult(JSONConversationProtocol::prepareError(
                           Request.CallBack,
                           Request.CallString,
                           enuReturnType::Undefined,
                           e.what()));
        }
    }catch(exTargomanBase &e){
        this->sendError(enuReturnType::InvalidStream,e.what());
    }
}

void clsClientManager::sendError(enuReturnType::Type _type, const QString& _message)
{
    QString Message =
            JSONConversationProtocol::prepareError("", "", _type, _message);
    this->write(Message.toUtf8());

    TargomanDebug(8,"Sent and disconnected to("<<
                  this->peerAddress().toString()<<":"<<
                  this->peerPort()<<"): "<<Message);
    this->disconnectFromHost();
    this->waitForDisconnected();
}

void clsClientManager::sendResult(const QString &_data)
{
    this->write(_data.toUtf8());
    TargomanDebug(8, "Sent to Client ("<<
                  this->peerAddress().toString()<<":"<<
                  this->peerPort()<<"): "<<_data);
}

}
}
}
}
