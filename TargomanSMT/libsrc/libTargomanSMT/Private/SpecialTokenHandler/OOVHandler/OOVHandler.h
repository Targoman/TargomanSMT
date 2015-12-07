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
 * @author Saeed Torabzadeh <saeed.torabzadeh@targoman.com>
 */

#ifndef TARGOMAN_SMT_PRIVATE_SPECIAL_TOKEN_HANDLER_OOV_OOVHANDLER_H
#define TARGOMAN_SMT_PRIVATE_SPECIAL_TOKEN_HANDLER_OOV_OOVHANDLER_H

#include "libTargomanCommon/tmplBoundedCache.hpp"
#include "libTargomanCommon/Configuration/tmplConfigurable.h"
#include "Private/RuleTable/clsRuleNode.h"
#include "Private/GlobalConfigs.h"
#include "Private/Proxies/LanguageModel/intfLMSentenceScorer.hpp"
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
    static Targoman::Common::Configuration::tmplConfigurable<bool>      CheckDifferentLetterCases;
    static QMap<QString, intfOOVHandlerModule*>                         AvailableOOVHandlers; /**< List of available special OOV handlers*/
    friend class intfOOVHandlerModule;
};

}
}
}
}
}
#endif // TARGOMAN_SMT_PRIVATE_SPECIAL_TOKEN_HANDLER_OOV_OOVHANDLER_H
