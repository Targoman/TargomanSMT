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

#ifndef TARGOMAN_CORE_PRIVATE_LANGUAGEMODEL_INTFLMSENTENCESCORER_HPP
#define TARGOMAN_CORE_PRIVATE_LANGUAGEMODEL_INTFLMSENTENCESCORER_HPP

#include "libTargomanCommon/Types.h"
#include "libTargomanCommon/Configuration/intfConfigurable.hpp"
#include "libTargomanCommon/exTargomanBase.h"
#include "libTargomanCommon/Configuration/intfConfigurableModule.hpp"
#include "Private/GlobalConfigs.h"

namespace Targoman {
namespace Core {
namespace Private {
namespace LanguageModel{

TARGOMAN_ADD_EXCEPTION_HANDLER(exPhraseTable, exTargomanCore);

class intfLMSentenceScorer : public Common::Configuration::intfModule
{
public:
    intfLMSentenceScorer(const QString& _name):
        intfModule(_name){}
    virtual void initHistory(const intfLMSentenceScorer& _oldScorer) = 0;
    virtual void reset() = 0;
    virtual Common::LogP_t wordProb(const QString& _word) = 0;
    virtual Common::LogP_t wordProb(const Common::WordIndex_t& _wordIndex) = 0;
    virtual Common::WordIndex_t getWordIndex(const QString& _word) = 0;

    virtual Common::WordIndex_t endOfSentence()= 0;
};

}
}
}
}
#endif // TARGOMAN_CORE_PRIVATE_LANGUAGEMODEL_INTFLMSENTENCESCORER_HPP
