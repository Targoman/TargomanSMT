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

#ifndef TARGOMAN_SMT_PRIVATE_SPECIAL_TOKEN_HANDLER_IXMLTAGHANDLER_H
#define TARGOMAN_SMT_PRIVATE_SPECIAL_TOKEN_HANDLER_IXMLTAGHANDLER_H

#include "Private/GlobalConfigs.h"
#include "libTargomanCommon/Constants.h"
#include "Private/SpecialTokenHandler/SpecialTokensRegistry.hpp"
#include "libTargomanTextProcessor/TextProcessor.h"
#include <libTargomanSMT/Private/RuleTable/clsTargetRule.h>
#include "libTargomanCommon/Configuration/tmplAddinConfig.hpp"

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
    IXMLTagHandler() {}
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

    TARGOMAN_DEFINE_SINGLETON_MODULE(IXMLTagHandler);

private:
    QMap<QString,intfIXMLTagHandlerModule*>                             ActiveTagHandlers;    /**< List of active special tag handlers.*/

private:

    static Targoman::Common::Configuration::tmplAddinConfig<intfIXMLTagHandlerModule>   IXMLTagHandlerModules;
    static QMap<QString, intfIXMLTagHandlerModule*>                     AvailableTagHandlers; /**< List of available special tag handlers*/
    static Targoman::Common::Configuration::tmplConfigurable<bool>      IgnoreUserDefinedTags;
    static Targoman::Common::Configuration::tmplConfigurable<bool>      KeepUnknownUserDefinedTags;
    friend class intfIXMLTagHandlerModule;
};

#define TARGOMAN_DEFINE_TAG_HANDLER_MODULE(_tagName, _name) \
public: \
    static inline QString tagName(){return Targoman::NLPLibs::enuTextTags::toStr(_tagName);}  \
    TARGOMAN_DEFINE_SINGLETON_MODULE(_name)
}
}
}
}
}

#endif // TARGOMAN_SMT_PRIVATE_SPECIAL_TOKEN_HANDLER_IXMLTAGHANDLER_H
