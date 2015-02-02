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

#include "clsStringBasedProbingModel.h"
#include "libTargomanCommon/Constants.h"

namespace Targoman {
namespace NLPLibs {
namespace Private {

using namespace Common;

LogP_t clsStringBasedProbingModel::lookupNGram(const QStringList& _ngram, quint8& _foundedGram) const
{
    Q_ASSERT(_ngram.size());

    stuProbAndBackoffWeights PB;
    LogP_t      Backoff = Constants::LogP_One;
    LogP_t      Prob = Constants::LogP_Zero;
    QString     NGram = _ngram.last();
    QString     NGram2 = _ngram.last();
    quint8      CurrGram = 0;

    while (true){
        PB = this->getNGramWeights(NGram.toUtf8().constData());
        if (PB.ID > 0){
            Prob = PB.Prob;
            Backoff = Constants::LogP_One;
            _foundedGram = CurrGram+1;
        }

        if (++CurrGram >= _ngram.size()){
            break;
        }
        NGram = ((QStringList)_ngram.mid(_ngram.size() - CurrGram - 1)).join(" ");
        NGram2 = ((QStringList)_ngram.mid(_ngram.size() - CurrGram - 1, CurrGram)).join(" ");
        PB = this->getNGramWeights(NGram2.toUtf8().constData());
        if (PB.ID > 0){
            Backoff += PB.Backoff;
        }
    }
    return Prob + Backoff;
}

}
}
}
