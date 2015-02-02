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
    inline Common::LogP_t wordProb(const QString& _word, OUTPUT quint8 &_foundedGram){
        Targoman::NLPLibs::clsLMSentenceScorer::wordProb(_word,_foundedGram);
    }

    inline Common::WordIndex_t getWordIndex(const QString& _word){return this->LM.getID(_word);}

private:
    static Targoman::NLPLibs::clsLanguageModel LM;

    TARGOMAN_DEFINE_MODULE("TargomanLM", clsTargomanLMProxy)
};

TARGOMAN_REGISTER_MODULE(clsTargomanLMProxy)
}
}
}
}

#endif // TARGOMAN_CORE_PRIVATE_LANGUAGEMODEL_CLSTARGOMANLMPROXY_HPP
