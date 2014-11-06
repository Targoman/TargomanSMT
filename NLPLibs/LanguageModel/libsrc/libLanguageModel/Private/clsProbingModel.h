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

#ifndef TARGOMAN_NLPLIBS_PRIVATE_CLSPROBINGMODEL_H
#define TARGOMAN_NLPLIBS_PRIVATE_CLSPROBINGMODEL_H

#include "clsBaseModel.hpp"
#include "WeightStructs.hpp"
#include "../Definitions.h"

#include <QHash>

namespace Targoman {
namespace NLPLibs {
namespace Private {

class clsProbingModel : public clsBaseModel
{
public:
    clsProbingModel(clsVocab* _vocab);
    void    insert(const NGram_t &_ngram, float _prob, float _backoff = 0);
    float lookupNGram(NGram_t &_ngram) const;

private:
    QHash<NGram_t, stuProbAndBackoffWeights> LMData; // Choose other hashing mechanism
    stuProbAndBackoffWeights    UnknownWeights;
};

}
}
}

#endif // TARGOMAN_NLPLIBS_PRIVATE_CLSPROBINGMODEL_H
