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

#ifndef TARGOMAN_COMMON_CONFIGURATION_PRIVATE_RPCREGISTRY_HPP
#define TARGOMAN_COMMON_CONFIGURATION_PRIVATE_RPCREGISTRY_HPP

#include <QMetaMethod>
#include "libTargomanCommon/exTargomanBase.h"
#include "Configuration/intfRPCExporter.hpp"

namespace Targoman {
namespace Common {
namespace Configuration {
namespace Private {

TARGOMAN_ADD_EXCEPTION_HANDLER(exRPCReg, exTargomanBase);

class clsRPC : public QObject
{
public:
    clsRPC(QObject* _module, QMetaMethod _method, bool _async) :
        QObject(_module),
        Method(_method),
        IsAsync(_async)
    {}

    inline stuRPCOutput invoke(const QVariantMap& _args) const{
        Q_ASSERT(this->parent());
        stuRPCOutput Result;
        if (this->Method.invoke(this->parent(),
                                Qt::DirectConnection,
                                Q_RETURN_ARG(Common::Configuration::stuRPCOutput, Result),
                                Q_ARG(QVariantMap, _args)))
            return Result;
        else
            throw exRPCReg(QString("Error Invoking ") + Method.name());
    }

private:
    QMetaMethod   Method;
    bool          IsAsync;
};

class RPCRegistry
{
public:
    void registerRPC(QObject* _module, const QMetaMethod& _method){
        if ((_method.name().startsWith("rpc") == false &&
            _method.name().startsWith("asyncRPC") == false)||
            _method.typeName() == NULL ||
            QString(_method.typeName()).endsWith("stuRPCOutput") == false ||
            _method.parameterCount() != 1 ||
            _method.parameterTypes().first() != "QVariantMap")
            return;

        QString MethodName = _method.name().constData();
        MethodName = MethodName.mid(MethodName.indexOf("rpc",0,Qt::CaseInsensitive) + 3);

        this->Registry.insert(MethodName,
                              new clsRPC(_module,
                                         _method,
                                         _method.methodSignature().startsWith("async")));
    }

    const clsRPC& getRPCObject(const QString& _name){
        if (_name.startsWith("rpc") == false)
            throw exRPCReg("Invalid Method Name: <" + _name + ">");

        clsRPC* RPC = this->Registry.value(_name.mid(3));
        if(RPC == NULL)
            throw exRPCReg("Method " + _name + " Not Found");
        return *RPC;
    }

    inline QStringList registeredMethods(){return this->Registry.keys();}
    static inline RPCRegistry& instance(){
        static RPCRegistry*  Instance = NULL;
        return *(Q_LIKELY(Instance) ? Instance : (Instance = new RPCRegistry));
    }

private:
    RPCRegistry(){}
    Q_DISABLE_COPY(RPCRegistry)

private:
    QHash<QString, clsRPC*>  Registry;
};

}
}
}
}
#endif // TARGOMAN_COMMON_CONFIGURATION_PRIVATE_RPCREGISTRY_HPP
