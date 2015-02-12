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

#ifndef TARGOMAN_CORE_PRIVATE_LANGUAGEMODEL_CLSTARGOMANLMPROXY_HPP
#define TARGOMAN_CORE_PRIVATE_LANGUAGEMODEL_CLSTARGOMANLMPROXY_HPP

#include "intfLMSentenceScorer.hpp"
#include "libLanguageModel/clsLMSentenceScorer.h"
#include "libLanguageModel/clsLanguageModel.h"
#include "libTargomanCommon/Configuration/intfConfigurable.hpp"

namespace Targoman {
namespace Core {
namespace Private {
namespace LanguageModel{

class clsTargomanLMProxy : public intfLMSentenceScorer
{
public:
    clsTargomanLMProxy();
    ~clsTargomanLMProxy();
    inline void reset(){this->LMSentenceScorer->reset();}

    inline Common::LogP_t wordProb(const Common::WordIndex_t& _wordIndex) {
        quint8 Dummy;
        return this->LMSentenceScorer->wordProb(_wordIndex, Dummy);
    }

    inline Common::WordIndex_t getWordIndex(const QString& _word){return this->LM.getID(_word);}

    inline Common::WordIndex_t endOfSentence(){return this->LMSentenceScorer->endOfSentence();}
    inline void initHistory(const intfLMSentenceScorer& _oldScorer){
        this->LMSentenceScorer->initHistory(*(dynamic_cast<const clsTargomanLMProxy&>(_oldScorer).LMSentenceScorer));
    }

    bool haveSameHistoryAs(const intfLMSentenceScorer& _otherScorer) const{
        return this->LMSentenceScorer->haveSameHistoryAs(
                    *(dynamic_cast<const clsTargomanLMProxy&>(_otherScorer).LMSentenceScorer));
    }


private:
    static Targoman::NLPLibs::clsLanguageModel LM;
    QScopedPointer<Targoman::NLPLibs::clsLMSentenceScorer> LMSentenceScorer;
    TARGOMAN_DEFINE_MODULE("TargomanLM", clsTargomanLMProxy)
};

}
}
}
}

#endif // TARGOMAN_CORE_PRIVATE_LANGUAGEMODEL_CLSTARGOMANLMPROXY_HPP
