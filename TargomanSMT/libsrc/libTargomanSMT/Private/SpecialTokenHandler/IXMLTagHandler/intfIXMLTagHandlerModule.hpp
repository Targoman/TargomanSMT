#ifndef TARGOMAN_SMT_PRIVATE_SPECIAL_TOKEN_HANDLER_INTFIXMLTAGHANDLERMODULE_HPP
#define TARGOMAN_SMT_PRIVATE_SPECIAL_TOKEN_HANDLER_INTFIXMLTAGHANDLERMODULE_HPP
#include "libTargomanCommon/Configuration/intfConfigurableModule.hpp"
#include "libTargomanSMT/Private/RuleTable/clsRuleNode.h"
#include "libTargomanSMT/Private/InputDecomposer/clsInput.h"
#include "libTargomanSMT/Private/RuleTable/clsTargetRule.h"
#include "libTargomanTextProcessor/TextProcessor.h"
#include "IXMLTagHandler.h"

#include "Types.h"


namespace Targoman{
namespace SMT {
namespace Private{
namespace SpecialTokenHandler {
namespace IXMLTagHandler {
using namespace RuleTable;

class intfIXMLTagHandlerModule : public Common::Configuration::intfModule {
public:
    /**
     * @brief When an instance of any kind special OOV handler instantiates, its name and pointer of itself will be added to AvailableOOVHandlers data member of OOVHandler class.
     */
    intfIXMLTagHandlerModule(const QString& _tagName, const QString& _moduleName, quint64 _instanceID) :
        intfModule(_moduleName, _instanceID)
    {
#ifndef TARGOMAN_SHOW_DEBUG
        Q_UNUSED(_instanceID)
#else
        Q_ASSERT_X(_instanceID == 0, "intfIXMLTagHandlerModule()", "Multiple instances of IXMLTagHandlerModule may probably cause bugs");
#endif
        IXMLTagHandler::instance().AvailableTagHandlers.insert(_tagName, this);
    }
    virtual TargetRulesContainer_t getTargetRules(const QString& _token) = 0;
    virtual QString getDefaultTranslation(const QString& _token) = 0;

};

}
}
}
}
}

#endif // TARGOMAN_SMT_PRIVATE_SPECIAL_TOKEN_HANDLER_INTFIXMLTAGHANDLERMODULE_HPP
