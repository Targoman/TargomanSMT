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

#ifndef TARGOMAN_SMT_PRIVATE_SPECIAL_TOKEN_HANDLER_OOV_OOVDEFAULTHANDLERS_H
#define TARGOMAN_SMT_PRIVATE_SPECIAL_TOKEN_HANDLER_OOV_OOVDEFAULTHANDLERS_H

#include "Types.h"
#include "intfOOVHandlerModule.hpp"
#include "Private/InputDecomposer/clsInput.h"

namespace Targoman{
namespace SMT {
namespace Private{
namespace SpecialTokenHandler {
namespace OOV{

using namespace RuleTable;
using namespace Common;

/**
 * @brief This type of Special OOV handler removes the OOV in the output but it doesn't ignore in decoding process.
 */

class RemoveOnTarget : public intfOOVHandlerModule{
public:
    explicit RemoveOnTarget():
        intfOOVHandlerModule(/*this->moduleName()*/) {}
    ~RemoveOnTarget();

    /**
     * @brief process       Sets type of OOV handling and returns an appropriate target rule.
     * @param[in] _token    Input string of token.
     * @param[out] _attrs   Type of  OOV handling will be stored in this argument.
     * @return              Returns an invalid target rule.
     */

    RuleTable::clsTargetRule process(const QString &_token, QVariantMap& _attrs){
        Q_UNUSED(_token)
        _attrs.insert(InputDecomposer::enuDefaultAttrs::toStr(InputDecomposer::enuDefaultAttrs::NoShow), true);
        //TODO: Check this invalid target rule whether it cause segmentation fault.
        return clsTargetRule();
    }

private:
    TARGOMAN_DEFINE_SINGLETON_MODULE(RemoveOnTarget);
};

/**
 * @brief This type of Special OOV handler ignores OOV word in decoding process.
 */

class RemoveDecoding : public intfOOVHandlerModule{
public:
    explicit RemoveDecoding():
        intfOOVHandlerModule(/*this->moduleName()*/) {}
    ~RemoveDecoding();

    /**
     * @brief process       Sets type of OOV handling and returns an appropriate target rule.
     * @param[in] _token    Input string of token.
     * @param[out] _attrs   Type of  OOV handling will be stored in this argument.
     * @return              Returns an invalid target rule.
     */

    RuleTable::clsTargetRule process(const QString &_token, QVariantMap& _attrs){
        Q_UNUSED(_token)

        _attrs.insert(InputDecomposer::enuDefaultAttrs::toStr(InputDecomposer::enuDefaultAttrs::NoDecode), true);
        // Note: input decomposer removes this token.
        return clsTargetRule();
    }

private:

    TARGOMAN_DEFINE_SINGLETON_MODULE(RemoveDecoding);
};

/**
 * @brief This type of Special OOV handler keeps OOV word as it is in the output.
 */

class KeepSource : public intfOOVHandlerModule{
public:
    explicit KeepSource():
        intfOOVHandlerModule(/*this->moduleName()*/) {}
    ~KeepSource();

    /**
     * @brief process       Sets type of OOV handling and returns an appropriate target rule.
     * @param[in] _token    Input string of token.
     * @param[out] _attrs   Type of  OOV handling will be stored in this argument.
     * @return              Returns an invalid target rule.
     */

    RuleTable::clsTargetRule process(const QString &_token, QVariantMap& _attrs){
        QList<WordIndex_t> TargetPhrase;
        TargetPhrase.append(gConfigs.EmptyLMScorer->getWordIndex(_token));
        _attrs.insert(
                    InputDecomposer::enuDefaultAttrs::toStr(InputDecomposer::enuDefaultAttrs::ShowSource),
                    true);
        return clsTargetRule::createZeroCostTargetRule(TargetPhrase, true);
    }

private:

    TARGOMAN_DEFINE_SINGLETON_MODULE(KeepSource);
};

}
}
}
}
}
#endif // TARGOMAN_SMT_PRIVATE_SPECIAL_TOKEN_HANDLER_OOV_OOVDEFAULTHANDLERS_H
