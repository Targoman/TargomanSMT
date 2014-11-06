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
 */

#include "clsProbingModel.h"
#include "../Definitions.h"

namespace Targoman {
namespace NLPLibs {
namespace Private {

clsProbingModel::clsProbingModel(clsVocab *_vocab) : clsBaseModel(enuMemoryModel::Probing, _vocab)
{
}

void clsProbingModel::insert(const NGram_t&_ngram, float _prob, float _backoff)
{
    this->LMData.insert(_ngram, stuProbAndBackoffWeights(_prob, _backoff));
    if (_ngram.size() == 1 && _ngram.at(0) == LM_UNKNOWN_WINDEX){
        this->UnknownWeights.Backoff = _backoff;
        this->UnknownWeights.Prob = _prob;
    }
}

float clsProbingModel::lookupNGram(NGram_t &_ngram) const
{
    Q_ASSERT(_ngram.size());

    stuProbAndBackoffWeights PB;
    LogP_t      Backoff = LogP_One;

    while (true){

        PB = this->LMData.value(_ngram);
        if (PB.Prob)
            return Backoff + PB.Prob;
        if (_ngram.size() == 1)
            return LogP_Zero;

        PB = this->LMData.value(_ngram.mid(0, _ngram.size() - 1));
        Backoff += PB.Backoff;
        _ngram.removeFirst();
    }
}

}
}
}
