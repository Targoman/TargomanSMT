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
 @author Behrooz Vedadian <vedadian@gmail.com>
 */

#include <stdlib.h>
#include "libTargomanCommon/Constants.h"
#include "libTargomanCommon/HashFunctions.hpp"
#include "libTargomanCommon/FastOperations.hpp"
#include "clsAbstractProbingModel.h"
#include "../Definitions.h"

using namespace Targoman::Common;

namespace Targoman {
namespace NLPLibs {
namespace Private {

clsAbstractProbingModel::clsAbstractProbingModel() : intfBaseModel(enuMemoryModel::Probing)
{
    this->SumLevels = 0;
    this->MaxLevel = 0;
}

void clsAbstractProbingModel::setUnknownWordDefaults(LogP_t _prob, LogP_t _backoff)
{
    this->UnknownWeights.Backoff = _backoff;
    this->UnknownWeights.Prob = _prob;
}

void clsAbstractProbingModel::insert(const char* _ngram, quint8 _order, LogP_t _prob, LogP_t _backoff)
{
    Hash_t HashLoc, HashValue;

    size_t NGramLen = strlen(_ngram);
    if (_order == 1 && !strcmp(_ngram, LM_UNKNOWN_WORD)){
        this->UnknownWeights.Backoff = _backoff;
        this->UnknownWeights.Prob = _prob;
        return;
    }

    HashLoc = (HashFunctions::murmurHash64(_ngram, NGramLen, 0) % this->HashTableSize) + 1;;
    for (quint8 HashLevel = 1; HashLevel< MAX_HASH_LEVEL; ++HashLevel)
    {
        HashValue = HashFunctions::murmurHash64(_ngram, NGramLen, HashLevel) ;

        if (this->NGramHashTable[HashLoc].HashValueLevel){
            if (this->NGramHashTable[HashLoc].hashValue() == (HashValue & HASHVALUE_CONTAINER) &&
                    this->NGramHashTable[HashLoc].hashLevel() == HashLevel-1)
                throw exLanguageModel(QString("Fatal Collision found on: %1 vs %5 (%2, %3, %4)").arg(
                                          _ngram).arg(
                                          HashLoc).arg(
                                          this->NGramHashTable[HashLoc].hashValue()).arg(
                                          HashLevel).arg(
                                          this->NGramHashTable[HashLoc].Original->c_str()));
            this->NGramHashTable[HashLoc].setContinues();
        }else{
            this->NGramHashTable[HashLoc].HashValueLevel = (HashValue & HASHVALUE_CONTAINER) + HashLevel-1;
            this->NGramHashTable[HashLoc].Prob      = _prob;
            this->NGramHashTable[HashLoc].Backoff   = _backoff;
            //            this->NGramHashTable[HashLoc].Original  = new std::string;
            //            *this->NGramHashTable[HashLoc].Original = _ngram;

            this->MaxLevel = qMax(this->MaxLevel, HashLevel);
            this->SumLevels += HashLevel;

            return;
        }
        HashLoc = (HashValue % this->HashTableSize) + 1;
    }

    this->RemainingHashes.insert(_ngram,
                                 stuProbAndBackoffWeights(this->RemainingHashes.size() + this->HashTableSize, _prob,_backoff));
}

void clsAbstractProbingModel::init(quint32 _maxNGramCount)
{
    this->HashTableSize = 0;
    for (size_t i =0; i< sizeof(TargomanGoodHashTableSizes)/sizeof(quint32) - 1; i++)
        if (_maxNGramCount < TargomanGoodHashTableSizes[i]){
            this->HashTableSize = TargomanGoodHashTableSizes[i+1];
            break;
        }

    if (this->HashTableSize){
        TargomanInfo(5, "Allocating "<<this->HashTableSize * sizeof(stuNGramHash)<<
                     " Bytes for max "<<this->HashTableSize<<" Items. Curr Items = "<<_maxNGramCount);
        this->NGramHashTable = new stuNGramHash[this->HashTableSize + 1];
        TargomanInfo(5, "Allocated");
        this->NgramCount = _maxNGramCount;
    }else
        throw exLanguageModel("Count of NGrams("+QString::number(_maxNGramCount)+") exeeds valid Table Sizes");
}




stuProbAndBackoffWeights clsAbstractProbingModel::getNGramWeights(const char *_ngram, bool _justSingle) const
{
    Hash_t HashLoc, HashValue;
    size_t NGramLen = strlen(_ngram);

    HashLoc = (HashFunctions::murmurHash64(_ngram, NGramLen, 0) % this->HashTableSize) + 1;;

    for (quint8 HashLevel = 1; HashLevel< MAX_HASH_LEVEL; ++HashLevel)
    {
        HashValue = HashFunctions::murmurHash64(_ngram, NGramLen, HashLevel);

        if (this->NGramHashTable[HashLoc].hashValue() == (HashValue & HASHVALUE_CONTAINER) &&
                this->NGramHashTable[HashLoc].hashLevel() == HashLevel-1){
            if (_justSingle && this->NGramHashTable[HashLoc].isMultiIndex())
                return UnknownWeights;
            else
                return stuProbAndBackoffWeights(
                            HashLoc,
                            this->NGramHashTable[HashLoc].Prob,
                            this->NGramHashTable[HashLoc].Backoff);
        }else if (!this->NGramHashTable[HashLoc].continues())
            return UnknownWeights;
        HashLoc = (HashValue % this->HashTableSize) + 1;
    }

    return this->RemainingHashes.value(_ngram, UnknownWeights);
}

}
}
}
