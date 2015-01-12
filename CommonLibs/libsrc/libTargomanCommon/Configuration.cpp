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

#define _NUMERIC_CONFIGURABLE_IMPL(_name, _variantType, _type, _nextType, _min, _max) \
    template <> \
        bool clsConfigurable<_type>::validate(const QVariant& _value, QString& _errorMessage){ \
            if (_value.canConvert(_variantType) == false) {\
                _errorMessage = "Unable to convert" + _value.toString() + " to numeric."; \
                return false;\
            }else if (_value.value<_nextType>() > _max || _value.value<_nextType>() < _min ){ \
                _errorMessage = QString("%1 values must be between (%2 : %3)").arg(_name).arg(_min).arg(_max); \
                return false; \
            }else return true; \
        } \
    template <> \
        void clsConfigurable<_type>::setFromVariant(const QVariant& _value){ \
            QString ErrorMessage; \
            if (this->validate(_value, ErrorMessage)) this->Value = _value.value<_type>(); \
            else throw exConfiguration(this->ConfigPath + ": " + ErrorMessage);\
        }

/***************************************************************************************/
_NUMERIC_CONFIGURABLE_IMPL("qint8",QVariant::Int,qint8,qint16, CHAR_MIN,CHAR_MAX)
_NUMERIC_CONFIGURABLE_IMPL("qint16",QVariant::Int,qint16,qint32, SHRT_MIN,SHRT_MAX)
#ifdef TARGOMAN_ARCHITECTURE_64
_NUMERIC_CONFIGURABLE_IMPL("qint32",QVariant::Int,qint32,qint64, INT_MIN, INT_MAX)
_NUMERIC_CONFIGURABLE_IMPL("qint64",QVariant::Int,qint64,qint64, LONG_MIN,LONG_MAX)
#else
_NUMERIC_CONFIGURABLE_IMPL("qint32",QVariant::ULongLong,qint32,qint64, LONG_MIN, LONG_MAX)
_NUMERIC_CONFIGURABLE_IMPL("qint64",QVariant::ULongLong,qint64,qint64, LONG_LONG_MIN,LONG_LONG_MAX)
#endif

_NUMERIC_CONFIGURABLE_IMPL("quint8",QVariant::UInt,quint8,quint16, 0,CHAR_MAX)
_NUMERIC_CONFIGURABLE_IMPL("quint16",QVariant::UInt,quint16,quint32, 0,USHRT_MAX)
#ifdef TARGOMAN_ARCHITECTURE_64
_NUMERIC_CONFIGURABLE_IMPL("quint32",QVariant::ULongLong,quint32,quint64, 0, UINT_MAX)
_NUMERIC_CONFIGURABLE_IMPL("quint64",QVariant::ULongLong,quint64,quint64, 0,ULONG_MAX)
#else
_NUMERIC_CONFIGURABLE_IMPL("quint32",QVariant::ULongLong,quint32,quint64, 0, ULONG_MAX)
_NUMERIC_CONFIGURABLE_IMPL("quint64",QVariant::ULongLong,quint64,quint64, 0,ULONG_LONG_MAX)
#endif

_NUMERIC_CONFIGURABLE_IMPL("double",QVariant::Double,double,double, DBL_MIN, DBL_MAX)
_NUMERIC_CONFIGURABLE_IMPL("float",QVariant::Double, float,double, FLT_MIN,FLT_MAX)

//////QString
template <>
    bool clsConfigurable<QString>::validate(const QVariant&, QString& ){ return true; }
template <>
    void clsConfigurable<QString>::setFromVariant(const QVariant& _value){ this->Value = _value.toString(); }


}
}
