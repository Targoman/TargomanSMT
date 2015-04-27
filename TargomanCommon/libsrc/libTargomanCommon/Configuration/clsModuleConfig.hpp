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

#ifndef TARGOMAN_COMMON_CONFIGURATION_CLSMODULECONFIG_HPP
#define TARGOMAN_COMMON_CONFIGURATION_CLSMODULECONFIG_HPP

#include "libTargomanCommon/Configuration/intfConfigurable.hpp"
#include "libTargomanCommon/Configuration/ConfigManager.h"

namespace Targoman {
namespace Common {
namespace Configuration {
/**
 * @brief This class is a derivation of intfConfigurable that is specific for module configurables
 * (for example for setting language model module in program configuration)
 */
class clsModuleConfig : public intfConfigurable{
public:
    clsModuleConfig(const QString&  _configPath,
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
        this->Instantiatior = ConfigManager::instance().getInstantiator(this->ActiveModuleName);
        if (this->Instantiatior == NULL)
            throw exConfiguration("Invalid module name <" + this->ActiveModuleName + "> for " + this->configPath());
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
};

}
}
}
#endif // TARGOMAN_COMMON_CONFIGURATION_CLSMODULECONFIG_HPP
