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

#include "clsIndexBasedProbingModel.h"
#include "libTargomanCommon/HashFunctions.hpp"
#include "libTargomanCommon/FastOperations.hpp"
#include "libTargomanCommon/Types.h"
#include "libTargomanCommon/Constants.h"

namespace Targoman {
namespace NLPLibs {
namespace Private {

using namespace Common;

void clsIndexBasedProbingModel::insert(const char* _ngram, quint8 _order, LogP_t _prob, LogP_t _backoff)
{
    if (_order > 1){
        QList<WordIndex_t> WIndexSentence;
        const char* NGramStrBegin = _ngram;
        const char* NGramStrEnd = NGramStrBegin;
        char LastChar;
        while(*NGramStrBegin){
            NGramStrEnd = Common::fastSkip2Space(NGramStrEnd);
            LastChar = *((char*)NGramStrEnd);
            *((char*)NGramStrEnd) = (char)NULL;
            WordIndex_t WordIndex = this->getID(NGramStrBegin);
            if (WordIndex == 0 && strcmp(NGramStrBegin,LM_UNKNOWN_WORD) )
                throw exLanguageModel(QString(NGramStrBegin) + " Not Found in vocab");
            WIndexSentence.append(WordIndex);
            *((char*)NGramStrEnd) = LastChar;
            NGramStrBegin = Common::fastSkip2NonSpace(NGramStrEnd);
            NGramStrEnd = NGramStrBegin;
        }
        this->insert(WIndexSentence, _prob, _backoff);
    }else{
        clsAbstractProbingModel::insert(_ngram, _order, _prob, _backoff);
    }
}

LogP_t clsIndexBasedProbingModel::lookupNGram(const QList<WordIndex_t> &_ngram, quint8 &_foundedGram) const
{
    Q_ASSERT(_ngram.size());

    stuProbAndBackoffWeights PB;
    LogP_t      Backoff = Constants::LogP_One;
    LogP_t      Prob = Constants::LogP_Zero;
    QList<WordIndex_t>     NGram;
    NGram.append(_ngram.last());
    QList<WordIndex_t>     NGram2;
    quint8      CurrGram = 0;

    while (true){
        PB = this->getNGramWeights(NGram);
        if (PB.ID > 0){
            Prob = PB.Prob;
            Backoff = Constants::LogP_One;
            _foundedGram = CurrGram+1;
        }

        if (++CurrGram >= _ngram.size()){
            break;
        }
        NGram = _ngram.mid(_ngram.size() - CurrGram - 1);
        NGram2 = _ngram.mid(_ngram.size() - CurrGram - 1, CurrGram);
        PB = this->getNGramWeights(NGram2);
        if (PB.ID > 0){
            Backoff += PB.Backoff;
        }
    }
    return Prob + Backoff;
}

LogP_t clsIndexBasedProbingModel::lookupNGram(const QStringList &_ngram, quint8 &_foundedGram) const
{
    QList<WordIndex_t> NGram;
    foreach (const QString& Word, _ngram)
        NGram.append(this->getID(Word.toUtf8().constData()));

    return lookupNGram(NGram, _foundedGram);
}

void clsIndexBasedProbingModel::insert(QList<WordIndex_t> _ngram, LogP_t _prob, LogP_t _backoff)
{
    Hash_t HashLoc, HashValue;

    if (_ngram.size() < 2)
        throw exLanguageModel("Seems buggy as input _ngram has just one Item", __LINE__);

    HashLoc = (HashFunctions::murmurHash64(_ngram, 0) % this->HashTableSize) + 1;;
    for (quint8 HashLevel = 1; HashLevel< MAX_HASH_LEVEL; ++HashLevel) {
        HashValue = HashFunctions::murmurHash64(_ngram, HashLevel) ;

        if (this->NGramHashTable[HashLoc].HashValueLevel){
            if (this->NGramHashTable[HashLoc].hashValue() == (HashValue & HASHVALUE_CONTAINER) &&
                this->NGramHashTable[HashLoc].hashLevel() == HashLevel-1)
                throw exLanguageModel(QString("Fatal Collision found on: %1 (%2, %3, %4)").arg(
                                         "ListOfWIndexes").arg(
                                         HashLoc).arg(
                                         this->NGramHashTable[HashLoc].hashValue()).arg(
                                         HashLevel));
            this->NGramHashTable[HashLoc].setContinues();
        }else{
            this->NGramHashTable[HashLoc].HashValueLevel = (HashValue & HASHVALUE_CONTAINER) + HashLevel-1;
            this->NGramHashTable[HashLoc].Prob      = _prob;
            this->NGramHashTable[HashLoc].Backoff   = _backoff;
            this->NGramHashTable[HashLoc].setMultiIndex();

            this->MaxLevel = qMax(this->MaxLevel, HashLevel);
            this->SumLevels += HashLevel;

            return;
        }
        HashLoc = (HashValue % this->HashTableSize) + 1;
    }

    QString NGramStr;
    foreach (WordIndex_t WIndex, _ngram)
        NGramStr+=QString::number(WIndex) + " ";

    this->RemainingHashes.insert(NGramStr.trimmed().toLatin1().constData(),
                                 stuProbAndBackoffWeights(this->RemainingHashes.size() + this->HashTableSize,
                                                          _prob,_backoff));

}

stuProbAndBackoffWeights clsIndexBasedProbingModel::getNGramWeights(QList<WordIndex_t> _ngram) const
{
    Hash_t HashLoc, HashValue;

    HashLoc = (HashFunctions::murmurHash64(_ngram, 0) % this->HashTableSize) + 1;;

    for (quint8 HashLevel = 1; HashLevel< MAX_HASH_LEVEL; HashLevel++)
    {
        HashValue = HashFunctions::murmurHash64(_ngram, HashLevel);

        if (this->NGramHashTable[HashLoc].hashValue() == (HashValue & HASHVALUE_CONTAINER) &&
                this->NGramHashTable[HashLoc].hashLevel() == HashLevel-1){
                return stuProbAndBackoffWeights(
                            HashLoc,
                            this->NGramHashTable[HashLoc].Prob,
                            this->NGramHashTable[HashLoc].Backoff);
        }else if (!this->NGramHashTable[HashLoc].continues())
            return UnknownWeights;
        HashLoc = (HashValue % this->HashTableSize) + 1;
    }

    QString NGramStr;
    foreach (WordIndex_t WIndex, _ngram)
        NGramStr+=QString::number(WIndex) + " ";

    return this->RemainingHashes.value(NGramStr.trimmed().toLatin1().constData(), UnknownWeights);
}

}
}
}
