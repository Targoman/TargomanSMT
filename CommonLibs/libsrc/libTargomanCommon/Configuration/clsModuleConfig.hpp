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

#ifndef TARGOMAN_COMMON_CONFIGURATION_CLSMODULECONFIG_HPP
#define TARGOMAN_COMMON_CONFIGURATION_CLSMODULECONFIG_HPP

#include "libTargomanCommon/Configuration/intfConfigurable.hpp"
#include "libTargomanCommon/Configuration/ConfigManager.h"

namespace Targoman {
namespace Common {
namespace Configuration {

class clsModuleConfig : public intfConfigurable{
public:
    clsModuleConfig(const QString&  _configPath,
                    const QString&  _description,
                    const QString&  _shortSwitch = "",
                    const QString&  _shortHelp = "",
                    const QString&  _LongSwitch = "") :
        intfConfigurable(_configPath,
                         _description,
                         _shortSwitch,
                         _shortHelp,
                         _LongSwitch){
        this->ArgCount = -1;
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

    virtual inline void finalizeConfig(){
        this->Instantiatior = ConfigManager::instance().getInstantiator(this->ActiveModuleName);
        if (this->Instantiatior == NULL)
            throw exConfiguration("Invalid module name <" + this->ActiveModuleName + "> for " + this->configPath());
    }

private:
    QString ActiveModuleName;
    fpModuleInstantiator Instantiatior;
};

}
}
}
#endif // TARGOMAN_COMMON_CONFIGURATION_CLSMODULECONFIG_HPP
