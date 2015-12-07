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
    intfIXMLTagHandlerModule(const QString& _tagName, quint64 _instanceID) :
        intfModule(_instanceID)
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
