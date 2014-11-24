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

clsProbingModel::clsProbingModel(/*clsVocab *_vocab*/) : clsBaseModel(enuMemoryModel::Probing/*, _vocab*/)
{
    this->SumLevels = 0;
}

void clsProbingModel::setUnknownWordDefaults(LogP_t _prob, LogP_t _backoff)
{
    /*NGram_t NGram;
    NGram.append(LM_UNKNOWN_WINDEX);
    this->insert(NGram, _prob, _backoff);*/
}

void clsProbingModel::insert(const char* _ngram, LogP_t _prob, LogP_t _backoff)
{
    Hash_t HashLoc, HashValue;
    size_t NGramLen = strlen(_ngram);
    HashLoc = (TargomanHasher::hashVal(_ngram, NGramLen, 0) % this->HashTableSize) + 1;;
    for (quint8 HashLevel = 0; HashLevel< 16; ++HashLevel)
    {
        HashValue = TargomanHasher::hashVal(_ngram, NGramLen, HashLevel) ;

        if (this->NGramHashTable[HashLoc].HashValueLevel){
            if (this->NGramHashTable[HashLoc].hashValue() == HashValue & 0xFFFFFFFFFFFFFFE0LL &&
                this->NGramHashTable[HashLoc].hashLevel() == HashLevel)
                throw exNgramManager(QString("Fatal Collision found on: %1 vs %5 (%2, %3, %4)").arg(
                                         _ngram).arg(
                                         HashLoc).arg(
                                         this->NGramHashTable[HashLoc].hashValue()).arg(
                                         HashLevel).arg(
                                         this->NGramHashTable[HashLoc].Original.c_str()));
            this->NGramHashTable[HashLoc].setContinues();
        }else{
            this->NGramHashTable[HashLoc].HashValueLevel = (HashValue & 0xFFFFFFFFFFFFFFE0LL) + HashLevel;
            this->NGramHashTable[HashLoc].Prob = _prob;
            this->NGramHashTable[HashLoc].Backoff = _backoff;
          //  this->NGramHashTable[HashLoc].Original = _ngram;

            this->MaxLevel = qMax(this->MaxLevel, ++HashLevel);
            this->SumLevels += HashLevel;

            return;
        }
        HashLoc = (HashValue % this->HashTableSize) + 1;
    }

    this->RemainingHashes.insert(_ngram, stuProbAndBackoffWeights(_prob,_backoff));
}

void clsProbingModel::initHashTable(quint32 _maxNGramCount)
{
    this->HashTableSize = 0;
    for (int i =0; i< sizeof(PrimeNumbers)/sizeof(quint32) - 1; i++)
        if (_maxNGramCount < PrimeNumbers[i]){
            this->HashTableSize = PrimeNumbers[i+1];
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
        if (PB.Prob  != 0){
            Prob = PB.Prob;
            Backoff = Constants::LogP_One;
        }

        if (++CurrGram >= _ngram.size()){
            break;
        }
        NGram = ((QStringList)_ngram.mid(_ngram.size() - CurrGram - 1)).join(" ");
        NGram2 = ((QStringList)_ngram.mid(_ngram.size() - CurrGram - 1, CurrGram)).join(" ");
        PB = this->lookupNGram(NGram2.toUtf8().constData());
        if (PB.Prob != 0){
            Backoff += PB.Backoff;
        }
    }
    _foundedGram = CurrGram - 1;
    return Prob + Backoff;
}

stuProbAndBackoffWeights clsProbingModel::lookupNGram(const char *_ngram) const
{
    Hash_t HashLoc, HashValue;
    size_t NGramLen = strlen(_ngram);

    HashLoc = (TargomanHasher::hashVal(_ngram, NGramLen, 0) % this->HashTableSize) + 1;;

    for (quint8 HashLevel = 0; HashLevel< 16; HashLevel++)
    {
        HashValue = TargomanHasher::hashVal(_ngram, NGramLen, HashLevel);

        if (this->NGramHashTable[HashLoc].hashValue() == HashValue & 0xFFFFFFFFFFFFFFE0LL &&
            this->NGramHashTable[HashLoc].hashLevel() == HashLevel)
            return stuProbAndBackoffWeights(
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
