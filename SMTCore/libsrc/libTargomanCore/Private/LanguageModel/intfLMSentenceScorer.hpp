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

namespace Targoman {
namespace Core {
namespace Private {
namespace LanguageModel{

TARGOMAN_ADD_EXCEPTION_HANDLER(exPhraseTable, exTargomanCore);

class intfLMSentenceScorer : public Common::Configuration::intfModule
{
public:
    virtual void reset() = 0;
    virtual Common::LogP_t wordProb(const QString& _word, OUTPUT quint8 &_foundedGram) = 0;
    virtual Common::LogP_t wordProb(const Targoman::Common::WordIndex_t &_wordIndex, OUTPUT quint8 &_foundedGram) = 0;
    virtual Common::WordIndex_t getWordIndex(const QString& _word) = 0;
};

}
}
}
}
#endif // TARGOMAN_CORE_PRIVATE_LANGUAGEMODEL_INTFLMSENTENCESCORER_HPP