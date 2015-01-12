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

#ifndef TARGOMAN_COMMON_CONFIGURATION_H
#define TARGOMAN_COMMON_CONFIGURATION_H

#include "libTargomanCommon/exTargomanBase.h"
#include <cfloat>

namespace Targoman {
namespace Common {

namespace Private {
class clsConfigurationPrivate;
}

TARGOMAN_ADD_EXCEPTION_HANDLER(exConfiguration, exTargomanBase);


class clsConfigurableAbstract;

typedef bool (*isValidConfig_t)(clsConfigurableAbstract* _item, QString& _errorMessageHolder);

/***************************************************************************************/
class clsConfigurableAbstract
{
public:
    clsConfigurableAbstract(const QString&  _configPath,
                            const QString&  _description,
                            const QString&  _shortSwitch = "",
                            const QString&  _shortHelp = "",
                            const QString&  _longSwitch = "");

    virtual void        setFromVariant(const QVariant& _value) = 0;
    virtual QVariant    toVariant() = 0;
    virtual bool        validate(const QVariant& _value, QString& _errorMessage) = 0;
    virtual bool        crossValidate(QString& _errorMessage) = 0;

    inline const QString& description(){return this->Description;}
    inline const QString& shortSwitch(){return this->ShortSwitch;}
    inline const QString& shortHelp(){return this->ShortHelp;}
    inline const QString& longSwitch(){return this->LongSwitch;}
    inline qint8 argCount(){return this->ArgCount;}
    inline const QString& configPath(){return this->ConfigPath;}

protected:
    QString ConfigPath;
    QString Description;
    QString ShortSwitch;
    QString ShortHelp;
    QString LongSwitch;
    qint8   ArgCount;
};

/***************************************************************************************/
/**
 * @brief The clsFileBasedConfig class is used when there are more optional configs stored in configuration file
 * this optional configs will not be stored and monitored by configuration manager.
 */
class clsFileBasedConfig : public clsConfigurableAbstract{
public:
    clsFileBasedConfig(const QString&  _configPath) :
        clsConfigurableAbstract(_configPath, "OPTIONAL_CONFIGS_IN_FILE"){
        this->ArgCount = -1;
    }

    virtual inline void setFromVariant(const QVariant& ){
        throw exTargomanNotImplemented(this->ConfigPath + " is Abstract");
    }

    virtual inline QVariant    toVariant(){
        throw exTargomanNotImplemented(this->ConfigPath + " is Abstract");
    }

    virtual inline bool        validate(const QVariant&, QString&){
        return true;
    }

    virtual inline bool        crossValidate(QString& ){
        return true;
    }
};

/***************************************************************************************/
/**
 * @brief The clsConfigurable template is used to store and validate different configurable items
 */
template <class Type_t> class clsConfigurable : public clsConfigurableAbstract
{
public:
    clsConfigurable(const QString&  _configPath,
                    const QString&  _description,
                    const QVariant& _default,
                    isValidConfig_t _crossValidator = NULL,
                    const QString&  _shortSwitch = "",
                    const QString&  _shortHelp = "",
                    const QString&  _LongSwitch = ""):
        clsConfigurableAbstract(_configPath,
                                _description,
                                _shortSwitch,
                                _shortHelp,
                                _LongSwitch){
        QString ErrorMessage;
        if (!this->validate(_default, ErrorMessage))
            throw exTargomanInitialization("Invalid default value for: " + _configPath + ": " + ErrorMessage);
        this->setFromVariant(_default);
        this->fCrossValidator = _crossValidator;
    }

    virtual inline void setFromVariant(const QVariant& _value){
        throw exTargomanMustBeImplemented("setFromVariant for "+this->ConfigPath+" Not Implemented");
    }

    virtual inline QVariant    toVariant(){
        return QVariant(this->Value);
    }

    virtual inline bool        validate(const QVariant& _value, QString& _errorMessage){
        return true;
    }

    virtual inline bool        crossValidate(QString& _errorMessage){
        return this->fCrossValidator(this, _errorMessage);
    }

    inline Type_t  value(){return this->Value;}

private:
    Type_t  Value;
    isValidConfig_t fCrossValidator;
};

/***************************************************************************************/
#define _SPECIAL_CONFIGURABLE(_type) \
    template <> bool clsConfigurable<_type>::validate(const QVariant& _value, QString& _errorMessage);\
    template <> void clsConfigurable<_type>::setFromVariant(const QVariant& _value);

/***************************************************************************************/
_SPECIAL_CONFIGURABLE(qint8)
_SPECIAL_CONFIGURABLE(qint16)
_SPECIAL_CONFIGURABLE(qint32)
_SPECIAL_CONFIGURABLE(qint64)
_SPECIAL_CONFIGURABLE(quint8)
_SPECIAL_CONFIGURABLE(quint16)
_SPECIAL_CONFIGURABLE(quint32)
_SPECIAL_CONFIGURABLE(quint64)
_SPECIAL_CONFIGURABLE(double)
_SPECIAL_CONFIGURABLE(float)
_SPECIAL_CONFIGURABLE(QString)

/***************************************************************************************/
/**
 * @brief The Configuration class is the base configuration manager class.
 * Currently it will just manage Arguments and config file
 */
class Configuration
{
public:
    ~Configuration();
    static inline Configuration& instance(){
        return *(Q_LIKELY(Instance) ? Instance : (Instance = new Configuration));}

    void init(const QStringList &_arguments);
    void save2File(const QString&  _fileName, bool _backup);
    void addConfig(const QString _path, clsConfigurableAbstract* _item);
    QVariant getConfig(const QString& _path, const QVariant &_default = QVariant());

private:
    Configuration();
    Q_DISABLE_COPY(Configuration)

private:
    static Configuration* Instance;
    QScopedPointer<Private::clsConfigurationPrivate> pPrivate;
};

}
}
#endif // TARGOMAN_COMMON_CONFIGURATION_H
