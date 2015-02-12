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

class clsTargomanLMProxy :
        public intfLMSentenceScorer, public Targoman::NLPLibs::clsLMSentenceScorer{
public:
    clsTargomanLMProxy();
    inline void reset(){Targoman::NLPLibs::clsLMSentenceScorer::reset();}

    inline Common::LogP_t wordProb(const Common::WordIndex_t& _wordIndex) {
        quint8 Dummy;
        return Targoman::NLPLibs::clsLMSentenceScorer::wordProb(_wordIndex, Dummy);
    }

    inline Common::WordIndex_t getWordIndex(const QString& _word){return this->LM.getID(_word);}

    inline Common::WordIndex_t endOfSentence(){return Targoman::NLPLibs::clsLMSentenceScorer::endOfSentence();}
    virtual void initHistory(const intfLMSentenceScorer& _oldScorer){
        //TODO
    }

    bool haveSameHistoryAs(const intfLMSentenceScorer& _otherScorer) const{
        //TODO
    }


private:
    static Targoman::NLPLibs::clsLanguageModel LM;

    TARGOMAN_DEFINE_MODULE("TargomanLM", clsTargomanLMProxy)
};

}
}
}
}

#endif // TARGOMAN_CORE_PRIVATE_LANGUAGEMODEL_CLSTARGOMANLMPROXY_HPP
