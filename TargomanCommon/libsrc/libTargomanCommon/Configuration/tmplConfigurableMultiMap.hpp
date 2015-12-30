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
 */

#ifndef TARGOMAN_COMMON_CONFIGURATION_TMPLCONFIGURABLEMULTIMAP_HPP
#define TARGOMAN_COMMON_CONFIGURATION_TMPLCONFIGURABLEMULTIMAP_HPP

#include "libTargomanCommon/Configuration/tmplConfigurableArray.hpp"

namespace Targoman {
namespace Common {
namespace Configuration {

class intfConfigurableMultiMap  {
public:
    virtual void reserve(const QString& _key, quint16 _count) = 0;
};

template <class itmplType_t>
class tmplConfigurableMultiMap : public intfConfigurable, public intfConfigurableMultiMap
{
public:
    tmplConfigurableMultiMap(const clsConfigPath&  _configPath,
                             const QString&  _description = "") :
        intfConfigurable(enuConfigType::MultiMap,
                         _configPath.Path + "/",
                         _description)
    {
        this->RemoteViewAllowed = false;
        this->ConfigSources = enuConfigSource::Invalid;
        this->ArgCount = -1;
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

    void reserve(const QString& _key, quint16 _count){
        if (this->Items.contains(_key) == false)
            this->Items.insert(_key, tmplConfigurableArray<itmplType_t>(this->configPath() + _key + "/","",0));
        this->Items[_key].reserve(_count);
    }

    static const tmplConfigurableArray<itmplType_t>& invalidArray(){
        static tmplConfigurableArray<itmplType_t>* InvalidArray = NULL;
        return *(InvalidArray ?
                     InvalidArray :
                     (InvalidArray = new tmplConfigurableArray<itmplType_t>("InvalidArray","InvalidArray",1)));
    }

    bool contains(const QString& _key){
        return this->Items.contains(_key);
    }

    tmplConfigurableArray<itmplType_t> values(const QString& _key) const{
        if (this->Items.contains(_key) == false)
            throw exConfiguration("Invalid lookup to non existent item key: " + _key);
        return this->Items.value(_key,tmplConfigurableMultiMap::invalidArray());
    }

    tmplConfigurableArray<itmplType_t>& operator [](const QString& _key){
        if (this->Items.contains(_key) == false)
            throw exConfiguration("Invalid lookup to non existent item key: " + _key);
        return this->Items[_key];
    }

    qint32 size(){
        return this->size();
    }

    QStringList keys(){
        return this->Items.keys();
    }

private:
    QMap<QString, tmplConfigurableArray<itmplType_t>> Items;
};

}
}
}
#endif // TARGOMAN_COMMON_CONFIGURATION_TMPLCONFIGURABLEMULTIMAP_HPP
