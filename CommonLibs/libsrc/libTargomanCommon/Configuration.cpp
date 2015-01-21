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

void Configuration::init(const QStringList &_arguments, const QString& _license)
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

    this->pPrivate->Initialized = true;
}

void Configuration::save2File(const QString &_fileName, bool _backup)
{
    Q_UNUSED(_fileName)
    Q_UNUSED(_backup)
    //TODO implement me
}

void Configuration::addConfig(const QString _path, intfConfigurable *_item)
{
    if (this->pPrivate->Configs.contains(_path))
        throw exConfiguration("Duplicate path key: " + _path);
    this->pPrivate->Configs.insert(_path, _item);
}

QVariant Configuration::getConfig(const QString &_path, const QVariant& _default)
{
    if (this->pPrivate->Initialized == false)
        throw exConfiguration("Configuration is not initialized yet.");

    intfConfigurable* Item= this->pPrivate->Configs.value(_path);
    if (Item->toVariant().isValid() == false)
        return Item->toVariant();
    else
        return _default;
}
/***********************************************************************************************/
void Private::clsConfigurationPrivate::printHelp(const QString& _license)
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
    this->Description = _description;
    this->ShortSwitch = _shortSwitch;
    this->LongSwitch = _longSwitch;
    this->ShortHelp = _shortHelp;
    this->ConfigPath = _configPath;
    this->ArgCount = this->ShortHelp.split(" ").size();

    Configuration::instance().addConfig(_configPath, this);
}

/***************************************************************************************/
#define _NUMERIC_CONFIGURABLE_IMPL(_name, _variantType, _type, _nextType, _min, _max) \
    template <> \
    bool clsConfigurable<_type>::validate(const QVariant& _value, QString& _errorMessage) const{ \
    if (_value.canConvert(_variantType) == false) {\
    _errorMessage = "Unable to convert" + _value.toString() + " to numeric."; \
    return false;\
}else if (_value.value<_nextType>() > _max || _value.value<_nextType>() < _min ){ \
    _errorMessage = QString("%1 values must be between (%2 : %3)").arg(_name).arg(_min).arg(_max); \
    return false; \
}else return true; \
} \
    template <> \
    void clsConfigurable<_type>::setFromVariant(const QVariant& _value) { \
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
_NUMERIC_CONFIGURABLE_IMPL("qint32",QVariant::LongLong,qint32,qint64, LONG_MIN, LONG_MAX)
_NUMERIC_CONFIGURABLE_IMPL("qint64",QVariant::LongLong,qint64,qint64, LONG_LONG_MIN,LONG_LONG_MAX)
#endif

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wtype-limits"
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
#pragma GCC diagnostic pop

//////QString
template <>
bool clsConfigurable<QString>::validate(const QVariant&, QString& )const { return true; }
template <>
void clsConfigurable<QString>::setFromVariant(const QVariant& _value){ this->Value = _value.toString(); }

//////bool
template <>
bool clsConfigurable<bool>::validate(const QVariant& _value, QString& _errorMessage) const{
    if (_value.canConvert(QVariant::Bool) == false) {
        _errorMessage = "Unable to convert" + _value.toString() + " to bool.";
        return false;
    }else
        return true;
}
template <>
void clsConfigurable<bool>::setFromVariant(const QVariant& _value){
    QString ErrorMessage;
    if (this->validate(_value, ErrorMessage)) this->Value = _value.value<bool>();
    else throw exConfiguration(this->ConfigPath + ": " + ErrorMessage);
}

//////QList<quint8>
template <>
bool clsConfigurable<QList<quint8> >::validate(const QVariant&, QString& )const {
    Q_ASSERT_X (0, "QList<QVariant>", "Check Validation");return true;
}
template <>
void clsConfigurable<QList<quint8> >::setFromVariant(const QVariant& _value){
    QString ErrorMessage;
    if (this->validate(_value, ErrorMessage)) {
        foreach (const QString& Value, _value.toString().split(" ")){

            this->Value.append(Value.toUInt());
        }
    }
    else throw exConfiguration(this->ConfigPath + ": " + ErrorMessage);
}
template <>
QVariant    clsConfigurable<QList<quint8> >::toVariant() const{
    Q_ASSERT_X (0, "QList<QVariant>", "toVariant");
    return QVariant();
}



/***************************************************************************************/
Validators::clsPathValidator::clsPathValidator(PathAccess::Options _requiredAccess):
    RequiredAccess(_requiredAccess)
{}

bool Validators::clsPathValidator::validate(const intfConfigurable &_item, QString &_errorMessage)
{
    QString Path = _item.toVariant().toString();
    QFileInfo PathInfo(Path);

    if (this->RequiredAccess.testFlag(PathAccess::Dir) && PathInfo.isDir() == false){
        _errorMessage = _item.configPath() + ": <"+Path+"> must be a directory";
        return false;
    }else if (this->RequiredAccess.testFlag(PathAccess::File) && PathInfo.isFile() == false){
        _errorMessage = _item.configPath() + ": <"+Path+"> must be a file";
        return false;
    }
    if (this->RequiredAccess.testFlag(PathAccess::Executable) && PathInfo.isExecutable() == false){
        _errorMessage = _item.configPath() + ": <"+Path+"> must be executable";
        return false;
    }

    if (this->RequiredAccess.testFlag(PathAccess::Readable) && PathInfo.isReadable() == false){
        _errorMessage = _item.configPath() + ": Unable to open <"+Path+"> for READING";
        return false;
    }
    if (this->RequiredAccess.testFlag(PathAccess::Writeatble) && PathInfo.isWritable() == false){
        _errorMessage = _item.configPath() + ": Unable to open <"+Path+"> for WRITING";
        return false;
    }
    _errorMessage.clear();
    return true;
}
///////////////////////////////////////////////////////////////////////
Validators::clsIntValidator::clsIntValidator(qint64 _min, qint64 _max):
    Max(_max),Min(_min)
{}

bool Validators::clsIntValidator::validate(const intfConfigurable &_item, QString &_errorMessage)
{
    qint64 Number = _item.toVariant().toLongLong();
    if (Number < this->Min || Number > this->Max){
        _errorMessage = _item.configPath() + QString(": must be between %1 - %2").arg(this->Min).arg(this->Max);
        return false;
    }
    _errorMessage.clear();
    return true;
}

///////////////////////////////////////////////////////////////////////
Validators::clsUIntValidator::clsUIntValidator(quint64 _min, quint64 _max):
    Max(_max),Min(_min)
{}

bool Validators::clsUIntValidator::validate(const intfConfigurable &_item, QString &_errorMessage)
{
    quint64 Number = _item.toVariant().toULongLong();
    if (Number < this->Min || Number > this->Max){
        _errorMessage = _item.configPath() + QString(": must be between %1 - %2").arg(this->Min).arg(this->Max);
        return false;
    }
    _errorMessage.clear();
    return true;
}

///////////////////////////////////////////////////////////////////////
Validators::clsDoubleValidator::clsDoubleValidator(double _min, double _max):
    Max(_max),Min(_min)
{}

bool Validators::clsDoubleValidator::validate(const intfConfigurable &_item, QString &_errorMessage)
{
    double Number = _item.toVariant().toDouble();
    if (Number < this->Min || Number > this->Max){
        _errorMessage = _item.configPath() + QString(": must be between %1 - %2").arg(this->Min).arg(this->Max);
        return false;
    }
    _errorMessage.clear();
    return true;
}

}
}
