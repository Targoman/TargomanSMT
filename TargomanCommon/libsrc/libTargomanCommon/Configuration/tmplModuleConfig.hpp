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
 * @author Saeed Torabzadeh <saeed.torabzadeh@targoman.com>
 */

#ifndef TARGOMAN_COMMON_CONFIGURATION_TMPLMODULECONFIG_HPP
#define TARGOMAN_COMMON_CONFIGURATION_TMPLMODULECONFIG_HPP

#include "libTargomanCommon/Configuration/intfConfigurable.hpp"
#include "libTargomanCommon/Configuration/ConfigManager.h"

namespace Targoman {
namespace Common {
namespace Configuration {
/**
 * @brief This class is a derivation of intfConfigurable that is specific for module configurables
 * (for example for setting language model module in program configuration)
 */
template<class itmplAcceptableInterface>
class tmplModuleConfig : public intfConfigurable{
public:
    tmplModuleConfig(const clsConfigPath&  _configPath,
                    const QString&  _description,
                    const QString&  _default,
                    const QString&  _shortSwitch = "",
                    const QString&  _shortHelp = "",
                    const QString&  _LongSwitch = "") :
        intfConfigurable(enuConfigType::Module,
                         _configPath,
                         _description,
                         _shortSwitch,
                         _shortHelp,
                         _LongSwitch,
                         (enuConfigSource::Type)(
                            enuConfigSource::Arg  |
                            enuConfigSource::File),
                         false){
        this->ActiveModuleName = _default;
        this->AcceptableModule = itmplAcceptableInterface::moduleScope();
    }
    virtual inline void setFromVariant(const QVariant& _var){
        this->ActiveModuleName = _var.toString();
    }

    virtual inline QVariant    toVariant() const{
        return this->ActiveModuleName;
    }

    virtual inline bool        validate(const QVariant&, QString&) const{
        return true;
    }

    /**
     * @brief puts instantiator of module in #Instantiator data member.
     * @exception throws exeption if module doesn't have a instantiator.
     */
    virtual inline void finalizeConfig(){
        this->Instantiatior =
                ConfigManager::instance().getInstantiator(
                    this->AcceptableModule + "::" + this->ActiveModuleName);
        if (this->Instantiatior == NULL)
            throw exConfiguration(
                    QString("Invalid module name <%1> for %d\nValid Options are: (%3)").arg(
                        this->ActiveModuleName).arg(
                        this->ConfigPath).arg(
                        ConfigManager::instance().registeredModules(
                            this->AcceptableModule).join('|')
                        )
                    );
    }
    /**
     * @brief returns #Instantiator.
     */
    template <class Type_t> Type_t* getInstance() const {
        return static_cast<Type_t*>(this->Instantiatior());
    }

    virtual QString typeString() const{
        return "QString";
    }

    virtual QString validValues() const{
        return "Module Valid Values reporting not implemented yet";
    }

private:
    QString ActiveModuleName;               /**< Module name which will be set by setFromVariant(const QVariant&)*/
    fpModuleInstantiator_t Instantiatior;   /**< Pointer to the Instantiator function of module.*/
    QString AcceptableModule;
};

}
}
}
#endif // TARGOMAN_COMMON_CONFIGURATION_TMPLMODULECONFIG_HPP
