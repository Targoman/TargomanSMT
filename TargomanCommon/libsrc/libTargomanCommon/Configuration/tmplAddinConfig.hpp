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

#ifndef TARGOMAN_COMMON_CONFIGURATION_TMPLADDINCONFIG_HPP
#define TARGOMAN_COMMON_CONFIGURATION_TMPLADDINCONFIG_HPP

#include "libTargomanCommon/Configuration/intfConfigurable.hpp"
#include "libTargomanCommon/Configuration/ConfigManager.h"

namespace Targoman {
namespace Common {
namespace Configuration {
/**
 * @brief This class is a derivation of intfConfigurable that is specific for addin configurables
 */
template<class itmplAcceptableInterface>
class tmplAddinConfig : public intfConfigurable{
public:
    tmplAddinConfig(const clsConfigPath&  _configPath,
                    const QString&  _description,
                    const QStringList&  _default = QStringList(),
                    const QString&  _shortSwitch = "",
                    const QString&  _shortHelp = "",
                    const QString&  _LongSwitch = "") :
        intfConfigurable(enuConfigType::Addin,
                         _configPath,
                         _description,
                         _shortSwitch,
                         _shortHelp,
                         _LongSwitch,
                         (enuConfigSource::Type)(
                             enuConfigSource::Arg  |
                             enuConfigSource::File),
                         false){
        this->ActiveAddins = _default;
        this->AcceptableModule = itmplAcceptableInterface::moduleScope();
    }
    virtual inline void setFromVariant(const QVariant& _var){
        if (_var.canConvert(QVariant::StringList))
            this->ActiveAddins = _var.toStringList();
        else
            this->ActiveAddins = _var.toString().split(',');
    }

    virtual inline QVariant    toVariant() const{
        return this->ActiveAddins.join(",");
    }

    virtual inline bool        validate(const QVariant&, QString&) const{
        return true;
    }

    /**
     * @brief puts instantiator of module in #Instantiator data member.
     * @exception throws exeption if module doesn't have a instantiator.
     */
    virtual inline void finalizeConfig(){
        foreach (const QString& Addin, this->ActiveAddins){
            bool IsSingleton;
            ConfigManager::instance().getInstantiator(
                        this->AcceptableModule + "::" + Addin,
                        this->Instantiatior,
                        IsSingleton);
            if (this->Instantiatior == NULL){

                throw exConfiguration(
                            QString("Invalid addin name <%1> for %2\nValid Options are: (%3)\n"
                                    "Maybe you have forgotten to place:\n"
                                    "\tTARGOMAN_REGISTER_SINGLETON_MODULE(%1)\n"
                                    "In implementation of <%1>").arg(
                                Addin).arg(
                                this->ConfigPath).arg(
                                this->validValues())
                            );
            }
            if (IsSingleton == false)
                throw exConfiguration("Addin <" + Addin + "> must be singleton");
            intfModule* Instance = this->Instantiatior();
            if(dynamic_cast<itmplAcceptableInterface*>(Instance) == NULL)
                throw exConfiguration(QString("Addin: %1 must be of type %2").arg(
                                          typeid(itmplAcceptableInterface).name()));
        }
    }
    /**
     * @brief returns #Instantiator.
     */
    template <class Type_t> Type_t* getInstance() const {
        return static_cast<Type_t*>(this->Instantiatior());
    }

    virtual QString typeString() const{
        return "AddinConfig";
    }

    virtual QString validValues() const{
        QStringList AcceptableAddins;
        foreach(const QString CheckingAddin,
                ConfigManager::instance().registeredModules(this->AcceptableModule)){
            try{
                fpModuleInstantiator_t Instantiator;
                bool IsSingleton;
                ConfigManager::instance().getInstantiator(
                            this->AcceptableModule + "::" + CheckingAddin,
                            Instantiator,
                            IsSingleton);
                intfModule* Instance = Instantiator();
                if(dynamic_cast<itmplAcceptableInterface*>(Instance) != NULL)
                    AcceptableAddins.append(CheckingAddin);
            }catch(exConfiguration &e){
                //Ignore exception on non-singleton modules
            }
        }
        return AcceptableAddins.join('|');
    }

private:
    QStringList ActiveAddins;           /**< Module name which will be set by setFromVariant(const QVariant&)*/
    fpModuleInstantiator_t Instantiatior;   /**< Pointer to the Instantiator function of module.*/
    QString AcceptableModule;
};

}
}
}
#endif // TARGOMAN_COMMON_CONFIGURATION_TMPLADDINCONFIG_HPP
