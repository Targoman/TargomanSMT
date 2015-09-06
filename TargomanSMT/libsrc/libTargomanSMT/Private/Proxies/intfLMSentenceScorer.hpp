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

#ifndef TARGOMAN_CORE_PRIVATE_PROXIES_LANGUAGEMODEL_INTFLMSENTENCESCORER_HPP
#define TARGOMAN_CORE_PRIVATE_PROXIES_LANGUAGEMODEL_INTFLMSENTENCESCORER_HPP

#include "libTargomanCommon/Types.h"
#include "libTargomanCommon/Configuration/intfConfigurable.hpp"
#include "libTargomanCommon/exTargomanBase.h"
#include "libTargomanCommon/Configuration/intfConfigurableModule.hpp"
#include "Private/GlobalConfigs.h"

namespace Targoman {
namespace SMT {
namespace Private {
/**
 *  @brief Namespace surrounding all classes and interfaces to external libraries
 */
namespace Proxies {

TARGOMAN_ADD_EXCEPTION_HANDLER(exRuleTable, exTargomanCore);

class intfLMSentenceScorer : public Common::Configuration::intfModule
{
public:
    intfLMSentenceScorer(const QString& _moduleName, quint64 _instanceID) :
        intfModule(_moduleName, _instanceID)
    {
        UnknownWordIndex = 0;
    }

    virtual ~intfLMSentenceScorer(){}

    virtual void init(bool _justVocab) = 0;
    virtual void initHistory(const intfLMSentenceScorer& _oldScorer) = 0;
    virtual void reset(bool _withStartOfSentence = true) = 0;
    virtual Common::LogP_t wordProb(const Common::WordIndex_t& _wordIndex) = 0;
    virtual Common::LogP_t endOfSentenceProb() = 0;
    virtual Common::WordIndex_t getWordIndex(const QString& _word) = 0;
    virtual QString getWordByIndex(Common::WordIndex_t _wordIndex) = 0;
    virtual bool haveSameHistoryAs(const intfLMSentenceScorer& _otherScorer) const = 0;
    virtual int compareHistoryWith(const intfLMSentenceScorer& _otherScorer) const = 0;

protected:
    Common::WordIndex_t UnknownWordIndex;

public:
    inline Common::WordIndex_t unknownWordIndex() const {
        return this->UnknownWordIndex;
    }
};

}
}
}
}
#endif // TARGOMAN_CORE_PRIVATE_PROXIES_LANGUAGEMODEL_INTFLMSENTENCESCORER_HPP
