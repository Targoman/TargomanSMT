/*************************************************************************
 * Copyright © 2012-2014, Targoman.com
 *
 * Published under the terms of TCRL(Targoman Community Research License)
 * You can find a copy of the license file with distributed source or
 * download it from http://targoman.com/License.txt
 * 
 *************************************************************************/
/**
 @author S. Mohammad M. Ziabary <smm@ziabary.com>
 */

#include "clsLMSentenceScorer.h"
#include "Private/clsVocab.hpp"
#include "Private/clsLMSentenceScorer_p.h"

namespace Targoman {
namespace NLPLibs {

using namespace Private;

clsLMSentenceScorer::clsLMSentenceScorer(const clsLanguageModel &_lm) :
    pPrivate(new clsLMSentenceScorerPrivate(_lm))
{
}

clsLMSentenceScorer::~clsLMSentenceScorer()
{
    //Just to suppress Compiler error when using QScopped Poiter
}

float clsLMSentenceScorer::wordProb(const QString &_word)
{
    return this->wordProb(this->pPrivate->LM.getIndex(_word));
}

float clsLMSentenceScorer::wordProb(const WordIndex_t& _wordIndex)
{
    if (Q_UNLIKELY(this->pPrivate->History.isEmpty())){
        for(int i=0; i<this->pPrivate->LM.order(); i++){
            this->pPrivate->History.append(LM_BEGIN_SENTENCE_WINDEX);
        }
    }
    this->pPrivate->History.removeFirst();
    this->pPrivate->History.append(_wordIndex);

    return this->pPrivate->LM.lookupNGram(this->pPrivate->History);
}

}
}
