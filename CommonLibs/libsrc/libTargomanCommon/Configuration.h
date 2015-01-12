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
    inline size_t argCount(){return this->ArgCount;}

protected:
    QString ConfigPath;
    QString Description;
    QString ShortSwitch;
    QString ShortHelp;
    QString LongSwitch;
    size_t  ArgCount;
};

/***************************************************************************************/
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

    virtual void setFromVariant(const QVariant& _value){
        throw exTargomanMustBeImplemented("setFromVariant for "+this->ConfigPath+" Not Implemented");
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
#define _NUMERIC_CONFIGURABLE(_name, _variantType, _type, _nextType, _min, _max) \
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
_NUMERIC_CONFIGURABLE("qint8",QVariant::Int,qint8,qint16, CHAR_MIN,CHAR_MAX)
_NUMERIC_CONFIGURABLE("qint16",QVariant::Int,qint16,qint32, SHRT_MIN,SHRT_MAX)
#ifdef TARGOMAN_ARCHITECTURE_64
_NUMERIC_CONFIGURABLE("qint32",QVariant::Int,qint32,qint64, INT_MIN, INT_MAX)
_NUMERIC_CONFIGURABLE("qint64",QVariant::Int,qint64,qint64, LONG_MIN,LONG_MAX)
#else
_NUMERIC_CONFIGURABLE("qint32",QVariant::ULongLong,qint32,qint64, LONG_MIN, LONG_MAX)
_NUMERIC_CONFIGURABLE("qint64",QVariant::ULongLong,qint64,qint64, LONG_LONG_MIN,LONG_LONG_MAX)
#endif

_NUMERIC_CONFIGURABLE("quint8",QVariant::UInt,quint8,quint16, 0,CHAR_MAX)
_NUMERIC_CONFIGURABLE("quint16",QVariant::UInt,quint16,quint32, 0,USHRT_MAX)
#ifdef TARGOMAN_ARCHITECTURE_64
_NUMERIC_CONFIGURABLE("quint32",QVariant::ULongLong,quint32,quint64, 0, UINT_MAX)
_NUMERIC_CONFIGURABLE("quint64",QVariant::ULongLong,quint64,quint64, 0,ULONG_MAX)
#else
_NUMERIC_CONFIGURABLE("quint32",QVariant::ULongLong,quint32,quint64, 0, ULONG_MAX)
_NUMERIC_CONFIGURABLE("quint64",QVariant::ULongLong,quint64,quint64, 0,ULONG_LONG_MAX)
#endif

_NUMERIC_CONFIGURABLE("double",QVariant::Double,double,double, DBL_MIN, DBL_MAX)
_NUMERIC_CONFIGURABLE("float",QVariant::Double, float,double, FLT_MIN,FLT_MAX)

//////QString
template <>
    bool clsConfigurable<QString>::validate(const QVariant&, QString& ){ return true; }
template <>
    void clsConfigurable<QString>::setFromVariant(const QVariant& _value){ this->Value = _value.toString(); }



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
