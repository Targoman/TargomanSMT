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

#include "tmplConfigurable.h"
#include "libTargomanCommon/CmdIO.h"
#include "../Private/clsConfigManager_p.h"

namespace Targoman {
namespace Common {
namespace Configuration {

/***************************************************************************************/
#define _NUMERIC_CONFIGURABLE_IMPL(_name, _variantType, _type, _nextType, _min, _max) \
    template <> \
    bool tmplConfigurable<_type>::validate(const QVariant& _value, QString& _errorMessage) const{ \
        if (_value.canConvert(_variantType) == false) {\
            _errorMessage = "Unable to convert " + _value.toString() + " to numeric."; \
            return false;\
        }else if (_value.value<_nextType>() > _max || _value.value<_nextType>() < _min ){ \
            _errorMessage = QString("%1 values must be between (%2 : %3)").arg(_name).arg(_min).arg(_max); \
            return false; \
        }else return true; \
    } \
    template <> \
    void tmplConfigurable<_type>::setFromVariant(const QVariant& _value) { \
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
_NUMERIC_CONFIGURABLE_IMPL("qint32",QVariant::LongLong,qint32,qint64, LONG_MIN, LONG_MAX);
_NUMERIC_CONFIGURABLE_IMPL("qint64",QVariant::LongLong,qint64,qint64, LONG_LONG_MIN,LONG_LONG_MAX);
#endif

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wtype-limits"
_NUMERIC_CONFIGURABLE_IMPL("quint8",QVariant::UInt,quint8,quint16, 0,UCHAR_MAX)
_NUMERIC_CONFIGURABLE_IMPL("quint16",QVariant::UInt,quint16,quint32, 0,USHRT_MAX)
#ifdef TARGOMAN_ARCHITECTURE_64
_NUMERIC_CONFIGURABLE_IMPL("quint32",QVariant::ULongLong,quint32,quint64, 0, UINT_MAX)
_NUMERIC_CONFIGURABLE_IMPL("quint64",QVariant::ULongLong,quint64,quint64, 0,ULONG_MAX)
#else
_NUMERIC_CONFIGURABLE_IMPL("quint32",QVariant::ULongLong,quint32,quint64, 0, ULONG_MAX)
_NUMERIC_CONFIGURABLE_IMPL("quint64",QVariant::ULongLong,quint64,quint64, 0,ULONG_LONG_MAX)
#endif

_NUMERIC_CONFIGURABLE_IMPL("double",QVariant::Double,double,double, -DBL_MAX, DBL_MAX)
_NUMERIC_CONFIGURABLE_IMPL("float",QVariant::Double, float,double, -FLT_MAX,FLT_MAX)
#pragma GCC diagnostic pop

//////QString
template <>
bool tmplConfigurable<QString>::validate(const QVariant&, QString& )const { return true; }
template <>
void tmplConfigurable<QString>::setFromVariant(const QVariant& _value){ this->Value = _value.toString(); }

//////FilePath_t
template <>
bool tmplConfigurable<FilePath_t>::validate(const QVariant&, QString& )const { return true; }
template <>
void tmplConfigurable<FilePath_t>::setFromVariant(const QVariant& _value){
    this->Value = ConfigManager::instance().getAbsolutePath(_value.toString());
}

//////QStringList
template <>
bool tmplConfigurable<QStringList>::validate(const QVariant& , QString&  )const {
    return true;
}

template <>
void tmplConfigurable<QStringList>::setFromVariant(const QVariant& _value){
    QString ErrorMessage;
    if (this->validate(_value, ErrorMessage)){
        if (_value.canConvert(QVariant::StringList))
            this->Value = _value.toStringList();
        else
            this->Value = _value.toString().split(',');
    }
    else
        throw exConfiguration(this->ConfigPath + ": " + ErrorMessage);
}
template <>
QVariant tmplConfigurable<QStringList>::toVariant() const{
    return this->Value.join(",");
}


//////bool
template <>
bool tmplConfigurable<bool>::validate(const QVariant& _value, QString& _errorMessage) const{
    if (_value.canConvert(QVariant::Bool) == false) {
        _errorMessage = "Unable to convert " + _value.toString() + " to bool.";
        return false;
    }else
        return true;
}
template <>
void tmplConfigurable<bool>::setFromVariant(const QVariant& _value){
    QString ErrorMessage;
    if (this->validate(_value, ErrorMessage)) this->Value = _value.value<bool>();
    else throw exConfiguration(this->ConfigPath + ": " + ErrorMessage);
}

//////QRegExp
template <>
bool tmplConfigurable<QRegExp, false>::validate(const QVariant& _value, QString& _errorMessage) const{
    if (_value.toString().size()){
        QRegExp TempRegex(_value.toString(), Qt::CaseSensitive, QRegExp::RegExp);
        if (TempRegex.isValid() == false){
            _errorMessage = "Invalid RegEx pattern: " + TempRegex.errorString();
            return false;
        }
    }
    return true;
}
template <>
void tmplConfigurable<QRegExp, false>::setFromVariant(const QVariant& _value){
    QString ErrorMessage;
    if (this->validate(_value, ErrorMessage)) this->Value = QRegExp(_value.value<QString>(),
                                                                    Qt::CaseSensitive,
                                                                    QRegExp::RegExp);
    else throw exConfiguration(this->ConfigPath + ": " + ErrorMessage);
}

template <>
QVariant tmplConfigurable<QRegExp, false>::toVariant() const{
    return this->Value.pattern();
}

//////QRegExp when used as wildcard
template <>
bool tmplConfigurable<QRegExp, true>::validate(const QVariant& _value, QString& _errorMessage) const{
    if (_value.toString().size()){
        QRegExp TempRegex(_value.toString(), Qt::CaseSensitive, QRegExp::WildcardUnix);
        if (TempRegex.isValid() == false){
            _errorMessage = "Invalid wildcard pattern: " + TempRegex.errorString();
            return false;
        }
    }
    return true;
}
template <>
void tmplConfigurable<QRegExp, true>::setFromVariant(const QVariant& _value){
    QString ErrorMessage;
    if (this->validate(_value, ErrorMessage))this->Value = QRegExp(_value.value<QString>(),
                                                                   Qt::CaseSensitive,
                                                                   QRegExp::WildcardUnix);
    else throw exConfiguration(this->ConfigPath + ": " + ErrorMessage);
}

template <>
QVariant tmplConfigurable<QRegExp, true>::toVariant() const{
    return this->Value.pattern();
}

}
}
}
