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
 @author Behrooz Vedadian <vedadian@gmail.com>
 */

#ifndef TARGOMAN_COMMON_CONFIGURATION_TMPLCONFIGURABLEARRAY_HPP
#define TARGOMAN_COMMON_CONFIGURATION_TMPLCONFIGURABLEARRAY_HPP

#include <functional>
#include "libTargomanCommon/Configuration/intfConfigurable.hpp"
#include "libTargomanCommon/Configuration/ConfigManager.h"
#include "libTargomanCommon/Macros.h"
#include "libTargomanCommon/FastOperations.hpp"
#include "libTargomanCommon/Configuration/tmplConfigurable.h"

namespace Targoman {
namespace Common {
namespace Configuration {

template <class itmplType_t>
class tmplConfigurableArrayItem : public itmplType_t{
public:
    tmplConfigurableArrayItem(const QString& _baseconfigPath,
                              size_t _index) :
        itmplType_t(QString("%1%2/").arg(_baseconfigPath).arg(_index))
    { }

    tmplConfigurableArrayItem(const tmplConfigurableArrayItem& _other) :
        itmplType_t(_other){
        int a=2;
        ++a;
    }

    const itmplType_t& constData() const{
        return *this;
    }

    itmplType_t& data() const{
        return *this;
    }
};

class intfConfigurableArray  {
public:
    virtual void reserve(size_t _count) = 0;
};

template <class itmplType_t>
class tmplConfigurableArray : public intfConfigurable, public intfConfigurableArray
{
public:
    tmplConfigurableArray(const QString&  _configPath,
                          const QString&  _description,
                          size_t _minItemsCount = 0,
                          int    _maxItemsCount = -1) :
        intfConfigurable(enuConfigType::Array,
                         _configPath,
                         _description)
    {
        this->RemoteViewAllowed = false;
        this->ConfigSources = enuConfigSource::Invalid;
        this->ArgCount = -1;
        if (_maxItemsCount >= 0 && _maxItemsCount < _minItemsCount)
            throw exConfiguration(_configPath + " MaxItems must be greater than MinIems");
        this->MaxItems = _maxItemsCount;
        if (_minItemsCount)
            this->reserve(_minItemsCount);
    }

    virtual inline void setFromVariant(const QVariant& ){
        throw exTargomanNotImplemented(this->ConfigPath + " is Abstract");
    }

    virtual inline QVariant    toVariant() const{
        throw exTargomanNotImplemented(this->ConfigPath + " is Abstract");
    }

    virtual inline bool        validate(const QVariant&, QString&) const{
        return true;
    }

    virtual inline bool        crossValidate(QString& ) const{
        return true;
    }

    virtual QString typeString() const{
        return "NULL";
    }

    virtual QString validValues() const{
        return "";
    }

    void reserve(size_t _count){
        if (_count < Items.size())
            throw exConfiguration(this->configPath() + " cannot remove items via calling reserve");
        if (this->MaxItems<0 && this->Items.size() + _count > this->MaxItems)
            throw exConfiguration(this->configPath() + " is full. Cannot add more items");
        for (int i=Items.size(); i<_count; ++i)
            this->Items.append(tmplConfigurableArrayItem<itmplType_t>(this->configPath(), i));
    }

    const tmplConfigurableArrayItem<itmplType_t>& at(size_t _index){
        return this->Items.at(_index);
    }

    tmplConfigurableArrayItem<itmplType_t>& operator [](size_t _index){
        return this->Items[_index];
    }

    size_t size(){
        return this->Items.size();
    }

private:
    QList<tmplConfigurableArrayItem<itmplType_t>> Items;
    int MaxItems;
};

}
}
}
#endif // TARGOMAN_COMMON_CONFIGURATION_TMPLCONFIGURABLEARRAY_HPP
