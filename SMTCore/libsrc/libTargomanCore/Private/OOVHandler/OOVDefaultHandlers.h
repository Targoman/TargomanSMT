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

#ifndef OOVDEFAULTHANDLERS_H
#define OOVDEFAULTHANDLERS_H

#include "intfOOVHandlerModule.hpp"
#include "Private/InputDecomposer/clsInput.h"

namespace Targoman{
namespace Core {
namespace Private{
namespace OOV{

using namespace RuleTable;

class clsOOVRemoveOnTarget : public intfOOVHandlerModule{
public:
    clsOOVRemoveOnTarget(quint64 _instanceID): intfOOVHandlerModule(this->moduleName(), _instanceID){}
    ~clsOOVRemoveOnTarget();

    RuleTable::clsTargetRule process(const QString &_token, QVariantMap& _attrs){
        Q_UNUSED(_token)

        _attrs.insert(InputDecomposer::enuDefaultAttrs::toStr(InputDecomposer::enuDefaultAttrs::NoShow), true);
        return clsTargetRule();
    }

private:

    TARGOMAN_DEFINE_MODULE("OOVRemoveOnTarget", clsOOVRemoveOnTarget)
};

class clsOOVRemoveDecoding : public intfOOVHandlerModule{
public:
    clsOOVRemoveDecoding(quint64 _instanceID): intfOOVHandlerModule(this->moduleName(), _instanceID){}
    ~clsOOVRemoveDecoding();

    RuleTable::clsTargetRule process(const QString &_token, QVariantMap& _attrs){
        Q_UNUSED(_token)

        _attrs.insert(InputDecomposer::enuDefaultAttrs::toStr(InputDecomposer::enuDefaultAttrs::NoDecode), true);
        return clsTargetRule();
    }

private:

    TARGOMAN_DEFINE_MODULE("OOVRemoveDecoding", clsOOVRemoveDecoding)
};

class clsOOVKeepSource : public intfOOVHandlerModule{
public:
    clsOOVKeepSource(quint64 _instanceID): intfOOVHandlerModule(this->moduleName(), _instanceID){}
    ~clsOOVKeepSource();

    RuleTable::clsTargetRule process(const QString &_token, QVariantMap& _attrs){
        Q_UNUSED(_token)

        _attrs.insert(InputDecomposer::enuDefaultAttrs::toStr(InputDecomposer::enuDefaultAttrs::ShowSource), true);
        return clsTargetRule();
    }

private:

    TARGOMAN_DEFINE_MODULE("OOVKeepSource", clsOOVKeepSource)
};

}
}
}
}

#endif // OOVDEFAULTHANDLERS_H
