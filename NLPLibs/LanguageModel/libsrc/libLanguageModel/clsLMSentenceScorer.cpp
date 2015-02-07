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

#include "clsLMSentenceScorer.h"
#include "Private/clsLMSentenceScorer_p.h"

namespace Targoman {
namespace NLPLibs {

using namespace Private;
using namespace Targoman::Common;

clsLMSentenceScorer::clsLMSentenceScorer(const clsLanguageModel &_lm, bool _stringBased) :
    pPrivate(new clsLMSentenceScorerPrivate(_lm))
{
}

clsLMSentenceScorer::~clsLMSentenceScorer()
{
    //Just to suppress Compiler error when using QScopped Poiter
}

void clsLMSentenceScorer::reset()
{
    this->pPrivate->StringBasedHistory.clear();
}

/**
 * @brief calculates word probability based on previous words.
 * @param _word         input word
 * @param _foundedGram  order of NGram that was existed in Hash Table.
 * @return              probablity of NGram.
 */

LogP_t clsLMSentenceScorer::wordProb(const QString& _word, quint8& _foundedGram)
{
    if (Q_UNLIKELY(this->pPrivate->StringBasedHistory.isEmpty())){
        this->pPrivate->StringBasedHistory.append(LM_BEGIN_SENTENCE);
    }else if (Q_LIKELY(this->pPrivate->StringBasedHistory.size() >= this->pPrivate->LM.order())){
        this->pPrivate->StringBasedHistory.removeFirst();
    }

    if (this->pPrivate->LM.getID(_word) == 0)
        this->pPrivate->StringBasedHistory.append(LM_UNKNOWN_WORD);
    else
        this->pPrivate->StringBasedHistory.append(_word);

    return this->pPrivate->LM.lookupNGram(this->pPrivate->StringBasedHistory, _foundedGram);
}

LogP_t clsLMSentenceScorer::wordProb(const WordIndex_t &_wordIndex, quint8& _foundedGram)
{
    if (Q_UNLIKELY(this->pPrivate->IndexBasedHistory.isEmpty())){
        this->pPrivate->IndexBasedHistory.append(LM_BEGIN_SENTENCE_WINDEX);
    }else if (Q_LIKELY(this->pPrivate->IndexBasedHistory.size() >= this->pPrivate->LM.order())){
        this->pPrivate->IndexBasedHistory.removeFirst();
    }

    this->pPrivate->IndexBasedHistory.append(_wordIndex);

    return this->pPrivate->LM.lookupNGram(this->pPrivate->IndexBasedHistory, _foundedGram);
}

void clsLMSentenceScorer::initHistory(const clsLMSentenceScorer &_oldScorer)
{
    this->pPrivate->StringBasedHistory = _oldScorer.pPrivate->StringBasedHistory;
    this->pPrivate->IndexBasedHistory = _oldScorer.pPrivate->IndexBasedHistory;
}


WordIndex_t clsLMSentenceScorer::wordIndex(const QString &_word)
{
    return this->pPrivate->LM.getID(_word);
}

clsLMSentenceScorerPrivate::~clsLMSentenceScorerPrivate()
{
    //Just to suppress compiler error using QScoppedPointer
}

}
}
