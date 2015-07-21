/******************************************************************************
 * Targoman: A robust Statistical Machine Translation framework               *
 *                                                                            *
 * Copyright 2014-2015 by ITRC <http://itrc.ac.ir>                            *
 *                                                                            *
 * This file is part of Targoman.                                             *
 *                                                                            *
 * Targoman is free software: you can redistribute it and/or modify           *
 * it under the terms of the GNU Lesser General Public License as published   *
 * by the Free Software Foundation, either version 3 of the License, or       *
 * (at your option) any later version.                                        *
 *                                                                            *
 * Targoman is distributed in the hope that it will be useful,                *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of             *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              *
 * GNU Lesser General Public License for more details.                        *
 * You should have received a copy of the GNU Lesser General Public License   *
 * along with Targoman. If not, see <http://www.gnu.org/licenses/>.           *
 *                                                                            *
 ******************************************************************************/
/**
 * @author S. Mohammad M. Ziabary <ziabary@targoman.com>
 * @author Behrooz Vedadian <vedadian@targoman.com>
 * @author Saeed Torabzadeh <saeed.torabzadeh@targoman.com>
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
    int ThisEffectiveLength = qMin(this->pPrivate->FoundedGram, (quint8)(this->pPrivate->LM.order() - 1));
    int OlderEffectiveLength = qMin(_oldScorer.pPrivate->FoundedGram, (quint8)(_oldScorer.pPrivate->LM.order() - 1));

    for (auto& item : this->pPrivate->IndexBasedHistory)
    {
        QString historyWord = this->pPrivate->LM.getWordByID(item);
    }
    for (auto& item : _oldScorer.pPrivate->IndexBasedHistory)
    {
        QString historyWord = _oldScorer.pPrivate->LM.getWordByID(item);
    }


    if (ThisEffectiveLength != OlderEffectiveLength)
        return false;

    if(Q_LIKELY(this->pPrivate->IndexBasedHistory.size()))
    {
        int ThisElementIndex = this->pPrivate->IndexBasedHistory.size() - 1;
        int OlderElementIndex = _oldScorer.pPrivate->IndexBasedHistory.size() - 1;
        int i = 0;
        while(i < ThisEffectiveLength && ThisElementIndex >= 0 && OlderElementIndex >= 0) {
            if(this->pPrivate->IndexBasedHistory.at(ThisElementIndex) !=
                    _oldScorer.pPrivate->IndexBasedHistory.at(OlderElementIndex))
                return false;
            ++i;
            --ThisElementIndex;
            --OlderElementIndex;
        }

    } else {

        int ThisElementIndex = this->pPrivate->StringBasedHistory.size() - 1;
        int OlderElementIndex = _oldScorer.pPrivate->StringBasedHistory.size() - 1;
        int i = 0;
        while(i < ThisEffectiveLength && ThisElementIndex >= 0 && OlderElementIndex >= 0) {
            if(this->pPrivate->StringBasedHistory.at(ThisElementIndex) !=
                    _oldScorer.pPrivate->StringBasedHistory.at(OlderElementIndex))
                return false;
            ++i;
            --ThisElementIndex;
            --OlderElementIndex;
        }
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
