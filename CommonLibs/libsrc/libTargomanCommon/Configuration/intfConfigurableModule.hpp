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

#ifndef TARGOMAN_COMMON_CONFIGURATION_INTFCONFIGURABLEMODULE_HPP
#define TARGOMAN_COMMON_CONFIGURATION_INTFCONFIGURABLEMODULE_HPP

#include <QString>
#include "libTargomanCommon/Logger.h"

namespace Targoman {
namespace Common {
namespace Configuration {

class intfModule {
public:
    intfModule(const QString& _name){
        TARGOMAN_REGISTER_ACTOR(_name)
    }

protected:
    QString ActorUUID;
};

typedef intfModule* (*fpModuleInstantiator)();

class clsModuleRegistrar{
public:
    clsModuleRegistrar(const QString& _name, fpModuleInstantiator _instantiatior);
};

#define TARGOMAN_DEFINE_MODULE(_class) \
    static Targoman::Common::Configuration::intfModule* instantiator(){return new _class;} \
    static Targoman::Common::Configuration::clsModuleRegistrar Registrar;

#define TARGOMAN_REGISTER_MODULE(_name, _class) \
    Common::Configuration::clsModuleRegistrar clsTargomanLMProxy::Registrar(_name, _class::instantiator());

}
}
}

#endif // TARGOMAN_COMMON_CONFIGURATION_INTFCONFIGURABLEMODULE_HPP
