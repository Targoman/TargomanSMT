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

#ifndef CLSLMSENTENCESCORER_H
#define CLSLMSENTENCESCORER_H

#include "clsLanguageModel.h"

namespace Targoman {
namespace NLPLibs {

namespace Private {
    class clsLMSentenceScorerPrivate;
}

class clsLMSentenceScorer
{
public:
    clsLMSentenceScorer(const clsLanguageModel& _lm);
    ~clsLMSentenceScorer();

    void reset();

    Common::LogP_t wordProb(const QString& _word, quint8 &_foundedGram);
    Common::LogP_t wordProb(const Common::WordIndex_t &_wordIndex, quint8 &_foundedGram);

private:
    QScopedPointer<Private::clsLMSentenceScorerPrivate> pPrivate;
};

}
}
#endif // CLSLMSENTENCESCORER_H
