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
  @author Behrooz Vedadian <vedadian@aut.ac.ir>
 */

#include "libTargomanCommon/Constants.h"
#include "clsProbingModel.h"
#include "../Definitions.h"

using namespace Targoman::Common;
namespace Targoman {
namespace NLPLibs {
namespace Private {

clsProbingModel::clsProbingModel(clsVocab *_vocab) : clsBaseModel(enuMemoryModel::Probing, _vocab)
{
}

void clsProbingModel::setUnknownWordDefaults(LogP_t _prob, LogP_t _backoff)
{
    QList<WordIndex_t> NGram;
    NGram.append(LM_UNKNOWN_WINDEX);
    this->insert(NGram, _prob, _backoff);
}

void clsProbingModel::insert(const NGram_t&_ngram, float _prob, float _backoff)
{
    this->LMData.insert(_ngram, stuProbAndBackoffWeights(_prob, _backoff));
}

LogP_t clsProbingModel::lookupNGram(const NGram_t &_ngram, quint8& _foundedGram) const
{
    Q_ASSERT(_ngram.size());

    stuProbAndBackoffWeights PB;
    LogP_t      Backoff = Constants::LogP_One;
    NGram_t NGram = _ngram;

    while (true){
        PB = this->LMData.value(NGram);
        if (PB.Prob){
            _foundedGram = NGram.size();
            return Backoff + PB.Prob;
        }
        if (NGram.size() == 1){
            _foundedGram = 0;
            return Constants::LogP_Zero;
        }

        PB = this->LMData.value(NGram.mid(0, NGram.size() - 1));
        Backoff += PB.Backoff;
        NGram.removeFirst();
    }
}

}
}
}
