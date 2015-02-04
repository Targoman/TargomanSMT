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

typedef intfModule* (*fpModuleInstantiator_t)();

struct stuInstantiator{
    fpModuleInstantiator_t fpMethod;
    bool                 IsSingleton;
    stuInstantiator(fpModuleInstantiator_t _method = NULL,
                    bool _isSingleton = false){
        this->fpMethod = _method;
        this->IsSingleton = _isSingleton;
    }
};

class clsModuleRegistrar{
public:
    clsModuleRegistrar(const QString& _name, stuInstantiator _instantiatior);
};

#define TARGOMAN_DEFINE_MODULE(_name, _class) \
private: \
    static inline QString moduleName(){return QStringLiteral(_name);}  \
    static inline _class* instantiator(){return new _class;} \
    static inline QString baseConfigPath(){return "/" + moduleName();} \
    static Targoman::Common::Configuration::clsModuleRegistrar Registrar;

#define TARGOMAN_DEFINE_SINGLETONMODULE(_name, _class) \
private: \
    static inline QString moduleName(){return QStringLiteral(_name);}  \
    static inline _class* instance(){return Q_LIKELY(Instance) ? Instance : (Instance = new _class);} \
    static inline QString baseConfigPath(){return "/" + moduleName();} \
    static Targoman::Common::Configuration::clsModuleRegistrar Registrar; \
    static _class* Instance;

#define TARGOMAN_REGISTER_MODULE(_class) \
    Targoman::Common::Configuration::clsModuleRegistrar _class::Registrar(_class::moduleName(), \
                      stuInstantiator(_class::instantiator,false));

#define TARGOMAN_REGISTER_SINGLETON_MODULE(_class) \
    Targoman::Common::Configuration::clsModuleRegistrar _class::Registrar(_class::moduleName(), \
                      stuInstantiator(_class::instance,true)); \
    _class* _class::Instance = NULL;
}
}
}

#endif // TARGOMAN_COMMON_CONFIGURATION_INTFCONFIGURABLEMODULE_HPP
