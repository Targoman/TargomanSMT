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
 @author Behrooz Vedadian <vedadian@gmail.com>
 */

#include "clsLMSentenceScorer.h"
//#include "Private/clsVocab.hpp"
#include "Private/clsLMSentenceScorer_p.h"

namespace Targoman {
namespace NLPLibs {

using namespace Private;
using namespace Targoman::Common;

clsLMSentenceScorer::clsLMSentenceScorer(const clsLanguageModel &_lm) :
    pPrivate(new clsLMSentenceScorerPrivate(_lm))
{
}

clsLMSentenceScorer::~clsLMSentenceScorer()
{
    //Just to suppress Compiler error when using QScopped Poiter
}

void clsLMSentenceScorer::reset()
{
    this->pPrivate->History.clear();
}

LogP_t clsLMSentenceScorer::wordProb(const QString& _word, quint8& _foundedGram)
{
        if (Q_UNLIKELY(this->pPrivate->History.isEmpty())){
                this->pPrivate->History.append(LM_BEGIN_SENTENCE);
        }else if (Q_LIKELY(this->pPrivate->History.size() >= this->pPrivate->LM.order())){
            this->pPrivate->History.removeFirst();
        }

        if (this->pPrivate->LM.getID(_word) == 0)
            this->pPrivate->History.append(LM_UNKNOWN_WORD);
        else
            this->pPrivate->History.append(_word);

        return this->pPrivate->LM.lookupNGram(this->pPrivate->History, _foundedGram);
}

}
}
