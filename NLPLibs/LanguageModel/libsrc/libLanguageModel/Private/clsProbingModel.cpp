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

#include "libTargomanCommon/Constants.h"
#include "libTargomanCommon/HashFunctions.hpp"
#include "clsProbingModel.h"
#include "../Definitions.h"

using namespace Targoman::Common;

namespace Targoman {
namespace NLPLibs {
namespace Private {
const quint8 MAX_HASH_LEVEL = 32;
typedef quint64 Hash_t;

clsProbingModel::clsProbingModel() : clsBaseModel(enuMemoryModel::Probing)
{
    this->SumLevels = 0;
}

void clsProbingModel::setUnknownWordDefaults(LogP_t _prob, LogP_t _backoff)
{
    if (this->lookupNGram(LM_UNKNOWN_WORD).Prob == 0){
        this->insert(LM_UNKNOWN_WORD, _prob, _backoff);
    }
}

void clsProbingModel::insert(const char* _ngram, LogP_t _prob, LogP_t _backoff)
{
    Hash_t HashLoc, HashValue;
    size_t NGramLen = strlen(_ngram);
    HashLoc = (HashFunctions::murmurHash64(_ngram, NGramLen, 0) % this->HashTableSize) + 1;;
    for (quint8 HashLevel = 0; HashLevel< MAX_HASH_LEVEL; ++HashLevel)
    {
        HashValue = HashFunctions::murmurHash64(_ngram, NGramLen, HashLevel) ;

        if (this->NGramHashTable[HashLoc].HashValueLevel){
            if (this->NGramHashTable[HashLoc].hashValue() == (HashValue & 0xFFFFFFFFFFFFFFC0LL) &&
                this->NGramHashTable[HashLoc].hashLevel() == HashLevel)
                throw exLanguageModel(QString("Fatal Collision found on: %1 vs %5 (%2, %3, %4)").arg(
                                         _ngram).arg(
                                         HashLoc).arg(
                                         this->NGramHashTable[HashLoc].hashValue()).arg(
                                         HashLevel).arg(
                                         this->NGramHashTable[HashLoc].Original->c_str()));
            this->NGramHashTable[HashLoc].setContinues();
        }else{
            this->NGramHashTable[HashLoc].HashValueLevel = (HashValue & 0xFFFFFFFFFFFFFFC0LL) + HashLevel;
            this->NGramHashTable[HashLoc].Prob      = _prob;
            this->NGramHashTable[HashLoc].Backoff   = _backoff;
//            this->NGramHashTable[HashLoc].Original  = new std::string;
//            *this->NGramHashTable[HashLoc].Original = _ngram;

            this->MaxLevel = qMax(this->MaxLevel, ++HashLevel);
            this->SumLevels += HashLevel;

            return;
        }
        HashLoc = (HashValue % this->HashTableSize) + 1;
    }

    this->RemainingHashes.insert(_ngram,
                                 stuProbAndBackoffWeights(this->RemainingHashes.size() + this->HashTableSize, _prob,_backoff));
}

void clsProbingModel::init(quint32 _maxNGramCount)
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
        this->NGramHashTable = new stuNGramHash[this->HashTableSize];
        TargomanInfo(5, "Allocated");
        this->NgramCount = _maxNGramCount;
    }else
        throw exLanguageModel("Count of NGrams("+QString::number(_maxNGramCount)+") exeeds valid Table Sizes");
}

LogP_t clsProbingModel::lookupNGram(const QStringList& _ngram, quint8& _foundedGram) const
{
    Q_ASSERT(_ngram.size());

    stuProbAndBackoffWeights PB;
    LogP_t      Backoff = Constants::LogP_One;
    LogP_t      Prob = Constants::LogP_Zero;
    QString     NGram = _ngram.last();
    QString     NGram2 = _ngram.last();
    quint8      CurrGram = 0;

    while (true){
        PB = this->lookupNGram(NGram.toUtf8().constData());
        if (PB.ID != 0){
            Prob = PB.Prob;
            Backoff = Constants::LogP_One;
            _foundedGram = CurrGram+1;
        }

        if (++CurrGram >= _ngram.size()){
            break;
        }
        NGram = ((QStringList)_ngram.mid(_ngram.size() - CurrGram - 1)).join(" ");
        NGram2 = ((QStringList)_ngram.mid(_ngram.size() - CurrGram - 1, CurrGram)).join(" ");
        PB = this->lookupNGram(NGram2.toUtf8().constData());
        if (PB.ID != 0){
            Backoff += PB.Backoff;
        }
    }
    return Prob + Backoff;
}

stuProbAndBackoffWeights clsProbingModel::lookupNGram(const char *_ngram) const
{
    Hash_t HashLoc, HashValue;
    size_t NGramLen = strlen(_ngram);

    HashLoc = (HashFunctions::murmurHash64(_ngram, NGramLen, 0) % this->HashTableSize) + 1;;

    for (quint8 HashLevel = 0; HashLevel< MAX_HASH_LEVEL; HashLevel++)
    {
        HashValue = HashFunctions::murmurHash64(_ngram, NGramLen, HashLevel);

        if (this->NGramHashTable[HashLoc].hashValue() == (HashValue & 0xFFFFFFFFFFFFFFC0LL) &&
            this->NGramHashTable[HashLoc].hashLevel() == HashLevel)
            return stuProbAndBackoffWeights(
                        HashLoc,
                        this->NGramHashTable[HashLoc].Prob,
                        this->NGramHashTable[HashLoc].Backoff);
        else if (!this->NGramHashTable[HashLoc].continues())
            return stuProbAndBackoffWeights();
        HashLoc = (HashValue % this->HashTableSize) + 1;
    }

    return this->RemainingHashes.value(_ngram);
}

}
}
}
