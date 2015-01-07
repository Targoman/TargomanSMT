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
    inline size_t argCount(){return this->ArgCount;}

protected:
    QString Description;
    QString ShortSwitch;
    QString ShortHelp;
    QString LongSwitch;
    size_t  ArgCount;
};

/***************************************************************************************/
template <class Type_t> class clsConfigurable
{
public:
    clsConfigurable(const QString&  _configPath,
                    const QString&  _description,
                    const Type_t&   _default,
                    isValidConfig_t _crossValidator = NULL,
                    const QString&  _shortSwitch = "",
                    const QString&  _shortHelp = "",
                    const QString&  _LongSwitch = ""):
        clsConfigurableAbstract(_configPath,
                                _description,
                                _shortSwitch,
                                _shortHelp,
                                _LongSwitch){
        if (!this->validate(_default))
            throw exConfiguration("Invalid default value for: " + _configPath);
        this->Value = _default;
    }

    virtual QVariant    toVariant(){
        return QVariant(this->Value);
    }

    virtual bool        validate(const QVariant& _value, QString& _errorMessage){
        return true;
    }

    virtual bool        crossValidate(QString& _errorMessage){
        return this->fCrossValidator(this, _errorMessage);
    }

public:
    Type_t  Value;
    isValidConfig_t fCrossValidator;
};

/***************************************************************************************/
#define VALIDATE_NUMERIC(_name, _variant, _variantType, _type, _max, _min, _errorMessage) \
    if (_value.canConvert(_variantType) == false) {\
        _errorMessage = "Unable to convert" + _value.toString() + " to numeric."; \
        return false;\
    }else if (_variant.value<_type>() > _max || _variant.value<_type>() < _min ){ \
        _errorMessage = QString("%1 values must be between (%2 : %3)").arg(_name).arg(_min).arg(_max); \
        return false; \
    }else return true;

template <>
class clsConfigurable <qint8>{
public:
    bool validate(const QVariant& _value, QString& _errorMessage){
        VALIDATE_NUMERIC("qint8",_value, QVariant::Int, qint8, 127, -128, _errorMessage)
    }
};

/***************************************************************************************/
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
