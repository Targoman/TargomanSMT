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

#include <QCoreApplication>
#include <QSettings>
#include <QFileInfo>
#include <iostream>
#include "ConfigManager.h"
#include "Private/clsConfigManager_p.h"

namespace Targoman {
namespace Common {
namespace Configuration {

ConfigManager* ConfigManager::Instance = NULL;

ConfigManager::ConfigManager() :
    pPrivate(new Private::clsConfigManagerPrivate)
{
    this->pPrivate->Initialized = false;
}

ConfigManager::~ConfigManager()
{
    //Defined just to suppress compiler error on QScoppedPointer
}

void ConfigManager::init(const QString& _license, const QStringList &_arguments)
{
    QString ErrorMessage;

    if (_arguments.indexOf(QRegExp("-h|--help")) >= 0){
        this->pPrivate->printHelp(_license);
        QCoreApplication::exit(1);
        exit(1);
        return;
    }
    //////////////////////////////////////////////////
    ///check arguments for configFileile path or set default cogFileplFile path
    //////////////////////////////////////////////////
    if (_arguments.count("-c") + _arguments.count("--config") > 1)
        throw exConfiguration("Invalid multiple ConfigManager file definition");

    int Index = _arguments.indexOf(QRegExp("-c|--config"));
    if (Index>=0){
        if (_arguments.size() <= Index + 1)
            throw exConfiguration("Invalid config option with no file name");
        this->pPrivate->ConfigFilePath = _arguments.at(Index + 1);
        if (QFileInfo(this->pPrivate->ConfigFilePath).isReadable() == false)
            throw exConfiguration("File: <"+this->pPrivate->ConfigFilePath+"> not found or can not be read.");
    }

    if (this->pPrivate->ConfigFilePath.isEmpty()){
        this->pPrivate->ConfigFilePath = QCoreApplication::applicationDirPath() + QCoreApplication::applicationName() + ".ini";
        if (QFileInfo(this->pPrivate->ConfigFilePath).isReadable() == false){
            TargomanWarn(1, "No ConfigFileile can be found. It is absolutely recomended to write one. Use --save to create one");
            this->pPrivate->ConfigFilePath.clear();
        }
    }

    //////////////////////////////////////////////////
    ///check configFileile and load everything
    //////////////////////////////////////////////////
    if (this->pPrivate->ConfigFilePath.size()){
        QSettings ConfigFileile(this->pPrivate->ConfigFilePath, QSettings::IniFormat);

        foreach (const QString& Key, ConfigFileile.allKeys()){
            if (this->pPrivate->Configs.contains(Key) == false){
                QString BasePath = Key;
                bool Found = false;
                while(BasePath.contains("/")){
                    BasePath.truncate(BasePath.lastIndexOf("/"));
                    if (this->pPrivate->Configs.value(BasePath)->argCount() < 0){
                        Found = true;
                        break;
                    }

                }
                if (Found)
                    continue; // Continue to next key
                else
                    throw exConfiguration("Configuration path <"+Key+"> is not registered");
            }
            intfConfigurable* ConfigItem  = this->pPrivate->Configs[Key];
            QVariant Value = ConfigFileile.value(Key);
            if (ConfigItem->validate(Value, ErrorMessage) == false)
                throw exConfiguration(ErrorMessage);
            else
                ConfigItem->setFromVariant(Value);
        }
    }

    //////////////////////////////////////////////////
    ///check arguments and override settings
    //////////////////////////////////////////////////
    for (auto KeyIter = _arguments.begin();
         KeyIter != _arguments.end();
         KeyIter++){
        if (*KeyIter == "-c" || *KeyIter == "--config"){
            KeyIter++;
            if (KeyIter != _arguments.end())
                continue;
        }

        if (KeyIter->startsWith("-")){
            for (auto ConfigItemIter = this->pPrivate->Configs.begin();
                 ConfigItemIter != this->pPrivate->Configs.end();
                 ConfigItemIter++){
                if ((KeyIter->startsWith("--") && *KeyIter == "--" + ConfigItemIter.value()->longSwitch()) ||
                       *KeyIter  == "-" + ConfigItemIter.value()->shortSwitch()){
                    QString Value;
                    for (qint8 i=0; i<ConfigItemIter.value()->argCount(); i++){
                        KeyIter++;
                        if (KeyIter == _arguments.end())
                            throw exConfiguration("Switch: <" +*KeyIter+ "> needs at least: " +
                                                  QString::number(ConfigItemIter.value()->argCount())+ " arguments.");
                        Value += *KeyIter + " ";
                    }
                    if (ConfigItemIter.value()->validate(Value.trimmed(), ErrorMessage) == false)
                        throw exConfiguration(ErrorMessage);
                    else
                        ConfigItemIter.value()->setFromVariant(Value.trimmed());
                }
            }
        }else
            throw exConfiguration("invalid argument <"+*KeyIter+">");
    }

    //////////////////////////////////////////////////
    ///validate all config items
    //////////////////////////////////////////////////
    foreach (intfConfigurable* ConfigItem, this->pPrivate->Configs.values()){
        if (ConfigItem->crossValidate(ErrorMessage) == false)
            throw exConfiguration(ErrorMessage);
    }

    //////////////////////////////////////////////////
    ///finalize all config items
    //////////////////////////////////////////////////
    foreach (intfConfigurable* ConfigItem, this->pPrivate->Configs.values()){
        ConfigItem->finalizeConfig();
    }

    this->pPrivate->Initialized = true;
}

void ConfigManager::save2File(const QString &_fileName, bool _backup)
{
    Q_UNUSED(_fileName)
    Q_UNUSED(_backup)
    //TODO implement me
}

void ConfigManager::addConfig(const QString _path, intfConfigurable *_item)
{
    if (this->pPrivate->Configs.contains(_path))
        throw exConfiguration("Duplicate path key: " + _path);
    this->pPrivate->Configs.insert(_path, _item);
}

void ConfigManager::addModuleInstantiaor(const QString _name, fpModuleInstantiator _instantiator)
{
    if (this->pPrivate->Configs.contains(_name))
        throw exConfiguration("Duplicate Module Name: " + _name);
    this->pPrivate->ModuleInstantiators.insert(_name, _instantiator);
}

QVariant ConfigManager::getConfig(const QString &_path, const QVariant& _default) const
{
    if (this->pPrivate->Initialized == false)
        throw exConfiguration("Configuration is not initialized yet.");

    intfConfigurable* Item= this->pPrivate->Configs.value(_path);
    if (Item->toVariant().isValid() == false)
        return Item->toVariant();
    else
        return _default;
}

fpModuleInstantiator ConfigManager::getInstantiator(const QString &_name) const
{
    if (this->pPrivate->Initialized == false)
        throw exConfiguration("Configuration is not initialized yet.");

    return this->pPrivate->ModuleInstantiators.value(_name, NULL);
}
/***********************************************************************************************/
void Private::clsConfigManagerPrivate::printHelp(const QString& _license)
{
    std::cout<<_license.toUtf8().constData()<<std::endl;
    std::cout<<"Usage:"<<std::endl;
    std::cout<<"\t-h|--help:\t Print this help"<<std::endl;
    QStringList Keys = this->Configs.keys();
    Keys.sort();
    foreach(const QString& Key, Keys){
        intfConfigurable* Item = this->Configs.value(Key);
        if (Item && (Item->shortSwitch().size() || Item->longSwitch().size())){
            std::cout<<"\t";
            if(Item->shortSwitch().size())
                std::cout<<("-" + Item->shortSwitch()).toUtf8().constData();
            if (Item->longSwitch().size()){
                if (Item->shortSwitch().size())
                    std::cout<<"|";
                std::cout<<Item->longSwitch().toUtf8().constData();
            }
            if (Item->shortHelp().size())
                std::cout<<Item->shortHelp().toUtf8().constData()<<std::endl;
            std::cout<<"\t\t\t"<<Item->description().toUtf8().constData()<<std::endl;
        }
    }
}

/***********************************************************************************************/

intfConfigurable::intfConfigurable(const QString &_configPath,
                                   const QString &_description,
                                   const QString &_shortSwitch,
                                   const QString &_shortHelp,
                                   const QString &_longSwitch)
{
    try{
        this->Description = _description;
        this->ShortSwitch = _shortSwitch;
        this->LongSwitch = _longSwitch;
        this->ShortHelp = _shortHelp;
        this->ConfigPath = _configPath;
        this->ArgCount = this->ShortHelp.split(" ").size();

        ConfigManager::instance().addConfig(_configPath, this);
    }catch(exTargomanBase &e){
        TargomanError(e.what());
        throw;
    }
}

/***********************************************************************************************/
clsModuleRegistrar::clsModuleRegistrar(const QString &_name, fpModuleInstantiator _instantiatior){
    ConfigManager::instance().addModuleInstantiaor(_name, _instantiatior);
}

}
}
}
