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

#ifndef TARGOMAN_NLPLIBS_LANGUAGEMODEL_CLSLMSENTENCESCORER_H
#define TARGOMAN_NLPLIBS_LANGUAGEMODEL_CLSLMSENTENCESCORER_H

#include "clsLanguageModel.h"

namespace Targoman {
namespace NLPLibs {
namespace TargomanLM {

namespace Private {
    class clsLMSentenceScorerPrivate;
}

/**
 * @brief This class manages history of sentence itself and gives score of a sentence.
 */

class clsLMSentenceScorer
{
public:
    clsLMSentenceScorer(const clsLanguageModel& _lm);
    ~clsLMSentenceScorer();

    void reset(bool _withStartOfSentence = true);

    Common::LogP_t wordProb(const QString& _word, OUTPUT quint8 &_foundedGram);
    Common::LogP_t wordProb(const Common::WordIndex_t& _wordIndex, OUTPUT quint8& _foundedGram);
    Common::LogP_t endOfSentenceProb(quint8 &_foundedGram);
    Common::WordIndex_t wordIndex(const QString& _word);
    void initHistory(const clsLMSentenceScorer& _oldScorer);
    bool haveSameHistoryAs(const clsLMSentenceScorer& _oldScorer);

private:
    QScopedPointer<Private::clsLMSentenceScorerPrivate> pPrivate;
};

}
}
}
#endif // TARGOMAN_NLPLIBS_LANGUAGEMODEL_CLSLMSENTENCESCORER_H
