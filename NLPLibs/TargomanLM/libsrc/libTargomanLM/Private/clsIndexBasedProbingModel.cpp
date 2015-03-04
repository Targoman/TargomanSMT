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
namespace TargomanLM {
namespace Private {

using namespace Common;
/**
 * @brief insert string ngram by there word index in hash table
 *
 * This function first checks whether order of input ngram is 1 or not.
 * If order is 1 (unigram) it inserts it directly without converting it to word index.
 * If order is greater than 1, splits space delimitted input string to words and for each word calculates its word index in Hash Table.
 * These word indices will be added to a vector of word indices.
 * Finally this wordIndex vector, probability and backoff weight of input NGram will be inserted to Hash Table.
 *
 * @param _ngram    input NGram.
 * @param _order    order of NGram.
 * @param _prob     probability of NGram.
 * @param _backoff  backoff weight of NGram.
 */
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

/**
 * @brief returns probablity of input NGram and maximum order of founded NGram.
 *
 * This function calculates and retrieves probablity of input NGram using maximum founded order of existed NGram in language model and higher order backoff weights.
 * Here is an example of calculating a 5 token input string:
 * Suppose our input string is w1,w2,w3,w4,w5 and we want to calculate the probablity of this string in our 5 Gram language model.
 * First we check whether w1,w2,w3,w4,w5 string exist in language model or not. if it exists the probablity of that returns out.
 * If it doesn't exist but w2,w3,w4,w5 exists, the backoff weight of w1,w2,w3,w4 adds with log probablity of w2,w3,w4,w5.
 * If both w1,w2,w3,w4,w5 and w2,s3,w4,w5 doesn't exist but w3,w4,w5 exists, backoff weight of w1,w2,w3,w4 and w2,w3,w4 and log probablity of w3,w4,w5 adds together, and so on.
 * The maximum order of NGram that is founded in language model and sum of log probablity of founded NGram and backoff of higher order NGram will be the return value of this function.
 * Note that This function calculates this process from unigram to higher order grams.
 *
 *
 * @param[in] _ngram            input NGram in a vector of word index.
 * @param[out] _foundedGram     maximum order of NGram that was existed in Hash Table.
 * @return                      returns probablity of input NGram.
 */

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
    _foundedGram = 1;

