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

#ifndef TARGOMAN_COMMON_CONFIGURATION_TMPLCONFIGURABLE_HPP
#define TARGOMAN_COMMON_CONFIGURATION_TMPLCONFIGURABLE_HPP

#include <functional>
#include "libTargomanCommon/Configuration/intfConfigurable.hpp"
#include "libTargomanCommon/Configuration/ConfigManager.h"
#include "libTargomanCommon/Macros.h"
#include "libTargomanCommon/FastOperations.hpp"

namespace Targoman {
namespace Common {
namespace Configuration {

/// this is a lambda function variable that its arguments are intfConfigurable and a QString and always returns true.
static std::function<bool(const intfConfigurable& _item,
                          QString& _errorMessage)> ReturnTrueCrossValidator = [] (const intfConfigurable&, QString& ) {
    return true;};

/**
 * @brief The clsConfigurable template is used to store and validate different configurable items
 */
template <class itmplType_t, bool ... _rest> class tmplConfigurable : public intfConfigurable
{
public:
    tmplConfigurable(const QString&  _configPath,
                    const QString&  _description,
                    const QVariant& _default = QVariant(),
                    const std::function< bool(const intfConfigurable& _item,
                                              QString& _errorMessage) >& _crossValidator = ReturnTrueCrossValidator,
                    const QString&  _shortSwitch = "",
                    const QString&  _shortHelp = "",
                    const QString&  _LongSwitch = "",
                     enuConfigSource::Type _configSources =
                        (enuConfigSource::Type)(
                            enuConfigSource::Arg  |
                            enuConfigSource::File |
                            enuConfigSource::Net ),
                     bool _remoteView = true) :
        intfConfigurable(enuConfigType::Normal,
                         _configPath,
                        _description,
                        _shortSwitch,
                        _shortHelp,
                        _LongSwitch,
                        _configSources,
                        _remoteView)
    {
        try{
            QString ErrorMessage;
            if (!this->validate(_default, ErrorMessage)){
                throw exTargomanInitialization("Invalid default value for: " + _configPath + ": " + ErrorMessage);
            }
            this->setFromVariant(_default);
            if (this->ShortHelp.size()){
                this->Description.append(" (default= " + this->toVariant().toString() + " )");
            }
            this->CrossValidator = _crossValidator;
        }catch(exTargomanBase &e){
            TargomanError(e.what());
            throw;
        }
    }

    /**
     * @brief tmplConfigurable Copy constructor of this class.
     */


    tmplConfigurable(const tmplConfigurable<itmplType_t>& _other) :
        intfConfigurable(_other){
        this->Value = _other.Value;
        this->CrossValidator = _other.CrossValidator;
    }


    tmplConfigurable& operator = (const tmplConfigurable<itmplType_t>& _other){
        this->ConfigPath = _other.ConfigPath;
        this->Description = _other.Description;
        this->ShortSwitch = _other.ShortSwitch;
        this->ShortHelp = _other.ShortHelp;
        this->LongSwitch = _other.LongSwitch;
        this->Value = _other.Value;
        this->CrossValidator = _other.CrossValidator;
        return *this;
    }

    /**
     * @brief This function will be overloaded for every type (such as int, float ,...).
     * This function converts input value from QVariant to a specific type based on overloaded implementation.
     */
    virtual inline void setFromVariant(const QVariant& _value){
        throw exTargomanMustBeImplemented("setFromVariant for "+this->ConfigPath+" Not Implemented");
    }
    /**
     * @brief convert #value variable to QVariant type.
     */
    virtual inline QVariant    toVariant() const{
        return QVariant(this->value());
    }
    /**
     * @brief This function will be overloaded for every type (such as int, float ,...)
     * This function validate value of input value.
     */
    virtual inline bool        validate(const QVariant& _value, QString& _errorMessage) const{
        return true;
    }
    /**
     * @brief if cross validator function is prepared that function will be called, else returns true.
     */
    virtual inline bool        crossValidate(QString& _errorMessage) const{
        return this->CrossValidator(*this, _errorMessage);
    }

    inline itmplType_t  value() const{ /*TODO Thread Safe*/ return this->Value;}

    virtual QString typeString() const{
        return getTypeStr(this->Value);
    }

    virtual QString validValues() const{
        return "Valid Values not implemented yet";
    }

private:
    itmplType_t  Value;
    std::function<bool(const intfConfigurable& _item, QString& _errorMessage)> CrossValidator;

};

/***************************************************************************************/
/**
 * @def SPECIAL_CONFIGURABLE type specific validate and setFromVariant functions signiture.
 */
#define SPECIAL_CONFIGURABLE(_type) \
    template <> bool Targoman::Common::Configuration::tmplConfigurable<_type>::validate(const QVariant& _value, QString& _errorMessage) const ;\
    template <> void Targoman::Common::Configuration::tmplConfigurable<_type>::setFromVariant(const QVariant& _value)

#define ENUM_CONFIGURABLE(_enum) \
    template <> bool Targoman::Common::Configuration::tmplConfigurable<_enum::Type>::validate(const QVariant& _value, QString& _errorMessage) const ;\
    template <> void Targoman::Common::Configuration::tmplConfigurable<_enum::Type>::setFromVariant(const QVariant& _value);\
    template <> QVariant Targoman::Common::Configuration::tmplConfigurable<_enum::Type>::toVariant() const

#define ENUM_CONFIGURABLE_IMPL(_enum) \
template <>\
bool tmplConfigurable<_enum::Type>::validate(const QVariant& _value, QString& _errorMessage) const{\
    if(_enum::toEnum(_value.toString().toLatin1().constData()) != _enum::Unknown)  return true; \
    _errorMessage = "Unrecognized option: " + _value.toString() + " for: " + this->configPath(); \
    return false; \
} \
template <> \
void tmplConfigurable<_enum::Type>::setFromVariant(const QVariant& _value){ \
    QString ErrorMessage; \
    if (this->validate(_value, ErrorMessage)) this->Value = \
            _enum::toEnum(_value.toString().toLatin1().constData()); \
    else throw exConfiguration(this->ConfigPath + ": " + ErrorMessage); \
} \
template <>\
QVariant tmplConfigurable<_enum::Type>::toVariant() const{ \
    return _enum::toStr(this->Value); \
}

/***************************************************************************************/
SPECIAL_CONFIGURABLE(qint8);
SPECIAL_CONFIGURABLE(qint16);
SPECIAL_CONFIGURABLE(qint32);
SPECIAL_CONFIGURABLE(qint64);
SPECIAL_CONFIGURABLE(quint8);
SPECIAL_CONFIGURABLE(quint16);
SPECIAL_CONFIGURABLE(quint32);
SPECIAL_CONFIGURABLE(quint64);
SPECIAL_CONFIGURABLE(double);
SPECIAL_CONFIGURABLE(float);
SPECIAL_CONFIGURABLE(QString);
SPECIAL_CONFIGURABLE(QStringList);
template <> QVariant Targoman::Common::Configuration::tmplConfigurable<QStringList>::toVariant() const;

SPECIAL_CONFIGURABLE(bool);
SPECIAL_CONFIGURABLE(QRegExp MACRO_SAFE_COMMA false); /* Used on normal regex matching */
SPECIAL_CONFIGURABLE(QRegExp MACRO_SAFE_COMMA true); /* Used on wildcard matching */

//TODO add Special QFile and QDir
}
}
}
#endif // TARGOMAN_COMMON_CONFIGURATION_TMPLCONFIGURABLE_HPP
