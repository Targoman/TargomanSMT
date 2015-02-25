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

#ifndef INTFOOVHANDLERMODULE_HPP
#define INTFOOVHANDLERMODULE_HPP

#include "libTargomanCommon/Configuration/intfConfigurableModule.hpp"
#include "Private/RuleTable/clsTargetRule.h"
#include "OOVHandler.h"

namespace Targoman{
namespace Core {
namespace Private{
namespace OOV{

TARGOMAN_ADD_EXCEPTION_HANDLER(exOOVHandlerModule, exOOVHandler);

class intfOOVHandlerModule : public Common::Configuration::intfModule{
public:
    intfOOVHandlerModule(const QString& _moduleName, quint64 _instanceID) :
        intfModule(_moduleName)
    {
        Q_ASSERT_X(_instanceID, "intfOOVHandlerModule", "Multiple instance of OOVHandlerModule seems buggy.");
        OOVHandler::instance().AvailableOOVHandlers.insert(_moduleName, this);
    }

    virtual RuleTable::clsTargetRule process(const QString& _token, QVariantMap& _currAttrs) = 0;
};

}
}
}
}
#endif // INTFOOVHANDLERMODULE_HPP