    while (true){
        PB = this->getNGramWeights(NGram);
        if (PB.ID > 0 || NGram.first() == LM_UNKNOWN_WINDEX){
            Prob = PB.Prob;
            Backoff = Constants::LogP_One; // backoff weight of higher order NGram is needed, so previously calculated backoffs should be reset to zero.
            _foundedGram = CurrGram+1;
        }

        if (++CurrGram >= _ngram.size()){
            break;
        }
        NGram = _ngram.mid(_ngram.size() - CurrGram - 1);               // This increases order of gram in each step
        NGram2 = _ngram.mid(_ngram.size() - CurrGram - 1, CurrGram);    // This is history of NGram. Lambda of backoffs are based on history string.
        PB = this->getNGramWeights(NGram2);
        if (PB.ID > 0){
            Backoff += PB.Backoff;
        }
    }
    return Prob + Backoff;
}

/**
 * @brief returns probablity of input NGram and maximum order of founded NGram.
 * @param[in] _ngram        input NGram in QStringList format.
 * @param[out] _foundedGram maximum order of NGram that was existed in Hash Table.
 * @return                  returns probablity of input NGram.
 */

LogP_t clsIndexBasedProbingModel::lookupNGram(const QStringList &_ngram, quint8 &_foundedGram) const
{
    QList<WordIndex_t> NGram;
    foreach (const QString& Word, _ngram)
        NGram.append(this->getID(Word.toUtf8().constData()));

    return lookupNGram(NGram, _foundedGram);
}

/**
 * @brief inserts multiple wordIndex  to HashTable
 *
 * This function first, finds the location for inserting NGram in hash table using sufficient needed level of hashing.
 * Algorithm of this function is in this way:
 * computes its first guess for correct place to insert using murmurHash of string value.
 * While this guessed location is occupied, it guesse another location using higher level of hashing.
 * If after 32 guesses it could not find an empty place for locating it, this string will be inserted in #RemainingHashes which is a QHash variable.
 * When empty place for inserting input NGram found in level "x", hash value of input NGram in level "x+1" and its hash level will be inserted in that location.
 * When a guessed place is occupied, its continue flag will be set.
 * Each cells in hash table is of type stuNGramHash. stuNGramHash have 64 bit variable called stuNGramHash::#HashValueLevel.
 * The first lowest 5 bits of this variable, is for recording hash level (which can save a level between 0-31).
 * The Sixth bit of this variable is for continue flag (which specifies whether another value in hash table, have guessed this occupied place for its location or not).
 * The Seven bit of this variable is for multiIndex flag (which specifies whether value of cell is hash of a multi gram or unigram).
 * The remaining 57 bits of this variable is for recording hash value of Ngram.
 * probability and backoff weight of input NGram will be also recorded in seperate variables, for each cell.
 * QList size should be greater than 1. So, when we succefully find correct place for insertion, we should always set multiIndex flag.
 *
 * @param[in] _ngram    a QList of type WordIndex_t. QList size should be greater than 1.
 * @param[in] _prob     probablity of NGram
 * @param[in] _backoff  weight of backoff
 */

void clsIndexBasedProbingModel::insert(QList<WordIndex_t> _ngram, LogP_t _prob, LogP_t _backoff)
{
    Hash_t HashLoc, HashValue;
    if (_ngram.size() < 2)
        throw exLanguageModel("Seems buggy as input _ngram has just one Item", __LINE__);

    HashLoc = (HashFunctions::murmurHash64(_ngram, 0) % this->HashTableSize) + 1;;
    for (quint8 HashLevel = 1; HashLevel< MAX_HASH_LEVEL; ++HashLevel) {
        HashValue = HashFunctions::murmurHash64(_ngram, HashLevel) ;

        if (this->NGramHashTable[HashLoc].HashValueLevel){
            if (this->NGramHashTable[HashLoc].hashValue() == this->getHashValue(HashValue) &&
                this->NGramHashTable[HashLoc].hashLevel() == HashLevel-1)
                throw exLanguageModel(QString("Fatal Collision found on: %1 (%2, %3, %4)").arg(
                                         "ListOfWIndexes").arg(
                                         HashLoc).arg(
                                         this->NGramHashTable[HashLoc].hashValue()).arg(
                                         HashLevel));
            this->NGramHashTable[HashLoc].setContinues();
        }else{
            this->NGramHashTable[HashLoc].HashValueLevel = this->getHashValue(HashValue) + HashLevel-1;
            this->NGramHashTable[HashLoc].Prob      = _prob;
            this->NGramHashTable[HashLoc].Backoff   = _backoff;
            this->NGramHashTable[HashLoc].setMultiIndex();

            this->MaxLevel = qMax(this->MaxLevel, HashLevel);
            this->SumLevels += HashLevel;
            ++this->StoredInHashTable;

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

/**
 * @brief Retrieves probability, backoff weight and ID of input NGram in Hash Table.
 * @param[in] _ngram        input NGram in a QList of type WordIndex_t.t.
 * @return returns probability, backoff weight and ID of input NGram in a structure (stuProbAndBackoffWeights).
 */

stuProbAndBackoffWeights clsIndexBasedProbingModel::getNGramWeights(QList<WordIndex_t> _ngram) const
{
    if (_ngram.size() == 1){
        if(_ngram.first() == LM_UNKNOWN_WINDEX)
            return stuProbAndBackoffWeights(
                        _ngram.first(),
                        this->UnknownWeights.Prob,
                        this->UnknownWeights.Backoff
                        );

        Q_ASSERT_X(_ngram.first() < (WordIndex_t)this->HashTableSize ||
                   this->NGramHashTable[_ngram.first()].isMultiIndex() == false,
                "getNGramWeights",
                "Invalid Wordindex provided");

        const stuNGramHash& FoundItem = this->NGramHashTable[_ngram.first()];

        return stuProbAndBackoffWeights(
                    _ngram.first(),
                    FoundItem.Prob,
                    FoundItem.Backoff);
    }


    Hash_t HashLoc, HashValue;

    HashLoc = (HashFunctions::murmurHash64(_ngram, 0) % this->HashTableSize) + 1;;

    for (quint8 HashLevel = 1; HashLevel< MAX_HASH_LEVEL; HashLevel++)
    {
        HashValue = HashFunctions::murmurHash64(_ngram, HashLevel);

        if (this->NGramHashTable[HashLoc].hashValue() == this->getHashValue(HashValue) &&
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
}
