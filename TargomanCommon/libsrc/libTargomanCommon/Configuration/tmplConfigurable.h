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

#ifndef TARGOMAN_COMMON_CONFIGURATION_TMPLCONFIGURABLE_HPP
#define TARGOMAN_COMMON_CONFIGURATION_TMPLCONFIGURABLE_HPP

#include <functional>
#include "libTargomanCommon/Configuration/intfConfigurable.hpp"
#include "libTargomanCommon/Configuration/ConfigManager.h"
#include "libTargomanCommon/Macros.h"
#include "libTargomanCommon/FastOperations.hpp"
#include "libTargomanCommon/Helpers.hpp"

namespace Targoman {
namespace Common {
namespace Configuration {

/// @brief A predefined lambda function used which always returns true used when there
/// is no further crossvalidation condition.
static std::function<bool(const intfConfigurable& _item,
                          QString& _errorMessage)> ReturnTrueCrossValidator =
        [] (const intfConfigurable&, QString& ) {return true;};

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
                     enuConfigSource::Type _configSources = (enuConfigSource::Type)(
                enuConfigSource::Arg  |
                enuConfigSource::File |
                enuConfigSource::Net ),
                     bool _remoteView = true,
                     const std::function< void(const intfConfigurable& _item) >& _finalizer = VoidFinalizer
                     ) :
        intfConfigurable(enuConfigType::Normal,
                         _configPath,
                         _description,
                         _shortSwitch,
                         _shortHelp,
                         _LongSwitch,
                         _configSources,
                         _remoteView,
                         _finalizer)
    {
        try{
            QString ErrorMessage;
            if (!this->validate(_default, ErrorMessage)){
                throw exTargomanInitialization("Invalid default value for: " + _configPath + ": " + ErrorMessage);
            }
            this->setFromVariant(_default);
            if (this->ShortHelp.size()){
                this->Description.append(" ( default= '" + this->toVariant().toString() + "' )");
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
    namespace Targoman { namespace Common { namespace Configuration { \
    template <> bool Targoman::Common::Configuration::tmplConfigurable<_enum::Type>::validate(const QVariant& _value, QString& _errorMessage) const ;\
    template <> void Targoman::Common::Configuration::tmplConfigurable<_enum::Type>::setFromVariant(const QVariant& _value);\
    template <> QVariant Targoman::Common::Configuration::tmplConfigurable<_enum::Type>::toVariant() const; \
}}}

#define ENUM_CONFIGURABLE_IMPL(_enum) \
    namespace Targoman { namespace Common { namespace Configuration { \
    template <> \
    bool tmplConfigurable<_enum::Type>::validate(const QVariant& _value, QString& _errorMessage) const{ \
    QString Option = _value.toString(); \
    if (Option.size() && Option.at(0).isDigit()){ \
    if (Option.toInt() >=0 && Option.toInt() < _enum::getCount()) return true; \
}else if(_enum::toEnum(Option.toLatin1().constData()) != _enum::Unknown)  return true; \
    _errorMessage = "Unrecognized option: " + _value.toString() + " for: " + this->configPath(); \
    return false; \
} \
    template <> \
    void tmplConfigurable<_enum::Type>::setFromVariant(const QVariant& _value){ \
    QString ErrorMessage; \
    if (this->validate(_value, ErrorMessage)) this->Value = \
    (_value.toString().at(0).isDigit() ? \
    (_enum::Type)_value.toInt() : \
    _enum::toEnum(_value.toString().toLatin1().constData())); \
    else throw exConfiguration(this->ConfigPath + ": " + ErrorMessage); \
} \
    template <> \
    QVariant tmplConfigurable<_enum::Type>::toVariant() const{ \
    return _enum::toStr(this->Value); \
}}}}

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
SPECIAL_CONFIGURABLE(FilePath_t);
SPECIAL_CONFIGURABLE(QStringList);
template <> QVariant Targoman::Common::Configuration::tmplConfigurable<QStringList>::toVariant() const;

SPECIAL_CONFIGURABLE(bool);
SPECIAL_CONFIGURABLE(QRegExp MACRO_SAFE_COMMA false); /* Used on normal regex matching */template <> QVariant Targoman::Common::Configuration::tmplConfigurable<QRegExp, false>::toVariant() const; \
template <> QVariant Targoman::Common::Configuration::tmplConfigurable<QRegExp, false>::toVariant() const; \
SPECIAL_CONFIGURABLE(QRegExp MACRO_SAFE_COMMA true); /* Used on wildcard matching */
template <> QVariant Targoman::Common::Configuration::tmplConfigurable<QRegExp, true>::toVariant() const; \

}
}
}


#endif // TARGOMAN_COMMON_CONFIGURATION_TMPLCONFIGURABLE_HPP
