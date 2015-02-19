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
            _errorMessage = "Unable to convert" + _value.toString() + " to numeric."; \
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

_NUMERIC_CONFIGURABLE_IMPL("double",QVariant::Double,double,double, -DBL_MAX, DBL_MAX)
_NUMERIC_CONFIGURABLE_IMPL("float",QVariant::Double, float,double, -FLT_MAX,FLT_MAX)
#pragma GCC diagnostic pop

//////QString
template <>
bool tmplConfigurable<QString>::validate(const QVariant&, QString& )const { return true; }
template <>
void tmplConfigurable<QString>::setFromVariant(const QVariant& _value){ this->Value = _value.toString(); }

//////QStringList
template <>
bool tmplConfigurable<QStringList>::validate(const QVariant& _value, QString& _errorMessage )const {
    if (_value.canConvert(QVariant::List) == false) {
        _errorMessage = "Unable to convert" + _value.toString() + " to QStringList.";
        return false;
    }else
        return true;
}

template <>
void tmplConfigurable<QStringList>::setFromVariant(const QVariant& _value){
    QString ErrorMessage;
    if (this->validate(_value, ErrorMessage))
        this->Value = _value.value<QStringList>();
    else
        throw exConfiguration(this->ConfigPath + ": " + ErrorMessage);
}

//////bool
template <>
bool tmplConfigurable<bool>::validate(const QVariant& _value, QString& _errorMessage) const{
    if (_value.canConvert(QVariant::Bool) == false) {
        _errorMessage = "Unable to convert" + _value.toString() + " to bool.";
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
    if (_value.canConvert(QVariant::RegExp) == false) {
        _errorMessage = "Unable to convert" + _value.toString() + " to regex.";
        return false;
    }else{
        QRegExp TempRegex(_value.toString());
        if (TempRegex.isValid() == false){
            _errorMessage = "Invalid regex pattern: " + TempRegex.errorString();
            return false;
        }else
        return true;
    }

}
template <>
void tmplConfigurable<QRegExp, false>::setFromVariant(const QVariant& _value){
    QString ErrorMessage;
    if (this->validate(_value, ErrorMessage))this->Value = _value.value<QRegExp>();
    else throw exConfiguration(this->ConfigPath + ": " + ErrorMessage);
}

//////QRegExp when used as wildcard
template <>
bool tmplConfigurable<QRegExp, true>::validate(const QVariant& _value, QString& _errorMessage) const{
    if (_value.canConvert(QVariant::RegExp) == false) {
        _errorMessage = "Unable to convert" + _value.toString() + " to regex.";
        return false;
    }else{
        QRegExp TempRegex(_value.toString(), Qt::CaseSensitive);
        if (TempRegex.isValid() == false){
            _errorMessage = "Invalid regex pattern: " + TempRegex.errorString();
            return false;
        }else
        return true;
    }

}
template <>
void tmplConfigurable<QRegExp, true>::setFromVariant(const QVariant& _value){
    QString ErrorMessage;
    if (this->validate(_value, ErrorMessage))this->Value = _value.value<QRegExp>();
    else throw exConfiguration(this->ConfigPath + ": " + ErrorMessage);
}




}
}
}
