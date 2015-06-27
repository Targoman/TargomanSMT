#ifndef TARGOMAN_SMT_PRIVATE_SPECIAL_TOKEN_HANDLER_IXMLTAGHANDLER_H
#define TARGOMAN_SMT_PRIVATE_SPECIAL_TOKEN_HANDLER_IXMLTAGHANDLER_H

#include "Private/GlobalConfigs.h"
#include "libTargomanCommon/Constants.h"
#include "Private/SpecialTokenHandler/SpecialTokensRegistry.hpp"
#include "libTargomanTextProcessor/TextProcessor.h"
#include <libTargomanSMT/Private/RuleTable/clsTargetRule.h>

namespace Targoman{
namespace SMT {
namespace Private{
namespace SpecialTokenHandler {
namespace IXMLTagHandler {
using namespace Common;
using namespace RuleTable;

class intfIXMLTagHandlerModule;
TARGOMAN_ADD_EXCEPTION_HANDLER(exIXMLTagHandler, exTargomanCore);

class IXMLTagHandler : public Targoman::Common::Configuration::intfModule
{
public:
    IXMLTagHandler(): intfModule(this->moduleName()) {}
    QList<WordIndex_t> getWordIndexOptions(const QString& _tagStr,
                                           const QString& _token,
                                           INOUT QVariantMap& _attrs);
    QList<WordIndex_t> process(const QString& _tagStr,
                               const QString& _token,
                               INOUT QVariantMap& _attrs,
                               const TargetRulesContainer_t& _targetRules,
                               const QString& _defaultTranslation);
    void initialize();


private:

    TARGOMAN_DEFINE_SINGLETONMODULE("IXMLTagHandler", IXMLTagHandler);

private:
    QMap<QString,intfIXMLTagHandlerModule*>                             ActiveTagHandlers;    /**< List of active special tag handlers.*/

private:

    static Targoman::Common::Configuration::tmplConfigurable<QString>   IXMLTagHandlerModules;
    static QMap<QString, intfIXMLTagHandlerModule*>                     AvailableTagHandlers; /**< List of available special tag handlers*/
    static Targoman::Common::Configuration::tmplConfigurable<bool>      IgnoreUserDefinedTags;
    static Targoman::Common::Configuration::tmplConfigurable<bool>      KeepUnknownUserDefinedTags;
    friend class intfIXMLTagHandlerModule;
};

#define TARGOMAN_DEFINE_TAG_HANDLER_MODULE(_tagName,_moduleName, _class) \
public: \
    static inline QString tagName(){return Targoman::NLPLibs::enuTextTags::toStr(_tagName);}  \
    TARGOMAN_DEFINE_MODULE(_moduleName, _class)


}
}
}
}
}

#endif // TARGOMAN_SMT_PRIVATE_SPECIAL_TOKEN_HANDLER_IXMLTAGHANDLER_H
