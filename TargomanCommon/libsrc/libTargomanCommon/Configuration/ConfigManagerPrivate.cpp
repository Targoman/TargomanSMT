/*******************************************************************************
 * Targoman: A robust Statistical Machine Translation framework                *
 *                                                                             *
 * Copyright 2014-2015 by ITRC <http://itrc.ac.ir>                           *
 *                                                                             *
 * This file is part of Targoman.                                              *
 *                                                                             *
 * Targoman is free software: you can redistribute it and/or modify            *
 * it under the terms of the GNU Lesser General Public License as published by *
 * the Free Software Foundation, either version 3 of the License, or           *
 * (at your option) any later version.                                         *
 *                                                                             *
 * Targoman is distributed in the hope that it will be useful,                 *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of              *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the               *
 * GNU Lesser General Public License for more details.                         *
 * You should have received a copy of the GNU Lesser General Public License    *
 * along with Targoman. If not, see <http://www.gnu.org/licenses/>.            *
 *                                                                             *
 *******************************************************************************/
/**
 @author S. Mohammad M. Ziabary <ziabary@targoman.com>
 @author Behrooz Vedadian <vedadian@targoman.com>
 */

#include <iostream>

#include "tmplConfigurable.h"
#include "Private/intfConfigManagerOverNet.hpp"
#include "Private/clsConfigByJsonRPC.h"
#include "Private/clsConfigManager_p.h"
#include "Private/clsLegacyConfigOverTCP.h"
#include "Validators.hpp"

#ifdef WITH_QJsonRPC
#include "libQJsonRPC/qjsonrpcabstractserver.h"
#endif

