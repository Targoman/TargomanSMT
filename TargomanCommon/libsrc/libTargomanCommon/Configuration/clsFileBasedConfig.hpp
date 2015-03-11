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

#ifndef TARGOMAN_COMMON_CONFIGURATION_CLSFILEBASEDCONFIG_HPP
#define TARGOMAN_COMMON_CONFIGURATION_CLSFILEBASEDCONFIG_HPP

#include "libTargomanCommon/Configuration/intfConfigurable.hpp"

namespace Targoman {
namespace Common {
namespace Configuration {

/**
 * @brief The clsFileBasedConfig class is used when there are more optional configs stored in configuration file
 * this optional configs will not be stored and monitored by configuration manager.
 */
class clsFileBasedConfig : public intfConfigurable{
public:
    clsFileBasedConfig(const QString&  _configPath) :
        intfConfigurable(_configPath, "OPTIONAL_CONFIGS_IN_FILE","","","",enuConfigSource::Virtual){
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
};

}
}
}

#endif // TARGOMAN_COMMON_CONFIGURATION_CLSFILEBASEDCONFIG_HPP
