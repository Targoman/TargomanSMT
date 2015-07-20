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

#ifndef TARGOMAN_SMT_PRIVATE_SPECIAL_TOKEN_HANDLER_OOV_OOVHANDLER_H
#define TARGOMAN_SMT_PRIVATE_SPECIAL_TOKEN_HANDLER_OOV_OOVHANDLER_H

#include "libTargomanCommon/tmplExpirableCache.hpp"
#include "libTargomanCommon/Configuration/tmplConfigurable.h"
#include "Private/RuleTable/clsRuleNode.h"
#include "Private/GlobalConfigs.h"
#include "Private/Proxies/intfLMSentenceScorer.hpp"
#include "libTargomanCommon/Constants.h"
#include "Private/SpecialTokenHandler/SpecialTokensRegistry.hpp"

namespace Targoman{
namespace SMT {
namespace Private{
namespace SpecialTokenHandler {
using namespace Common;

/**
 *  @brief Namespace surrounding all of the classes related to Out Of Vocabulary handling
 */
namespace OOV{

class intfOOVHandlerModule;

TARGOMAN_ADD_EXCEPTION_HANDLER(exOOVHandler, exTargomanCore);

/**
 * @brief The OOVHandler class is responsible for associating word index, string, caching and making rule node for OOV words.
 */
class OOVHandler : public Targoman::Common::Configuration::intfModule
{
public:
    OOVHandler(): intfModule(this->moduleName()) {}
    QList<WordIndex_t> getWordIndexOptions(const QString& _token, QVariantMap& _attrs);
    RuleTable::TargetRulesContainer_t generateTargetRules(const QString& _token);
    void initialize();

private:
    RuleTable::TargetRulesContainer_t gatherTargetRules(const QString& _token, QVariantMap& _attrs);

private:
    TARGOMAN_DEFINE_SINGLETONMODULE("OOVHandler", OOVHandler);

private:
    QList<intfOOVHandlerModule*>                                        ActiveOOVHandlers;        /**< List of active special OOV handlers.*/

private:
    static Targoman::Common::Configuration::tmplConfigurable<QString>   OOVHandlerModules;
    static QMap<QString, intfOOVHandlerModule*>                         AvailableOOVHandlers; /**< List of available special OOV handlers*/
    friend class intfOOVHandlerModule;
};

}
}
}
}
}
#endif // TARGOMAN_SMT_PRIVATE_SPECIAL_TOKEN_HANDLER_OOV_OOVHANDLER_H
