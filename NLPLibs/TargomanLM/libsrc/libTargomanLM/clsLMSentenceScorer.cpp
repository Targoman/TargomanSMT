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
namespace TargomanLM {

using namespace Private;
using namespace Targoman::Common;

clsLMSentenceScorer::clsLMSentenceScorer(const clsLanguageModel &_lm) :
    pPrivate(new clsLMSentenceScorerPrivate(_lm))
{
    this->reset();
}

clsLMSentenceScorer::~clsLMSentenceScorer()
{
    //Just to suppress Compiler error when using QScopped Poiter
}

/**
 * @brief clears string and index based history list.
 */

void clsLMSentenceScorer::reset(bool _withStartOfSentence)
{
    this->pPrivate->StringBasedHistory.clear();
    this->pPrivate->IndexBasedHistory.clear();

    if (_withStartOfSentence){
        this->pPrivate->FoundedGram = 1;
        this->pPrivate->StringBasedHistory.append(LM_BEGIN_SENTENCE);
        this->pPrivate->IndexBasedHistory.append(LM_BEGIN_SENTENCE_WINDEX);
    }else
        this->pPrivate->FoundedGram = 0;
}

/**
 * @brief calculates word probability using previous words (history) and input word .
 * @param _word         input word string
 * @param _foundedGram  order of NGram that was existed in Hash Table.
 * @return              probablity of NGram.
 */

LogP_t clsLMSentenceScorer::wordProb(const QString& _word, quint8& _foundedGram)
{
    if (this->pPrivate->LM.getID(_word) == 0 ||
        _word == LM_BEGIN_SENTENCE ||
        _word == LM_END_SENTENCE)
        this->pPrivate->StringBasedHistory.append(LM_UNKNOWN_WORD);
    else
        this->pPrivate->StringBasedHistory.append(_word);

    LogP_t Prob = this->pPrivate->LM.lookupNGram(this->pPrivate->StringBasedHistory, _foundedGram);
    if (Q_LIKELY(this->pPrivate->StringBasedHistory.size() >= this->pPrivate->LM.order()))
        this->pPrivate->StringBasedHistory.removeFirst();
    this->pPrivate->FoundedGram = _foundedGram;
    return Prob;
}

/**
 * @brief calculates word probability using previous words (history) and input word .
 * @param _word         input word index
 * @param _foundedGram  order of NGram that was existed in Hash Table.
 * @return              probablity of NGram.
 */
LogP_t clsLMSentenceScorer::wordProb(const WordIndex_t &_wordIndex, quint8& _foundedGram)
{
    if (_wordIndex == LM_BEGIN_SENTENCE_WINDEX ||
        _wordIndex == LM_END_SENTENCE_WINDEX)
        this->pPrivate->IndexBasedHistory.append(LM_UNKNOWN_WINDEX);
    else
        this->pPrivate->IndexBasedHistory.append(_wordIndex);

    LogP_t Prob = this->pPrivate->LM.lookupNGram(this->pPrivate->IndexBasedHistory, _foundedGram);
    if (Q_LIKELY(this->pPrivate->IndexBasedHistory.size() >= this->pPrivate->LM.order()))
        this->pPrivate->IndexBasedHistory.removeFirst();
    this->pPrivate->FoundedGram = _foundedGram;
    return Prob;
}

LogP_t clsLMSentenceScorer::endOfSentenceProb(quint8& _foundedGram)
{
    if (this->pPrivate->IndexBasedHistory.size() > 1){
        this->pPrivate->IndexBasedHistory.append(LM_END_SENTENCE_WINDEX);
        return this->pPrivate->LM.lookupNGram(this->pPrivate->IndexBasedHistory, _foundedGram);
    }else{
        this->pPrivate->StringBasedHistory.append(LM_END_SENTENCE);
        return this->pPrivate->LM.lookupNGram(this->pPrivate->StringBasedHistory, _foundedGram);
    }
}

/**
 * @brief Initializes history of language model with history of input sentence scorer.
 * @param _oldScorer input sentence scorer.
 */

void clsLMSentenceScorer::initHistory(const clsLMSentenceScorer &_oldScorer)
{
    this->pPrivate->StringBasedHistory = _oldScorer.pPrivate->StringBasedHistory;
    this->pPrivate->IndexBasedHistory = _oldScorer.pPrivate->IndexBasedHistory;
}

/**
 * @brief checks wethere our sentence scorer and input scorer have same history list or not.
 * @param _oldScorer input sentence scorer.
 */

bool clsLMSentenceScorer::haveSameHistoryAs(const clsLMSentenceScorer &_oldScorer)
{
    if (this->pPrivate->IndexBasedHistory  == _oldScorer.pPrivate->IndexBasedHistory &&
        this->pPrivate->StringBasedHistory == _oldScorer.pPrivate->StringBasedHistory)
        return true;

    size_t MyEffectiveLenght = qMin(this->pPrivate->FoundedGram, (quint8)(this->pPrivate->LM.order() - 1));
    size_t OtherEffectiveLenght = qMin(_oldScorer.pPrivate->FoundedGram, (quint8)(_oldScorer.pPrivate->LM.order() - 1));

    if (MyEffectiveLenght != OtherEffectiveLenght)
        return false;

    if(this->pPrivate->IndexBasedHistory.size()){
        for(size_t i=0; i< MyEffectiveLenght; ++i)
            if(this->pPrivate->IndexBasedHistory.at(i) != _oldScorer.pPrivate->IndexBasedHistory.at(i))
                return false;
    }else{
        for(size_t i=0; i< MyEffectiveLenght; ++i)
            if(this->pPrivate->StringBasedHistory.at(i) != _oldScorer.pPrivate->StringBasedHistory.at(i))
                return false;
    }
    return true;
}

/**
 * @brief returns word index of input word string.
 * @param _word
 * @return
 */

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
}
