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


#ifndef TARGOMAN_COMMON_CONFIGURATION_INTFCONFIGURABLEMODULE_HPP
#define TARGOMAN_COMMON_CONFIGURATION_INTFCONFIGURABLEMODULE_HPP

#include <QString>
#include <QAtomicInt>
#include "libTargomanCommon/Logger.h"

namespace Targoman {
namespace Common {
namespace Configuration {
/**
 * @brief This is the base class for all module classes.
 */
class intfModule {
public:
    intfModule(const QString& _moduleName, quint64 _instanceID = 0){
        this->InstanceID = _instanceID;
        if (_instanceID == 0)
            TARGOMAN_REGISTER_ACTOR(_moduleName);
    }

protected:
    virtual void unregister() = 0;

protected:
    QString ActorUUID;
    quint64 InstanceID;
};
/**
 * fpModuleInstantiator_t is function pointer to functions that returns pointer of intfModule (or its derivations) and have no argumnt.
 */
typedef intfModule* (*fpModuleInstantiator_t)();

/**
 * @brief This struct encapsulates modules's instantiator function pointer and wethere it is singleton or not.
 */
struct stuInstantiator{
    fpModuleInstantiator_t fpMethod;
    bool                 IsSingleton;
    stuInstantiator(fpModuleInstantiator_t _method = NULL,
                    bool _isSingleton = false){
        this->fpMethod = _method;
        this->IsSingleton = _isSingleton;
    }
};
/**
 * @brief This class is defined to make a static member of it in module class because we wanted its constructor to be called before main function.
 * In its constructor, the module that has a member of this class, inserts its instantiator to ModuleInstantiators Map of pPrivate member of ConfigManager.
 */
class clsModuleRegistrar{
public:
    clsModuleRegistrar(const QString& _name, stuInstantiator _instantiatior);
};


/**
 * @def TARGOMAN_DEFINE_MODULE adds three function and one data member to not singleton module classes.
 * Registrar member is the static member that is of type clsModuleRegistrar and will be instantiated before main to insert module instantiator to ModuleInstantiators Map of pPrivate member of ConfigManager.
 */
#define TARGOMAN_DEFINE_MODULE(_name, _class) \
public: \
    void   unregister(){/*TARGOMAN_UNREGISTER_ACTOR;*/} \
    static QString moduleName(){_class::ActiveInstances.fetchAndAddOrdered(-1);return QStringLiteral(_name);}  \
    static Targoman::Common::Configuration::intfModule* instantiator(){_class::ActiveInstances.fetchAndAddOrdered(1); return new _class(_class::Instances.fetchAndAddOrdered(1));} \
private: \
    static Targoman::Common::Configuration::clsModuleRegistrar Registrar; \
    static QAtomicInt Instances; \
    static QAtomicInt ActiveInstances


/**
 * @def TARGOMAN_DEFINE_SINGLETONMODULE adds three function and two data member to singleton module classes.
 * Registrar member is the static member that is of type clsModuleRegistrar and will be instantiated before main to insert module instantiator to ModuleInstantiators Map of pPrivate member of ConfigManager.
 */

#define TARGOMAN_DEFINE_SINGLETONMODULE(_name, _class) \
public: \
    void   unregister(){}\
    static _class& instance() {return *((_class*)_class::moduleInstance());} \
    static Targoman::Common::Configuration::intfModule* moduleInstance(){static _class* Instance = NULL; return Q_LIKELY(Instance) ? Instance : (Instance = new _class);} \
    static QString moduleName(){return QStringLiteral(_name);}  \
private: \
    Q_DISABLE_COPY(_class) \
    static Targoman::Common::Configuration::clsModuleRegistrar Registrar;

/**
 * @def TARGOMAN_REGISTER_MODULE initialization of Registrar member for non singleton classes.
 */
#define TARGOMAN_REGISTER_MODULE(_class) \
    Targoman::Common::Configuration::clsModuleRegistrar _class::Registrar(_class::moduleName(), \
                      Targoman::Common::Configuration::stuInstantiator(_class::instantiator,false)); \
    QAtomicInt _class::Instances; \
    QAtomicInt _class::ActiveInstances

/**
 * @def TARGOMAN_REGISTER_MODULE initialization of Registrar member for singleton classes. Also makes a null instance of class.
 */

#define TARGOMAN_REGISTER_SINGLETON_MODULE(_class) \
    Targoman::Common::Configuration::clsModuleRegistrar _class::Registrar(_class::moduleName(), \
                      Targoman::Common::Configuration::stuInstantiator(_class::moduleInstance,true));
}
}
}

#endif // TARGOMAN_COMMON_CONFIGURATION_INTFCONFIGURABLEMODULE_HPP
