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

#ifndef TARGOMAN_NLPLIBS_LANGUAGEMODEL_PRIVATE_CLSLMSENTENCESCORER_P_H
#define TARGOMAN_NLPLIBS_LANGUAGEMODEL_PRIVATE_CLSLMSENTENCESCORER_P_H

#include "../clsLanguageModel.h"
#include "clsLMSentenceScorer.h"

namespace Targoman {
namespace NLPLibs {
namespace TargomanLM {
namespace Private {


/**
 * @brief The main functionality of This class is for storing history of words.
 */

class clsLMSentenceScorerPrivate
{
public:
    clsLMSentenceScorerPrivate(const clsLanguageModel& _lm) : LM(_lm){}

    ~clsLMSentenceScorerPrivate();

public:
    const clsLanguageModel&    LM;     				/**< An instance of  clsLanguageModel class */
    QStringList                StringBasedHistory;  /**< History of seen words */
    QList<Common::WordIndex_t> IndexBasedHistory;	/**< History of seen indices */
    quint8                     FoundedGram;
};

}
}
}
}
#endif // TARGOMAN_NLPLIBS_LANGUAGEMODEL_PRIVATE_CLSLMSENTENCESCORER_P_H
