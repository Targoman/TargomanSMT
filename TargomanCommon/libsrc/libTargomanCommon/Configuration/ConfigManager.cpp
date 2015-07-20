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

#include <QCoreApplication>
#include <QtNetwork/QTcpSocket>
#include <QSettings>
#include <QFileInfo>
#include <QSet>
#include <QDir>
#include <iostream>
#include "ConfigManager.h"
#include "Private/clsConfigManager_p.h"
#include "Private/clsConfigManagerOverNet.h"
#include "tmplConfigurableArray.hpp"
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

void ConfigManager::init(const QString& _license, const QStringList &_arguments, bool _minimal)
{
    QString ErrorMessage;

    if (_arguments.indexOf(QRegExp("-h|--help")) >= 0){
        this->pPrivate->printHelp(_license, _minimal);
        QCoreApplication::exit(1);
        exit(1);
        return;
    }
    Targoman::Common::Logger::instance().registerActor(&this->pPrivate->ActorUUID, "ConfigManager");

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
            if (QFileInfo(this->pPrivate->ConfigFilePath).isReadable() == false && SaveFile == false){
                TargomanWarn(1, "No ConfigFile could be found. It is absolutely recomended to write one. Use --config-save to create one");
                this->pPrivate->ConfigFilePath.clear();
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
            QSettings ConfigFile(this->pPrivate->ConfigFilePath, QSettings::IniFormat);
            foreach (const QString& Key, ConfigFile.allKeys()){
                QString BasePath = Key;
                do{
                    BasePath.truncate(BasePath.lastIndexOf('/'));
                    Modules.insert(BasePath);
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
                                ConfigFile.beginGroup(ParentPath);
                                intfConfigurableArray* ConfArray = dynamic_cast<intfConfigurableArray*>(ConfigItem);
                                if (!ConfArray)
                                    throw exConfiguration("Invalid use of array flag on non array configuration");
                                ConfArray->reserve(ConfigFile.childGroups().size());
                                Generated = true;
                                ConfigFile.endGroup();
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

                QVariant Value = ConfigFile.value(Key);
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
                 *KeyIter == "--config-print"){
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
                if ( KeyIter->startsWith("--") && this->pPrivate->ModuleInstantiators.value(KeyIter->mid(2)).IsSingleton){
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
        // /finalize all config items (for module configurables, this puts instantiator
        // /function of module to Instatiator member of that.)
        // ////////////////////////////////////////////////
        foreach (intfConfigurable* ConfigItem, this->pPrivate->Configs.values()){
            ConfigItem->finalizeConfig();
        }

        // ////////////////////////////////////////////////
        // /marshal all singletons
        // ////////////////////////////////////////////////
        foreach (const QString& Module, Modules){
            stuInstantiator Instantiator = this->pPrivate->ModuleInstantiators.value(Module);
            if (Instantiator.IsSingleton && Instantiator.fpMethod)
                Instantiator.fpMethod();
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

    void ConfigManager::save2File(const QString &_fileName, bool _backup)
    {
        if (_backup){
            if (QFile::exists(_fileName))
                QFile::copy(_fileName, _fileName + ".back-" + QDateTime::currentDateTime().toString("dd-MM-yyyy_hh:mm:ss"));
        }
        QSettings ConfigFile(_fileName, QSettings::IniFormat);

        foreach (Configuration::intfConfigurable* ConfigItem, this->pPrivate->Configs.values())
            if (testFlag(ConfigItem->configSources(), enuConfigSource::File))
                ConfigFile.setValue(ConfigItem->configPath(),ConfigItem->toVariant());
        ConfigFile.sync();
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

    void ConfigManager::addModuleInstantiaor(const QString _name, const stuInstantiator &_instantiator)
    {
        if (this->pPrivate->Configs.contains(_name))
            throw exConfiguration("Duplicate Module Name: " + _name);
        this->pPrivate->ModuleInstantiators.insert(_name, _instantiator);
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
        if (this->pPrivate->Initialized == false)
            throw exConfiguration("Configuration is not initialized yet.");

        stuInstantiator Instantiator = this->pPrivate->ModuleInstantiators.value(_name);
        if (Instantiator.IsSingleton)
            throw exConfiguration(_name + " Is a singleton module and can not be reinstantiated");

        return Instantiator.fpMethod;
    }

    QString ConfigManager::configFilePath()
    {
        return this->pPrivate->ConfigFilePath;
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

    void ConfigManager::startAdminServer()
    {
        this->pPrivate->startNetworkListening();
    }

    /***********************************************************************************************/
    namespace Private {

    clsConfigManagerPrivate::clsConfigManagerPrivate(ConfigManager &_parent) :
        Parent(_parent),
        ConfigNetServer(new clsConfigNetworkServer(*this))
    {}

    clsConfigManagerPrivate::~clsConfigManagerPrivate()
    {
        //Just to supress compiler error on QScopped Pointer
    }

    void clsConfigManagerPrivate::printHelp(const QString& _license, bool _minimal)
    {
        QString LastModule = "ConfigManager";
        std::cout<<_license.toUtf8().constData()<<std::endl;
        std::cout<<"Usage:"<<std::endl;
        std::cout<<"\t-h|--help \n\t\t Print this help"<<std::endl;
        if (_minimal == false){
            std::cout<<"\n**** "<<LastModule.toLatin1().constData()<<" ****\n";
            std::cout<<"\t-c|--config FILE_PATH\n\t\t Path to config file"<<std::endl;
            std::cout<<"\t--config-save:\n\t\t Saves new configuration file based on old configs and input arguments"<<std::endl;
        }
        std::cout<<"\t--config-print:\n\t\t Prints all configurations"<<std::endl;
        QStringList Keys = this->Configs.keys();
        Keys.sort();
        foreach(const QString& Key, Keys){
            QString Module= Key.mid(0, Key.indexOf("/"));
            if (_minimal && (
                        Module == ConfigManager::moduleName() ||
                        Module == CmdIO::moduleName() ||
                        Module == Logger::moduleName()))
                continue;
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
        this->ConfigNetServer->check();
    }

    bool clsConfigManagerPrivate::isNetworkBased()
    {
        return this->ConfigNetServer->isActive();
    }

    void clsConfigManagerPrivate::startNetworkListening()
    {
        this->ConfigNetServer->start();
    }

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

    /***********************************************************************************************/
    /**
 * @brief constructor of intfConfigurable. this is where each configurable inserts itself to Configs hash map of pPrivate member of ConfigManager class.
 */
    intfConfigurable::intfConfigurable(enuConfigType::Type _configType,
                                       const QString &_configPath,
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
            if (_configPath.startsWith('/'))
                this->ConfigPath = _configPath.mid(1);
            else
                this->ConfigPath = _configPath;

            if (_configType == enuConfigType::Array ||
                    _configType == enuConfigType::FileBased)
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
    clsModuleRegistrar::clsModuleRegistrar(const QString &_name, stuInstantiator _instantiatior){
        ConfigManager::instance().addModuleInstantiaor(_name, _instantiatior);
    }
    /***********************************************************************************************/
    void intfRPCExporter::exportMyRPCs(){
        for (int i=0; i<this->metaObject()->methodCount(); i++)
            Private::RPCRegistry::instance().registerRPC(this,this->metaObject()->method(i));
    }

}
}
}
