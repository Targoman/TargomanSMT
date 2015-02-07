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

#ifndef TARGOMAN_NLPLIBS_PRIVATE_CLSLMSENTENCESCORER_P_H
#define TARGOMAN_NLPLIBS_PRIVATE_CLSLMSENTENCESCORER_P_H

#include "../clsLanguageModel.h"
#include "clsLMSentenceScorer.h"

namespace Targoman {
namespace NLPLibs {
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
    const clsLanguageModel& LM;     				/**< An instance of  clsLanguageModel class */
    QStringList StringBasedHistory; 				/**< History of seen words */
    QList<Common::WordIndex_t> IndexBasedHistory;	/**< History of seen indices */
};

}
}
}

#endif // TARGOMAN_NLPLIBS_PRIVATE_CLSLMSENTENCESCORER_P_H
