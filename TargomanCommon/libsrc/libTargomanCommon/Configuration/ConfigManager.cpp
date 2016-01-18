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

#include <QCoreApplication>
#include <QtNetwork/QTcpSocket>
#include <QSettings>
#include <QFileInfo>
#include <QSet>
#include <QDir>
#include <iostream>
#include <functional>
#include "ConfigManager.h"
#include "Private/clsConfigManager_p.h"
#include "Private/clsLegacyConfigOverTCP.h"
#include "tmplConfigurableArray.hpp"
#include "tmplConfigurableMultiMap.hpp"
#include "intfRPCExporter.hpp"
#include "Private/RPCRegistry.hpp"

namespace Targoman {
namespace Common {
namespace Configuration {

/******************************************************************************************/
ConfigManager::ConfigManager() :
    pPrivate(new Private::clsConfigManagerPrivate(*this))
{
    this->pPrivate->Initialized = false;
    this->pPrivate->SetPathsRelativeToConfigPath = true;
}

ConfigManager::~ConfigManager()
{
    //Defined just to suppress compiler error on QScoppedPointer
}

/**
 * @brief Initializes configuration managment.
 *
 * First, checks arguments for configFile path or set default configFile path. Second, checks configFile and loads all module names and sets configurable values (calls setFromvariant function of that configurable) after validating them.
 * Then, checks arguments and overrides settings. In the next step, it cross validates all config items, then calls finalizeItem for each config.
 * Finally, initializes singleton modules.
 * @note: content of Configs Map of #pPrivate is inserted in constructor methods of intfConfigurable.
 * @note: Each module has a static member of clsModuleRegistrar instance which will be constructed before main function. using that constructor that module insert itself in ModuleInstantiators Map of #pPrivate.
 *
 * @param _arguments list of arguments in QStringList format.
 * @exception throws exception if multiple configuration file defined
 * @exception throws exception if config option has no file name
 * @exception throws exception if config path not found or can not be read
 * @exception throws exception if a configurable in config file is not registered
 * @exception throws exception if a configurable could not be validated
 * @exception throws exception if not enough parameters is provided for a switch
 * @exception throws exception if invalid or unrecognized arguments are provided for switch
 * @exception throws exception if every configurable could not be cross validated
 */

void ConfigManager::init(const QString& _license,
                         const QStringList &_arguments,
                         fnAppInitializer_t _appInitializer,
                         bool _minimal)
{
    QString ErrorMessage;

    if (_arguments.indexOf(QRegExp("--loaded-libs")) >=0 ){
        Targoman::Common::printLoadedLibs();
    }

    if (_arguments.indexOf(QRegExp("-h|--help")) >= 0){
        this->pPrivate->printHelp(_license, _minimal);
        QCoreApplication::exit(1);
        exit(1);
        return;
    }

    QSet<QString> Modules;
    bool SaveFile = false;
    bool FirstTimeConfigFile = false;

    if (_minimal == false){
        // ////////////////////////////////////////////////
        // /check arguments to see wheter we must save file or not
        // ////////////////////////////////////////////////

        if (_arguments.count("--config-save"))
            SaveFile = true;

        // ////////////////////////////////////////////////
        // /check arguments for configFile path or set default cogFileplFile path
        // ////////////////////////////////////////////////
        if (_arguments.count("-c") + _arguments.count("--config") > 1)
            throw exConfiguration("Invalid multiple ConfigManager file definition");

        int Index = _arguments.indexOf(QRegExp("-c|--config"));
        if (Index>=0){
            if (_arguments.size() <= Index + 1)
                throw exConfiguration("Invalid config option with no file name");
            this->pPrivate->ConfigFilePath = _arguments.at(Index + 1);
            if (QFileInfo(this->pPrivate->ConfigFilePath).isReadable() == false){
                if (SaveFile){
                    TargomanWarn(5, "File: <"+this->pPrivate->ConfigFilePath+"> not found or can not be read. Creating new file.");
                    this->save2File(this->pPrivate->ConfigFilePath, false);
                    FirstTimeConfigFile = true;
                }else
                    throw exConfiguration("File: <"+this->pPrivate->ConfigFilePath+"> not found or can not be read.");
            }
        }

        if (this->pPrivate->ConfigFilePath.isEmpty()){
            this->pPrivate->ConfigFilePath = QCoreApplication::applicationDirPath() + QCoreApplication::applicationName() + ".ini";
            if (QFileInfo(this->pPrivate->ConfigFilePath).isReadable() == false){
                _appInitializer();
                if(SaveFile == false){
                    TargomanWarn(1, "No ConfigFile could be found. It is absolutely recomended to write one. Use --config-save to create one");
                    this->pPrivate->ConfigFilePath.clear();
                }
            }
        }

        if (this->pPrivate->ConfigFilePath.size()){
            this->pPrivate->ConfigFilePath = QFileInfo(this->pPrivate->ConfigFilePath).absoluteFilePath();
            this->pPrivate->ConfigFileDir = QFileInfo(this->pPrivate->ConfigFilePath).absoluteDir().path()+"/";
        }

        // ////////////////////////////////////////////////
        // /check configFile and load everything
        // ////////////////////////////////////////////////
        if (FirstTimeConfigFile == false &&
                this->pPrivate->ConfigFilePath.size()){
            QSharedPointer<QSettings> ConfigFile(this->configSettings());
            foreach (const QString& Key, ConfigFile->allKeys()){
                QString BasePath = Key;
                do{
                    BasePath.truncate(BasePath.lastIndexOf('/'));
                    Modules.insert('/' + BasePath);
                }while(BasePath.contains('/'));

                if (this->pPrivate->Configs.contains(Key) == false){
                    QString ParentPath = Key;
                    bool Found = false;
                    bool Generated = false;
                    do {
                        ParentPath.truncate(ParentPath.lastIndexOf('/'));
                        Configuration::intfConfigurable* ConfigItem =
                                this->pPrivate->Configs.value(ParentPath + "/");
                        if (ConfigItem &&
                                ConfigItem->canBemanaged() == false){
                            if (ConfigItem->configType() == enuConfigType::Array){
                                ConfigFile->beginGroup(ParentPath);
                                intfConfigurableArray* ConfArray = dynamic_cast<intfConfigurableArray*>(ConfigItem);
                                if (!ConfArray)
                                    throw exConfiguration("Invalid use of array flag on non array configuration");
                                ConfArray->reserve(ConfigFile->childGroups().size());
                                Generated = true;
                                ConfigFile->endGroup();
                            }else if (ConfigItem->configType() == enuConfigType::MultiMap){
                                ConfigFile->beginGroup(ParentPath);
                                intfConfigurableMultiMap* ConfMap =
                                        dynamic_cast<intfConfigurableMultiMap*>(ConfigItem);
                                if (!ConfMap)
                                    throw exConfiguration("Invalid use of multimap flag on non multimap configuration");
                                foreach(const QString& Key, ConfigFile->childGroups()){
                                    ConfigFile->beginGroup(Key);
                                    ConfMap->reserve(Key, ConfigFile->childGroups().size());
                                    ConfigFile->endGroup();
                                }
                                Generated = true;
                                ConfigFile->endGroup();
                            }
                            Found = true;
                            break;
                        }
                    } while(ParentPath.count('/') >= 1);
                    if (Generated == false){
                        if (Found)
                            continue; // Continue to next key
                        else
                            throw exConfiguration("Configuration path <"+Key+"> is not registered");
                    }else if (this->pPrivate->Configs.contains(Key) == false)
                        throw exConfiguration("Configuration path <"+Key+"> is not registered");
                }

                intfConfigurable* ConfigItem  = this->pPrivate->Configs[Key];
                if (testFlag(ConfigItem->configSources(), enuConfigSource::File) == false)
                    throw exConfiguration("Configuration path <"+Key+"> can not be configured by file");

                QVariant Value = ConfigFile->value(Key);
                if (ConfigItem->validate(Value, ErrorMessage) == false)
                    throw exConfiguration(ErrorMessage);
                else{
                    ConfigItem->setFromVariant(Value);
                    ConfigItem->setIsConfigured();
                }
            }
        }
    }
    // ////////////////////////////////////////////////
    // /check arguments and override settings
    // ////////////////////////////////////////////////
    for (auto KeyIter = _arguments.begin();
         KeyIter != _arguments.end();
         KeyIter++){
        if (*KeyIter == "-c" || *KeyIter == "--config"){
            KeyIter++;
            if (KeyIter != _arguments.end())
                continue;
        }else if(*KeyIter == "--config-save" ||
                 *KeyIter == "--config-print" ||
                 *KeyIter == "--loaded-libs"){
            continue;
        }
        bool ArgumentIsConfigItem = false;
        if (KeyIter->startsWith("-")){
            for (auto ConfigItemIter = this->pPrivate->Configs.begin();
                 ConfigItemIter != this->pPrivate->Configs.end();
                 ConfigItemIter++){
                if (testFlag(ConfigItemIter.value()->configSources(), enuConfigSource::Arg) == false)
                    continue;
                if ((KeyIter->startsWith("--") &&
                     (*KeyIter).toLower() == "--" + ConfigItemIter.value()->longSwitch()) ||
                        *KeyIter  == "-" + ConfigItemIter.value()->shortSwitch()){
                    QString Value;
                    for (qint8 i=0; i<ConfigItemIter.value()->argCount(); i++){
                        KeyIter++;
                        if (KeyIter == _arguments.end())
                            throw exConfiguration("Switch: <" +*(KeyIter - 1)+ "> needs at least: " +
                                                  QString::number(ConfigItemIter.value()->argCount())+ " arguments.");
                        Value += *KeyIter + " ";
                    }

                    if (ConfigItemIter.value()->argCount() == 0)
                        Value = "true"; //Used on boolean args with no param

                    if (ConfigItemIter.value()->validate(Value.trimmed(), ErrorMessage) == false)
                        throw exConfiguration(ErrorMessage);
                    else{
                        ConfigItemIter.value()->setFromVariant(Value.trimmed());
                        ConfigItemIter.value()->setIsConfigured();
                    }
                    ArgumentIsConfigItem = true;
                    break;
                }
            }
            if(!ArgumentIsConfigItem)
            {
                if ( KeyIter->startsWith("--") && this->pPrivate->ModuleInstantiatorsByFullName.value(KeyIter->mid(2)).IsSingleton){
                    Modules.insert(KeyIter->mid(2));
                }else{
                    throw exConfiguration("Unrecognized argument: " + *KeyIter);
                }
            }

        }else
            throw exConfiguration("invalid argument <"+*KeyIter+">");
    }

    //Do not move following to the end of this method as finalization needs an initialized ConfigManager
    this->pPrivate->Initialized = true;

    try{
        // ////////////////////////////////////////////////
        // /validate all config items
        // ////////////////////////////////////////////////
        foreach (intfConfigurable* ConfigItem, this->pPrivate->Configs.values()){
            if (ConfigItem->crossValidate(ErrorMessage) == false){
                throw exConfiguration(ErrorMessage);
            }
        }

        // ////////////////////////////////////////////////
        // /prepare TCP server
        // ////////////////////////////////////////////////
        if (_minimal == false)
            this->pPrivate->prepareServer();

        // ////////////////////////////////////////////////
        // /finalize all config items except addins
        // / * for module configurables, this puts instantiator function of
        // /   module to Instatiator member of that.
        // ////////////////////////////////////////////////
        foreach (intfConfigurable* ConfigItem, this->pPrivate->Configs.values()){
            if (ConfigItem->configType() != enuConfigType::Addin)
                ConfigItem->finalizeConfig();
        }

        // ////////////////////////////////////////////////
        // /marshal all singletons
        // ////////////////////////////////////////////////
        foreach (const QString& Module, Modules){
            stuInstantiator Instantiator = this->pPrivate->ModuleInstantiatorsByName.value(Module);
            if (Instantiator.IsSingleton && Instantiator.fpMethod)
                Instantiator.fpMethod();
        }

        // ////////////////////////////////////////////////
        // /finalize addin config items to instantiate active ones
        // ////////////////////////////////////////////////
        foreach (intfConfigurable* ConfigItem, this->pPrivate->Configs.values()){
            if (ConfigItem->configType() == enuConfigType::Addin)
                ConfigItem->finalizeConfig();
        }

        if (_minimal == false && SaveFile)
            this->save2File(this->pPrivate->ConfigFilePath, FirstTimeConfigFile ? false : true);

        if (_arguments.count("--config-print")){
            QString LastModule;
            QStringList Keys = this->pPrivate->Configs.keys();
            Keys.sort();
            foreach(const QString& Key, Keys){
                QString Module= Key.mid(0, Key.indexOf("/"));
                intfConfigurable* Item = this->pPrivate->Configs.value(Key);
                if (Item) {
                    if (Module != LastModule){
                        std::cout<<"\n**** "<<Module.toLatin1().constData()<<" ****\n";
                        LastModule = Module;
                    }
                    if(Item->configType() == enuConfigType::Normal ||
                            Item->configType() == enuConfigType::Array){
                        std::cout<<"\t";
                        std::cout<<Key.mid(Key.indexOf("/")+1).toUtf8().constData()<<" = ";
                        std::cout<<Item->toVariant().toString().toUtf8().constData()<<"\n";
                    }else{
                        std::cout<<"\t";
                        std::cout<<Key.mid(Key.indexOf("/")+1).toUtf8().constData()<<" is "<<
                                   enuConfigType::toStr(Item->configType())<<"\n";
                    }
                }
            }
        }
        TargomanLogHappy(1, "******* " <<
                         QCoreApplication::applicationName() <<
                         " (re)started *******");
    }catch(...){
        this->pPrivate->Initialized = false;
        throw;
    }
}


/**
 * @brief save current configuration to ini file (not implimented yet).
 * @param _fileName name of file to save.
 * @param _backup save backup option.
 */

void ConfigManager::save2File(const QString &_fileName, bool _backup, int _wrapLine)
{
    if (_backup){
        if (QFile::exists(_fileName))
            QFile::copy(_fileName, _fileName + ".back-" + QDateTime::currentDateTime().toString("dd-MM-yyyy_hh:mm:ss"));
    }

    QFile ConfigFile(_fileName);
    if (ConfigFile.open(QFile::WriteOnly) == false)
        throw exConfiguration("Unable to open " + _fileName + " for writing.");

    QTextStream ConfigStream(&ConfigFile);

    auto writeComments = [&](const QString& _comment) {
        static QRegExp RxDelimiter("\\b");

        QString Prepared = _comment;
        while(Prepared.size() > _wrapLine){
            int BreakPos = Prepared.lastIndexOf(RxDelimiter,_wrapLine);
            QString ToPrint = Prepared.mid(0,BreakPos);
            ConfigStream<<"# "<<ToPrint.replace("\n", "\n# ")<<"\n";
            Prepared = Prepared.mid(BreakPos).trimmed();
        }
        ConfigStream<<"# "<<Prepared.replace("\n", "\n# ")<<"\n";
    };
    auto writeLine = [&](char _ch, float _scale = 1) {
        ConfigStream<<"#"<<QString((_wrapLine - 1) * _scale, _ch)<<"\n";
    };

    auto writeSection  = [&](const QString& _sectionName) {
        writeLine('%');
        writeComments(QString((_wrapLine - 18 - _sectionName.size()) / 2, ' ') + "@@@ " + _sectionName + " SECTION @@@");
        writeLine('%');
    };

    auto writeVar = [&](const QString& _key, const QVariant& _val){
        writeLine('-',.5);
        ConfigStream<<";"<<_key<<" = "<<_val.toString()<<"\n";
    };

    ConfigStream.setCodec("UTF-8");
    writeLine('#');
    writeComments("This file was auto generated on "+QDateTime::currentDateTime().toString(Qt::ISODate));
    writeComments("Help strings are minimal and all configurations are commented. Uncomment any configuration that you want to change");
    writeLine('#');

    QStringList ConfigKeys = this->pPrivate->Configs.keys();
    ConfigKeys.sort();

    QString LastGroup = "";
    foreach (QString Key, ConfigKeys){
        QString Group = "";
        if (Key.contains('/')){
            Group= Key.mid(0,Key.indexOf('/'));
            Key = Key.mid(Group.length() + 1);
        }else{
            Group = Key;
            Key="";
        }

        intfConfigurable* Configurable = this->pPrivate->Configs.value(Group + (Key.isEmpty() ? "" : "/"+Key));
        if (!Configurable)
            Configurable = this->pPrivate->Configs.value(Group + (Key.isEmpty() ? "" : "/"+Key) + '/');
        Q_ASSERT(Configurable);


        if (LastGroup != Group){
            writeSection(Group);
            ConfigStream<<"["+Group+"]\n";
            LastGroup = Group;
        }

        writeComments(Configurable->description());
        switch(Configurable->configType()){
        case enuConfigType::Array:
            writeVar(Key + "#/... ","...");
            break;
        case enuConfigType::FileBased:
            writeVar(Key + "... ","...");
            break;
        case enuConfigType::MultiMap:
            writeVar(Key + ".../#/... ","...");
            break;

        default:
            writeComments("Valid values: " + Configurable->validValues());
            writeComments("Default value: " + Configurable->toVariant().toString());
            writeVar(Key,Configurable->toVariant());
        }

        ConfigStream<<"\n";
    }
}

/**
 * @brief Registers new configurations
 * @param _path configuration path
 * @param _item configurable item
 * @exception throws exception if a path for a configurable already exists.
 */

void ConfigManager::addConfig(const QString _path, intfConfigurable *_item)
{
    if (this->pPrivate->Configs.contains(_path))
        throw exConfiguration("Duplicate path key: " + _path);
    if (_item->shortSwitch().size() && (
                _item->shortSwitch() == "h" ||
                _item->shortSwitch() == "c"))
        throw exConfiguration("Short switch -" +
                              _item->shortSwitch() +
                              " on " + _item->configPath() +
                              " was reserved before by Config manager");
    if (_item->longSwitch().size() && (
                _item->longSwitch() == "help" ||
                _item->longSwitch() == "config" ||
                _item->longSwitch() == "config-save"))
        throw exConfiguration("Short switch -" +
                              _item->longSwitch() +
                              " on " + _item->configPath() +
                              " was reserved before by Config manager");

    foreach (intfConfigurable* ConfigItem, this->pPrivate->Configs.values()){
        if (_item->shortSwitch().size() && ConfigItem->shortSwitch() == _item->shortSwitch())
            throw exConfiguration("Short switch -" +
                                  _item->shortSwitch() +
                                  " on " + _item->configPath() +
                                  " was reserved before by: " +
                                  ConfigItem->configPath());
        if (_item->longSwitch().size() && ConfigItem->longSwitch() == _item->longSwitch())
            throw exConfiguration("Long switch --" +
                                  _item->longSwitch().toLower() +
                                  " on " + _item->configPath() +
                                  " was reserved before by: " +
                                  ConfigItem->configPath());
    }

    this->pPrivate->Configs.insert(_path, _item);
}

/**
 * @brief Registers new modules.
 * @param _name         Name of module
 * @param _instantiator A structure to encapsulate module instantiator and whether it is singleton or not.
 * @exception throws exception if a name for a module already exists.
 */

void ConfigManager::addModuleInstantiaor(const QString& _fullName,
                                         const QString& _name,
                                         const stuInstantiator &_instantiator)
{
    if (this->pPrivate->ModuleInstantiatorsByName.contains(_name) ||
        this->pPrivate->ModuleInstantiatorsByFullName.contains(_fullName))
        throw exConfiguration("Duplicate Module Name: " + _name);

    this->pPrivate->ModuleInstantiatorsByFullName.insert(_fullName, _instantiator);
    this->pPrivate->ModuleInstantiatorsByName.insert(_name, _instantiator);
}

/**
 * @brief gives value of a configurable in QVariant format.
 * @param _path     path of configurable (key in config registery Map).
 * @param _default  default value if value could not be convert to variant.
 * @exception throws exception configuration class is not initialized yet.
 */

QVariant ConfigManager::getConfig(const QString &_path, const QVariant& _default) const
{
    if (this->pPrivate->Initialized == false)
        throw exConfiguration("Configuration is not initialized yet.");

    if(_path.endsWith('/'))
        throw exConfiguration("Invalid query to non terminal path: "+ _path);

    QString Path = _path;
    if (Path.startsWith('/'))
        Path.remove(0,1);

    intfConfigurable* Item= this->pPrivate->Configs.value(Path);
    if (Item && Item->toVariant().isValid())
        return Item->toVariant();
    else
        return _default;
}

/**
 * @brief           Sets _value of a configurable in Config data member of #pPrivate using its _path.
 * @param _path     Path of configurable (key of hash map in the Config data member of #pPrivate).
 * @param _value    Value of configurable. (value of hash map in the Config data member of #pPrivate).
 * @exception       Throws exception if #pPrivate is not initialized yet.
 */

void ConfigManager::setValue(const QString &_path, const QVariant &_value) const
{
    if (this->pPrivate->Initialized == false)
        throw exConfiguration("Configuration is not initialized yet.");

    intfConfigurable* Item= this->pPrivate->Configs.value(_path);
    if (Item){
        Item->setFromVariant(_value);
        Item->setIsConfigured();
    }
}

/**
 * @brief gives instantiator function of a module.
 * @param _name     Name of module.
 * @exception throws exception if ConfigManager is not initialized yet.
 * @exception throws exception if input module is singleton (because singleton module can not be reinitialized).
 */
fpModuleInstantiator_t ConfigManager::getInstantiator(const QString &_name) const
{
    fpModuleInstantiator_t Instantiator;
    bool IsSingleton;

    this->getInstantiator(_name, Instantiator, IsSingleton);

    if (IsSingleton)
        throw exConfiguration(_name + " Is a singleton module and can not be reinstantiated");

    return Instantiator;
}

void ConfigManager::getInstantiator(const QString &_name, fpModuleInstantiator_t &_instantiator, bool &_isSingleton) const
{
    if (this->pPrivate->Initialized == false)
        throw exCofigItemNotInitialized("Configuration is not initialized yet.");

    stuInstantiator Instantiator = this->pPrivate->ModuleInstantiatorsByFullName.value(_name);
    _instantiator = Instantiator.fpMethod;
    _isSingleton = Instantiator.IsSingleton;
}

QStringList ConfigManager::registeredModules(const QString &_moduleRoot){
    QStringList AcceptableModules;
    foreach(const QString& ModuleName, this->pPrivate->ModuleInstantiatorsByFullName.keys())
        if (ModuleName.mid(0,ModuleName.lastIndexOf("::")) == _moduleRoot)
            AcceptableModules.append(ModuleName.mid(ModuleName.lastIndexOf("::") + 2, -1));
    return AcceptableModules;
}

QString ConfigManager::configFilePath()
{
    return this->pPrivate->ConfigFilePath;
}

QSharedPointer<QSettings> ConfigManager::configSettings()
{
    return this->pPrivate->configSettings(this->configFilePath());
}

QString ConfigManager::configFileDir()
{
    return this->pPrivate->ConfigFileDir;
}

QString ConfigManager::getAbsolutePath(const QString &_path)
{
    if (this->pPrivate->SetPathsRelativeToConfigPath && _path.startsWith("/") == false)
        return this->configFileDir() + _path;
    return _path;
}

bool ConfigManager::isNetworkManagable()
{
    return this->pPrivate->isNetworkBased();
}

#ifdef WITH_QJsonRPC
void ConfigManager::registerJsonRPCModule(QJsonRpcService &_service)
{
    return this->pPrivate->registerJsonRPCModule(_service);
}
#endif

void ConfigManager::startAdminServer()
{
    this->pPrivate->startNetworkListening();
}

/***********************************************************************************************/
namespace Private {

class intfConfigurablePrivate{
public:
    /**
     * @brief  updateConfig Finds location which _old configurable pointer is located in Configs hash map and changes its value with _new configurable pointer.
     * @param _old          old configurable pointer.
     * @param _new          new configurable pointer.
     */
    void updateConfig(const intfConfigurable* _old, intfConfigurable* _new){
        ConfigManager::instance().pPrivate->Configs[
                ConfigManager::instance().pPrivate->Configs.key((intfConfigurable*)_old)] = _new;
    }

