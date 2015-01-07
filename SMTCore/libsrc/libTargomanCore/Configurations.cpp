/*************************************************************************
 * Copyright © 2012-2014, Targoman.com
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
#include "Configurations.h"
#include "libTargomanTextProcessor/TextProcessor.h"
using Targoman::NLPLibs::TextProcessor;
#include "Private/clsConfigurations_p.h"


namespace Targoman {
namespace Core {

using namespace Private;

Configurations* Configurations::Instance = NULL;
Configurations& gConfigs = Configurations::instance();


Configurations::Configurations() :
    pPrivate(new Private::clsConfigurationPrivate)
{
    this->pPrivate->Initialized = false;

    this->addConfig("TextProcessor/NormalizationFile", QVariant::String, "", 1,
                    NULL,"","","",
                    "Normalization File");
    this->addConfig("TextProcessor/AbbreviationFile", QVariant::String, "", 1,
                    NULL,"","","",
                    "Abbreviation List File");
    this->addConfig("TextProcessor/SpellCorrectorBaseConfigPath", QVariant::String, "", 1,
                    NULL,"","","",
                    "Abbreviation List File");
    this->addConfig("TextProcessor/SpellCorrectorLanguageBasedConfigs", QVariant::UserType, "", 1,
                    NULL,"","","",
                    "Special configs per language");
}

Configurations::~Configurations()
{
    //Defined just to suppress compiler error on QScoppedPointer
}

void Configurations::init(const QStringList &_arguments)
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
                    if (this->pPrivate->Configs.value(BasePath).Type == QVariant::UserType){
                        Found = true;
                        break;
                    }

                }
                if (Found)
                    continue;
                else
                    throw exConfiguration("Configuration path <"+Key+"> is not registered");
            }
            clsConfigurationPrivate::stuConfigItem& ConfigItem  = this->pPrivate->Configs[Key];
            QVariant Value = ConfigFile.value(Key);
            if (ConfigItem.fValidator(Value, ErrorMessage) == false)
                throw exConfiguration(ErrorMessage);
            else
                ConfigItem.Value = Value;
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
                if ((KeyIter->startsWith("--") && ConfigItemIter.value().LongSwitch == "--" + *KeyIter) ||
                        ConfigItemIter.value().ShortSwitch == "-" + *KeyIter){
                    QString Value;
                    for (int i=0; i<ConfigItemIter.value().ValCount; i++){
                        Value += *KeyIter + " ";
                        KeyIter++;
                        if (KeyIter == _arguments.end())
                            throw exConfiguration("Switch: <" +*KeyIter+ "> needs at least: " +
                                                  ConfigItemIter.value().ValCount+ " arguments.");
                    }
                    if (ConfigItemIter.value().fValidator(Value.trimmed(), ErrorMessage) == false)
                        throw exConfiguration(ErrorMessage);
                    else
                        ConfigItemIter.value().Value = Value.trimmed();
                }
            }
        }else
            throw exConfiguration("invalid argument <"+*KeyIter+">");
    }

    //////////////////////////////////////////////////
    ///validate all config items
    //////////////////////////////////////////////////
    foreach (const clsConfigurationPrivate::stuConfigItem& ConfigItem, this->pPrivate->Configs.values()){
        if (ConfigItem.fValidator(ConfigItem.Value, ErrorMessage) == false)
            throw exConfiguration(ErrorMessage);
    }

    //////////////////////////////////////////////////
    /// Initialize external libraries
    //////////////////////////////////////////////////
    /// TODO: Move to engine
    ///                Text Processor
    /////////////////////////////////////////////////////
    {
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
    }

    this->pPrivate->Initialized = true;
}

QVariant Configurations::getConfig(const QString &_key, const QVariant& _default) const
{
    if (this->pPrivate->Initialized == false)
        throw exConfiguration("Configuration is not initialized yet.");

    Private::clsConfigurationPrivate::stuConfigItem Item= this->pPrivate->Configs.value(_key);
    if (Item.Value.isValid() == false)
        return Item.Value;
    else
        return _default;
}

void Configurations::addConfig(const QString&  _key,
                               QVariant::Type  _type,
                               const QVariant& _defaultValue,
                               int             _valueCount,
                               isValidConfig_t _validator,
                               const QString&  _shortSwitch,
                               const QString&  _longSwitch,
                               const QString&  _shortHelp,
                               const QString&  _longHelp)
{
    this->pPrivate->Configs.insert(_key,clsConfigurationPrivate::stuConfigItem());
    clsConfigurationPrivate::stuConfigItem& ConfigItem = this->pPrivate->Configs[_key];
    ConfigItem.fValidator = _validator;
    ConfigItem.LongHelp = _longHelp;
    ConfigItem.LongSwitch = _longSwitch;
    ConfigItem.ShortHelp = _shortHelp;
    ConfigItem.ShortSwitch = _shortSwitch;
    ConfigItem.Type = _type;
    ConfigItem.ValCount = _valueCount;
    ConfigItem.Value = _defaultValue;
}

void Configurations::save2File(const QString &_fileName)
{
    QSettings ConfigFile(_fileName,QSettings::IniFormat);
    if (!ConfigFile.isWritable())
        throw exConfiguration("unable to write configurations to: <" + _fileName+">");

    foreach(const QString& Key, this->pPrivate->Configs.keys()){
        ConfigFile.setValue(Key, this->pPrivate->Configs.value(Key).Value);
    }

    ConfigFile.sync();
}

}
}

