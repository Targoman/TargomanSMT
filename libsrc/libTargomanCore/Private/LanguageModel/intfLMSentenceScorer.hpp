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
 */

#ifndef TARGOMAN_CORE_PRIVATE_LANGUAGEMODEL_INTFLMSENTENCESCORER_HPP
#define TARGOMAN_CORE_PRIVATE_LANGUAGEMODEL_INTFLMSENTENCESCORER_HPP

#include "libTargomanCommon/Types.h"
#include "libTargomanCommon/Configuration.h"

namespace Targoman {
namespace Core {
namespace Private {
namespace LanguageModel{


TARGOMAN_ADD_EXCEPTION_HANDLER(exPhraseTable, exTargomanCore);

class intfLMSentenceScorer
{
public:
    virtual void reset() = 0;
    virtual Targoman::Common::LogP_t wordProb(const QString& _word, OUTPUT quint8 &_foundedGram) = 0;
    virtual Targoman::Common::LogP_t wordProb(const Targoman::Common::WordIndex_t &_wordIndex, OUTPUT quint8 &_foundedGram) = 0;

    static const QString& baseConfigPath(){return "LanguageModel";}
    static void  init(){
    }
    //static clsCon
};

}
}
}
}
#endif // TARGOMAN_CORE_PRIVATE_LANGUAGEMODEL_INTFLMSENTENCESCORER_HPP