    std::function<void(const intfConfigurable& _item)> Finalizer;
};

}

/**
 * @brief constructor of intfConfigurable. this is where each configurable inserts itself to Configs hash map of pPrivate member of ConfigManager class.
 */
intfConfigurable::intfConfigurable(enuConfigType::Type _configType,
                                   const clsConfigPath& _configPath,
                                   const QString &_description,
                                   const QString &_shortSwitch,
                                   const QString &_shortHelp,
                                   const QString &_longSwitch,
                                   enuConfigSource::Type _configSources,
                                   bool _remoteView,
                                   const std::function<void(const intfConfigurable& _item)> &_finalizer) :
    pPrivate(new Private::intfConfigurablePrivate)
{
    try{
        this->ConfigType  = _configType;
        this->Description = _description;
        this->ShortSwitch = _shortSwitch;
        this->LongSwitch = _longSwitch.toLower();
        this->ShortHelp = _shortHelp;
        this->pPrivate->Finalizer = _finalizer;

        this->ConfigPath = _configPath.Path;
        this->ConfigPath.replace("//", "/");
        this->ConfigPath.replace("//", "/");

        if (this->ConfigPath.startsWith('/'))
            this->ConfigPath = this->ConfigPath.mid(1);

        if (_configType == enuConfigType::Array ||
            _configType == enuConfigType::FileBased ||
            _configType == enuConfigType::MultiMap)
            if (this->ConfigPath.endsWith("/") == false)
                this->ConfigPath.append("/");
        this->ArgCount = this->shortHelp().size() ? this->ShortHelp.split(" ").size() : 0;
        this->WasConfigured = false;
        this->ConfigSources = _configSources;
        this->RemoteViewAllowed = _remoteView;

        ConfigManager::instance().addConfig(this->ConfigPath, this);
    }catch(exTargomanBase &e){
        TargomanError(e.what());
        throw;
    }
}
/**
 * @brief intfConfigurable::intfConfigurable Copy constructor of intfConfigurable class.
 *
 * This function is defined to call updateConfig function of #pPrivate.
 * Usage of this function is when we have QList of Configurables and we want to insert a new configurable to it.
 * When a temporary configurable is instantiated to be inserted into QList, its pointer will be added into Configs hash map, but that pointer will be deleted soon because it is temporary.
 * This copy constructor will be called when the temporary configurable is inserted into QList.
 * Calling updateConfig in this copy constructor, helps us to change the deleted pointer with the valid pointer of configurable which is in the QList.
 */
intfConfigurable::intfConfigurable(const intfConfigurable &_other):
    pPrivate(new Private::intfConfigurablePrivate)
{
    *this = _other;
}

intfConfigurable &intfConfigurable::operator =(const intfConfigurable &_other)
{
    this->Description = _other.Description;
    this->ShortSwitch = _other.ShortSwitch;
    this->LongSwitch = _other.LongSwitch;
    this->ShortHelp = _other.ShortHelp;
    this->ConfigPath = _other.ConfigPath;
    this->WasConfigured  = _other.WasConfigured;
    this->ConfigSources = _other.ConfigSources;
    this->ArgCount = _other.ArgCount;
    this->ConfigType = _other.ConfigType;
    this->RemoteViewAllowed = _other.RemoteViewAllowed;

    //To replace pointer of old registered config with the new copied config
    this->pPrivate->updateConfig(&_other, this);
    this->pPrivate->Finalizer = _other.pPrivate->Finalizer;
    return *this;
}

intfConfigurable::~intfConfigurable()
{
    //Just to suppress compiler error on QSopedPointer
}

void intfConfigurable::finalizeConfig()
{
    this->pPrivate->Finalizer(*this);
}

/***********************************************************************************************/
/**
 * @brief constructor of clsModuleRegistrar. This is where each module inserts its instantiator to ModuleInstantiators Map of pPrivate member of ConfigManager.
 */
clsModuleRegistrar::clsModuleRegistrar(const QString& _fullName,
                                       const QString& _name,
                                       stuInstantiator _instantiatior){
    ConfigManager::instance().addModuleInstantiaor(_fullName, _name, _instantiatior);
}
/***********************************************************************************************/
void intfRPCExporter::exportMyRPCs(){
    for (int i=0; i<this->metaObject()->methodCount(); i++)
        Private::RPCRegistry::instance().registerRPC(this,this->metaObject()->method(i));
}

}
}
}
