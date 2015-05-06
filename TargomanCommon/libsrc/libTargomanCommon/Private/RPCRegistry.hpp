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
                                Q_RETURN_ARG(stuRPCOutput, Result),
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
            strcmp(_method.typeName(), "Targoman::Common::Configuration::stuRPCOutput") ||
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
