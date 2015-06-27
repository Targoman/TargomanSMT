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

class clsOOVRemoveOnTarget : public intfOOVHandlerModule{
public:
    clsOOVRemoveOnTarget(quint64 _instanceID): intfOOVHandlerModule(this->moduleName(), _instanceID) {}
    ~clsOOVRemoveOnTarget();

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
    TARGOMAN_DEFINE_MODULE("OOVRemoveOnTarget", clsOOVRemoveOnTarget);
};

/**
 * @brief This type of Special OOV handler ignores OOV word in decoding process.
 */

class clsOOVRemoveDecoding : public intfOOVHandlerModule{
public:
    clsOOVRemoveDecoding(quint64 _instanceID): intfOOVHandlerModule(this->moduleName(), _instanceID) {}
    ~clsOOVRemoveDecoding();

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

    TARGOMAN_DEFINE_MODULE("OOVRemoveDecoding", clsOOVRemoveDecoding);
};

/**
 * @brief This type of Special OOV handler keeps OOV word as it is in the output.
 */

class clsOOVKeepSource : public intfOOVHandlerModule{
public:
    clsOOVKeepSource(quint64 _instanceID): intfOOVHandlerModule(this->moduleName(), _instanceID) {}
    ~clsOOVKeepSource();

    /**
     * @brief process       Sets type of OOV handling and returns an appropriate target rule.
     * @param[in] _token    Input string of token.
     * @param[out] _attrs   Type of  OOV handling will be stored in this argument.
     * @return              Returns an invalid target rule.
     */

    RuleTable::clsTargetRule process(const QString &_token, QVariantMap& _attrs){
        Q_UNUSED(_attrs);
        QList<WordIndex_t> TargetPhrase;
        TargetPhrase.append(gConfigs.EmptyLMScorer->getWordIndex(_token));
        return clsTargetRule::createZeroCostTargetRule(TargetPhrase, true);
    }

private:

    TARGOMAN_DEFINE_MODULE("OOVKeepSource", clsOOVKeepSource);
};

}
}
}
}
}
#endif // TARGOMAN_SMT_PRIVATE_SPECIAL_TOKEN_HANDLER_OOV_OOVDEFAULTHANDLERS_H
