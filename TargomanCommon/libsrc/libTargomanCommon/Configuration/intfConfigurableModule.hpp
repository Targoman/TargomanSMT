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
#include "libTargomanCommon/Helpers.hpp"

#define TARGOMAN_CAT_BY_SLASH(_m1,_m2) _m1/_m2
#define MAKE_ABSOLUTE_MODULE_NAME \
    QString("%1/%2").arg(TARGOMAN_M2STR(CONFIG_ROOT_PATH)).arg(moduleName()).replace("//", "/")


namespace Targoman {
namespace Common {
namespace Configuration {
/**
 * @brief This is the base class for all module classes.
 */
class intfModule {
public:
    intfModule(){}

protected:
    virtual QString moduleFullName() {
        throw exTargomanMustBeImplemented("Seems that you forgot to use TARGOMAN_DEFINE_MODULE_SCOPE macro");
    }
};

//Following macro must be used in all interfaces derved from intfModule
#define TARGOMAN_DEFINE_MODULE_SCOPE(_intf) \
    static QString moduleScope(){ \
        QString FN = Targoman::Common::demangle(typeid(_intf).name()); \
        return FN.mid(0,FN.lastIndexOf("::")); \
    } \
    virtual QString moduleFullName() { \
        throw exTargomanMustBeImplemented("Seems that you forgot to use module definition macros");\
    }



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
                    bool _isSingleton = false ){
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
    clsModuleRegistrar(const QString& _fullName,
                       const QString& _name,
                       stuInstantiator _instantiatior);
};


/**
 * @def TARGOMAN_DEFINE_MODULE adds three function and one data member to not singleton module classes.
 * Registrar member is the static member that is of type clsModuleRegistrar and will be instantiated before main to insert module instantiator to ModuleInstantiators Map of pPrivate member of ConfigManager.
 */
#define TARGOMAN_DEFINE_MODULE(_name) \
public: \
    QString moduleFullName(){return Targoman::Common::demangle(typeid(*this).name());}\
    static QString moduleFullNameStatic(){return Targoman::Common::demangle(typeid(cls##_name).name());} \
    static QString moduleName(){return QStringLiteral(TARGOMAN_M2STR(_name));}  \
    static Targoman::Common::Configuration::intfModule* instantiator(){ \
       return new cls##_name;} \
private: \
    static Targoman::Common::Configuration::clsModuleRegistrar Registrar; \
    static QAtomicInt Instances;

/**
 * @def TARGOMAN_DEFINE_SINGLETON_MODULE adds three function and two data member to singleton module classes.
 * Registrar member is the static member that is of type clsModuleRegistrar and will be instantiated before main to insert module instantiator to ModuleInstantiators Map of pPrivate member of ConfigManager.
 */
#define TARGOMAN_DEFINE_SINGLETON_MODULE(_name) \
public: \
    static QString moduleFullNameStatic(){return Targoman::Common::demangle(typeid(_name).name());}\
    QString moduleFullName(){return _name::moduleFullNameStatic();}\
    static _name& instance() {return *((_name*)_name::moduleInstance());} \
    static Targoman::Common::Configuration::intfModule* moduleInstance(){static _name* Instance = NULL; return Q_LIKELY(Instance) ? Instance : (Instance = new _name);} \
    static QString moduleName(){return QStringLiteral(TARGOMAN_M2STR(_name));}  \
private: \
    Q_DISABLE_COPY(_name) \
    static Targoman::Common::Configuration::clsModuleRegistrar Registrar;

#define TARGOMAN_DEFINE_SINGLETONSUBMODULE(_module, _name) \
public: \
    static QString moduleFullNameStatic(){return Targoman::Common::demangle(typeid(_name).name());}\
    virtual QString moduleFullName(){return _name::moduleFullNameStatic();}\
    static _name& instance() {return *((_name*)_name::moduleInstance());} \
    static Targoman::Common::Configuration::intfModule* moduleInstance(){static _name* Instance = NULL; return Q_LIKELY(Instance) ? Instance : (Instance = new _name);} \
    static QString moduleName(){return QStringLiteral(TARGOMAN_M2STR(TARGOMAN_CAT_BY_SLASH(_module,_name)));}  \
private: \
    Q_DISABLE_COPY(_name) \
    static Targoman::Common::Configuration::clsModuleRegistrar Registrar;

/**
 * @def TARGOMAN_REGISTER_MODULE initialization of Registrar member for non singleton classes.
 */
#define TARGOMAN_REGISTER_MODULE(_class) \
    Targoman::Common::Configuration::clsModuleRegistrar _class::Registrar( \
         _class::moduleFullNameStatic(), \
         MAKE_ABSOLUTE_MODULE_NAME, \
         Targoman::Common::Configuration::stuInstantiator(_class::instantiator,false)); \
    QAtomicInt _class::Instances;

/**
 * @def TARGOMAN_REGISTER_MODULE initialization of Registrar member for singleton classes. Also makes a null instance of class.
 */
#define TARGOMAN_REGISTER_SINGLETON_MODULE(_class) \
    Targoman::Common::Configuration::clsModuleRegistrar _class::Registrar(\
        _class::moduleFullNameStatic(), \
        MAKE_ABSOLUTE_MODULE_NAME, \
        Targoman::Common::Configuration::stuInstantiator(_class::moduleInstance,true));
}
}
}

#endif // TARGOMAN_COMMON_CONFIGURATION_INTFCONFIGURABLEMODULE_HPP