namespace Targoman {
namespace Common {
namespace Configuration {
namespace Private {

tmplConfigurable<int> intfConfigManagerOverNet::ListenPort(
        ConfigManager::moduleName() + "/Admin/Port",
        "Initializes a network channel to monitor and control application",
        10000,
        [] (const intfConfigurable& _item, QString& _errorMessage){
    if (_item.toVariant().toUInt() == 0 || _item.toVariant().toUInt() > 65000){
        _errorMessage = "Invalid port to listen: " + _item.toVariant().toString();
        return false;
    }
    return true;
},
"",
"PORT",
"admin-port",
(enuConfigSource::Type)(enuConfigSource::Arg | enuConfigSource::File),
false
);

tmplConfigurable<bool> intfConfigManagerOverNet::AdminLocal(
        ConfigManager::moduleName() + "/Admin/Local",
        "If set to true it will just listen to local connections.",
        false,
        ReturnTrueCrossValidator,
        "",
        "",
        "admin-just-local",
        (enuConfigSource::Type)(enuConfigSource::Arg | enuConfigSource::File),
        false
        );

tmplConfigurable<bool> intfConfigManagerOverNet::WaitPortReady(
        ConfigManager::moduleName() + "/Admin/WaitPortReady",
        "If set to true it will wait till port is ready checking every 500ms.",
        false,
        ReturnTrueCrossValidator,
        "",
        "",
        "admin-wait-port-ready",
        (enuConfigSource::Type)(enuConfigSource::Arg | enuConfigSource::File),
        false
        );

tmplConfigurable<int> intfConfigManagerOverNet::MaxSessionTime(
        ConfigManager::moduleName() + "/Admin/MaxSessiontime",
        "Max allowed time for a session. This is independent from idle time and must be greater. -1 means no limit",
        -1,
        [] (const intfConfigurable& _item, QString& _errorMessage){
    int MaxIdleTime = ConfigManager::instance().getConfig(ConfigManager::moduleName() + "/MaxIdleTime").toInt();
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
(enuConfigSource::Type)(enuConfigSource::Arg | enuConfigSource::File),
false
);

tmplConfigurable<int> intfConfigManagerOverNet::MaxIdleTime(
        ConfigManager::moduleName() + "/Admin/MaxIdleTime",
        "Max allowed time for a session. This is independent from idle time and must be greater. -1 means no limit",
        -1,
        [] (const intfConfigurable& _item, QString& _errorMessage){
    int MaxSessionTime = ConfigManager::instance().getConfig(ConfigManager::moduleName() + "/MaxSessiontime").toInt();
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
(enuConfigSource::Type)(enuConfigSource::Arg | enuConfigSource::File),
false
);

tmplConfigurable<quint16> intfConfigManagerOverNet::MaxConnections(
        ConfigManager::moduleName() + "/Admin/MaxConnections",
        "Max administration connections allowed",
        1,
        ReturnTrueCrossValidator,
        "",
        "MAX_ALLOWED",
        "admin-max-connections",
        (enuConfigSource::Type)(enuConfigSource::Arg | enuConfigSource::File),
        false
        );

tmplConfigurable<enuConfigOverNetMode::Type> clsConfigManagerPrivate::ConfigOverNetMode(
        ConfigManager::moduleName() + "/Admin/Mode",
        "Configuration over Net mode Can be [" + enuConfigOverNetMode::options().join("|")+"]",
        enuConfigOverNetMode::NoNetwork,
        ReturnTrueCrossValidator,
        "",
        "OVER_NET_MODE",
        "admin-mode",
        (enuConfigSource::Type)(enuConfigSource::Arg | enuConfigSource::File),
        false
        );

/**************************************************************************************************/
clsConfigManagerPrivate::clsConfigManagerPrivate(ConfigManager &_parent) :
    Parent(_parent)
{}

clsConfigManagerPrivate::~clsConfigManagerPrivate()
{
    //Just to supress compiler error on QScopped Pointer
}

void clsConfigManagerPrivate::printConfigsHelp(bool _include, const QStringList& _list, bool _showHeader)
{
    QString LastModule;
    QStringList Keys = this->Configs.keys();
    Keys.sort();
    foreach(const QString& Key, Keys){
        QString Module= Key.mid(0, Key.indexOf("/"));

        if (_include){
            if (_list.contains(Module) == false)
               continue;
        }else if (_list.contains(Module))
            continue;

        intfConfigurable* Item = this->Configs.value(Key);
        if (Item && (Item->shortSwitch().size() || Item->longSwitch().size())){
            if (_showHeader && Module != LastModule){
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

void clsConfigManagerPrivate::printHelp(const QString& _license, bool _minimal)
{
    std::cout<<_license.toUtf8().constData()<<std::endl;
    std::cout<<"Usage:"<<std::endl;
    std::cout<<"\t-h|--help \n\t\t Print this help"<<std::endl;
    std::cout<<"\t--loaded-libs \n\t\t Print a list of loaded libraries"<<std::endl;
    if (_minimal == false){
        std::cout<<"\n**** "<<ConfigManager::moduleName().toLatin1().constData()<<" ****\n";
        std::cout<<"\t-c|--config FILE_PATH\n\t\t Path to config file"<<std::endl;
        std::cout<<"\t--config-save:\n\t\t Saves new configuration file based on old configs and input arguments"<<std::endl;
        this->printConfigsHelp(true, QStringList()<<ConfigManager::moduleName(),false);
    }
    std::cout<<"\t--config-print:\n\t\t Prints all active configurations"<<std::endl;
    printConfigsHelp(true, QStringList()<<"App", true);
    printConfigsHelp(false,
                     QStringList()<<
                     ConfigManager::moduleName()<<
                     CmdIO::moduleName()<<
                     Logger::moduleName()<<
                     "App",
                     true);
    if (_minimal == false)
        printConfigsHelp(true,
                         QStringList()<<CmdIO::moduleName()<<Logger::moduleName(),
                         true);
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

void clsConfigManagerPrivate::prepareServer()
{
    switch(clsConfigManagerPrivate::ConfigOverNetMode.value()){
    case enuConfigOverNetMode::LegacyTCP:
        this->ConfigOverNetServer.reset(new clsLegacyConfigOverTCP(*this));
        break;
#ifdef WITH_QJsonRPC
    case enuConfigOverNetMode::JsonRPCOverTCP:
        this->ConfigOverNetServer.reset(new clsConfigByJsonRPC(clsConfigByJsonRPC::TCP, *this));
        break;
    case enuConfigOverNetMode::JsonRPCOverHTTP:
        this->ConfigOverNetServer.reset(new clsConfigByJsonRPC(clsConfigByJsonRPC::HTTP, *this));
        break;
#else
    case enuConfigOverNetMode::JsonRPCOverTCP:
    case enuConfigOverNetMode::JsonRPCOverHTTP:
        throw exConfiguration("JSonRPC has been disabled at compile time");
#endif
    default:
        return;
    }

    this->ConfigOverNetServer->checkPortAvailable();
}

#ifdef WITH_QJsonRPC
void clsConfigManagerPrivate::registerJsonRPCModule(QJsonRpcService &_service)
{
    switch(clsConfigManagerPrivate::ConfigOverNetMode.value()){
    case enuConfigOverNetMode::JsonRPCOverTCP:
    case enuConfigOverNetMode::JsonRPCOverHTTP:
        if (this->ConfigOverNetServer.isNull())
            throw exConfiguration("Invalid RPC registeration on uninitializaed config manager");
        ((QJsonRpcAbstractServer*)this->ConfigOverNetServer.data())->addService(&_service);
        break;
    default:
        throw exConfiguration("Unable to register JSonRPCModule on Legacy or no TCP server");


    }
}
#endif

}
}
}
}

ENUM_CONFIGURABLE_IMPL(Targoman::Common::Configuration::Private::enuConfigOverNetMode);

