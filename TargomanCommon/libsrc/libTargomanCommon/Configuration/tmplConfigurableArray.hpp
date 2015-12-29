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
        itmplType_t(_other)
    { }

    const itmplType_t& constData() const{
        return *this;
    }

    itmplType_t& data(){
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
    tmplConfigurableArray(const clsConfigPath&  _configPath = clsConfigPath(""),
                          const QString&  _description = ""):
        intfConfigurable(enuConfigType::Unknown,
                         _configPath,
                         _description) {
        Q_ASSERT_X(false, "tmplConfigurableArray", "Default constructor must never be called");
        throw exTargomanNotImplemented("Default tmplConfigurableArray constructor must never be called");
    }

    tmplConfigurableArray(const QString&  _configPath,
                          const QString&  _description,
                           size_t _minItemsCount,
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

    tmplConfigurableArray(const tmplConfigurableArray& _other) :
        intfConfigurable(_other),
        Items(_other.Items),
        MaxItems(_other.MaxItems)
    { }

    tmplConfigurableArray& operator = (const tmplConfigurableArray& _other) {
        intfConfigurable::operator =(_other);
        Items = _other.Items;
        MaxItems = _other.MaxItems;
        return *this;
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

    const tmplConfigurableArrayItem<itmplType_t>& at(size_t _index) const{
        return this->Items.at(_index);
    }

    tmplConfigurableArrayItem<itmplType_t>& operator [](size_t _index){
        return this->Items[_index];
    }

    size_t size() const{
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
