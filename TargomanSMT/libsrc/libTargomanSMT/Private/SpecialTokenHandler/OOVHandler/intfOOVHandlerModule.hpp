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

#ifndef TARGOMAN_SMT_PRIVATE_SPECIAL_TOKEN_HANDLER_OOV_INTFOOVHANDLERMODULE_HPP
#define TARGOMAN_SMT_PRIVATE_SPECIAL_TOKEN_HANDLER_OOV_INTFOOVHANDLERMODULE_HPP

#include "libTargomanCommon/Configuration/intfConfigurableModule.hpp"
#include "Private/RuleTable/clsTargetRule.h"
#include "OOVHandler.h"

namespace Targoman{
namespace SMT {
namespace Private{
namespace SpecialTokenHandler {
namespace OOV{

TARGOMAN_ADD_EXCEPTION_HANDLER(exOOVHandlerModule, exOOVHandler);
/**
 * @brief The intfOOVHandlerModule class is an interface class that every other Special OOV Handlers like clsOOVRemoveOnTarget or clsOOVKeepSource can be derive from this interface class.
 */
class intfOOVHandlerModule : public Common::Configuration::intfModule{
public:
    /**
     * @brief When an instance of any kind special OOV handler instantiates, its name and pointer of itself will be added to AvailableOOVHandlers data member of OOVHandler class.
     */
    intfOOVHandlerModule(const QString& _moduleName) {
       OOVHandler::instance().AvailableOOVHandlers.insert(_moduleName, this);
    }

    virtual RuleTable::clsTargetRule process(const QString& _token, QVariantMap& _currAttrs) = 0;
};

}
}
}
}
}
#endif // TARGOMAN_SMT_PRIVATE_SPECIAL_TOKEN_HANDLER_OOV_INTFOOVHANDLERMODULE_HPP
