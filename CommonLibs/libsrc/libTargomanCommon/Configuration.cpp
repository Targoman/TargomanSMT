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
#include "Configuration.h"
#include "Private/clsConfiguration_p.h"

namespace Targoman {
namespace Common {

Configuration* Configuration::Instance = NULL;

Configuration::Configuration() :
    pPrivate(new Private::clsConfigurationPrivate)
{
    this->pPrivate->Initialized = false;
}

Configuration::~Configuration()
{
    //Defined just to suppress compiler error on QScoppedPointer
}

void Configuration::init(const QStringList &_arguments)
{
    QString ErrorMessage;
    //////////////////////////////////////////////////
    ///check arguments for configfile path or set default configfile path
    //////////////////////////////////////////////////
    if (_arguments.count("-c") + _arguments.count("--config") > 1)
        throw exConfiguration("Invalid multiple configuration file definition");

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
            TargomanWarn(1, "No ConfigFile can be found. It is absolutely recomended to write one. Use --save to create one");
            this->pPrivate->ConfigFilePath.clear();
        }
    }

    //////////////////////////////////////////////////
    ///check configFile and load everything
    //////////////////////////////////////////////////
    if (this->pPrivate->ConfigFilePath.size()){
        QSettings ConfigFile(this->pPrivate->ConfigFilePath, QSettings::IniFormat);

        foreach (const QString& Key, ConfigFile.allKeys()){
            if (this->pPrivate->Configs.contains(Key) == false){
                QString BasePath = Key;
                bool Found = false;
                while(BasePath.contains("/")){
                    BasePath.truncate(BasePath.lastIndexOf("/"));
                    /*if (this->pPrivate->Configs.value(BasePath).Type == QVariant::UserType){
                        Found = true;
                        break;
                    }*/

                }
                if (Found)
                    continue; // Continue to next key
                else
                    throw exConfiguration("Configuration path <"+Key+"> is not registered");
            }
            clsConfigurableAbstract* ConfigItem  = this->pPrivate->Configs[Key];
            QVariant Value = ConfigFile.value(Key);
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
                if ((KeyIter->startsWith("--") && ConfigItemIter.value()->longSwitch() == "--" + *KeyIter) ||
                        ConfigItemIter.value()->shortSwitch() == "-" + *KeyIter){
                    QString Value;
                    for (size_t i=0; i<ConfigItemIter.value()->argCount(); i++){
                        Value += *KeyIter + " ";
                        KeyIter++;
                        if (KeyIter == _arguments.end())
                            throw exConfiguration("Switch: <" +*KeyIter+ "> needs at least: " +
                                                  QString::number(ConfigItemIter.value()->argCount())+ " arguments.");
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
    foreach (clsConfigurableAbstract* ConfigItem, this->pPrivate->Configs.values()){
        if (ConfigItem->crossValidate(ErrorMessage) == false)
            throw exConfiguration(ErrorMessage);
    }

    //////////////////////////////////////////////////
    /// Initialize external libraries
    //////////////////////////////////////////////////
    /// TODO: Move to engine
    ///                Text Processor
    /////////////////////////////////////////////////////
   /* {
        TextProcessor::stuConfigs TextProcessorConfigs;
        TextProcessorConfigs.AbbreviationsFile = this->getConfig("TextProcessor/AbbreviationFile").toString();
        TextProcessorConfigs.NormalizationFile = this->getConfig("TextProcessor/NormalizationFile").toString();
        TextProcessorConfigs.SpellCorrectorBaseConfigPath =
                this->getConfig("TextProcessor/SpellCorrectorBaseConfigPath").toString();

        if (this->pPrivate->ConfigFilePath.size()){
            QSettings ConfigFile(this->pPrivate->ConfigFilePath, QSettings::IniFormat);
            ConfigFile.beginGroup("TextProcessor/SpellCorrectorLanguageBasedConfigs");
            foreach(const QString& Lang, ConfigFile.childGroups()){
                ConfigFile.beginGroup(Lang);
                foreach (const QString& Key, ConfigFile.allKeys())
                    TextProcessorConfigs.SpellCorrectorLanguageBasedConfigs[Lang].insert(Key, ConfigFile.value(Key));
                ConfigFile.endGroup();
            }
            ConfigFile.endGroup();
        }
        TextProcessor::instance().init(TextProcessorConfigs);
    }*/

    this->pPrivate->Initialized = true;
}

void Configuration::save2File(const QString &_fileName, bool _backup)
{

}

void Configuration::addConfig(const QString _path, clsConfigurableAbstract *_item)
{
    if (this->pPrivate->Configs.contains(_path))
        throw exConfiguration("Duplicate path key: " + _path);
    this->pPrivate->Configs.insert(_path, _item);
}

QVariant Configuration::getConfig(const QString &_path, const QVariant& _default)
{
    if (this->pPrivate->Initialized == false)
        throw exConfiguration("Configuration is not initialized yet.");

    clsConfigurableAbstract* Item= this->pPrivate->Configs.value(_path);
    if (Item->toVariant().isValid() == false)
        return Item->toVariant();
    else
        return _default;
}

clsConfigurableAbstract::clsConfigurableAbstract(const QString &_configPath,
                                                 const QString &_description,
                                                 const QString &_shortSwitch,
                                                 const QString &_shortHelp,
                                                 const QString &_longSwitch)
{
    this->Description = _description;
    this->ShortSwitch = _shortSwitch;
    this->LongSwitch = _longSwitch;
    this->ShortHelp = _shortHelp;
    this->ConfigPath = _configPath;

    Configuration::instance().addConfig(_configPath, this);
}

}
}
