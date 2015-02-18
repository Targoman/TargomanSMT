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

#include <stdlib.h>
#include "libTargomanCommon/Constants.h"
#include "libTargomanCommon/HashFunctions.hpp"
#include "libTargomanCommon/FastOperations.hpp"
#include "libTargomanCommon/clsCmdProgressBar.h"
#include "clsAbstractProbingModel.h"
#include "../Definitions.h"

using namespace Targoman::Common;

namespace Targoman {
namespace NLPLibs {
namespace Private {

const char* BIN_FILE_HREADER = "TargomanLMBin-Probing-v1.0";

clsAbstractProbingModel::clsAbstractProbingModel() : intfBaseModel(enuMemoryModel::Probing)
{
    this->SumLevels = 0;
    this->MaxLevel = 0;
    this->StoredItems = 0;
    this->NGramHashTable = NULL;
}

/**
 * @brief Sets default probability and backoff for unknown words.
 */
void clsAbstractProbingModel::setUnknownWordDefaults(LogP_t _prob, LogP_t _backoff)
{
    this->UnknownWeights.Backoff = _backoff;
    this->UnknownWeights.Prob = _prob;
}

/**
 * @brief This function inserts a string in #NGramHashTable
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
 *
 * @param _ngram    input NGram string.
 * @param _order    order of NGram.
 * @param _prob     probability of NGram
 * @param _backoff  weight of backoff
 */
void clsAbstractProbingModel::insert(const char* _ngram, quint8 _order, LogP_t _prob, LogP_t _backoff)
{
    Hash_t HashLoc, HashValue;

    size_t NGramLen = strlen(_ngram);
    if (_order == 1 && !strcmp(_ngram, LM_UNKNOWN_WORD)){
        this->UnknownWeights.Backoff = _backoff;
        this->UnknownWeights.Prob = _prob;
        return;
    }
    //Our first guess.
    HashLoc = (HashFunctions::murmurHash64(_ngram, NGramLen, 0) % this->HashTableSize) + 1;;
    for (quint8 HashLevel = 1; HashLevel< MAX_HASH_LEVEL; ++HashLevel)
    {
        HashValue = HashFunctions::murmurHash64(_ngram, NGramLen, HashLevel) ;

        if (this->NGramHashTable[HashLoc].HashValueLevel){
            if (this->NGramHashTable[HashLoc].hashValue() == (HashValue & HASHVALUE_CONTAINER) &&
                    this->NGramHashTable[HashLoc].hashLevel() == HashLevel-1)
                throw exLanguageModel(QString("Fatal Collision found on: %1 (%2, %3, %4)").arg(
                                          _ngram).arg(
                                          HashLoc).arg(
                                          this->NGramHashTable[HashLoc].hashValue()).arg(
                                          HashLevel));
            this->NGramHashTable[HashLoc].setContinues();
        }else{
            this->NGramHashTable[HashLoc].HashValueLevel = (HashValue & HASHVALUE_CONTAINER) + HashLevel-1;
            this->NGramHashTable[HashLoc].Prob      = _prob;
            this->NGramHashTable[HashLoc].Backoff   = _backoff;

            this->MaxLevel = qMax(this->MaxLevel, HashLevel);
            this->SumLevels += HashLevel;
            ++this->StoredItems;

            return;
        }
        HashLoc = (HashValue % this->HashTableSize) + 1;
    }

    this->RemainingHashes.insert(_ngram,
                                 stuProbAndBackoffWeights(this->RemainingHashes.size() + this->HashTableSize, _prob,_backoff));
}

/**
 * @brief Initializes and allocates sufficient space for Hash table based on maximum oredr of NGram.
 * @param _maxNGramCount maximum order of NGram.
 */
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

void clsAbstractProbingModel::saveBinFile(const QString &_binFilePath, quint8 _order)
{
    QFile BinFile(_binFilePath);
    if (BinFile.open(QFile::WriteOnly) == false)
        throw exLanguageModel("Unable to open <" + _binFilePath + "> For writing");

    clsCmdProgressBar ProgressBar("Storing Bin Data", this->HashTableSize);

    BinFile.write(BIN_FILE_HREADER);
    BinFile.write(this->modelHeaderSuffix().toLatin1());
    BinFile.write((char*)&_order, sizeof(_order));
    BinFile.write((char*)&this->HashTableSize, sizeof(this->HashTableSize));
    BinFile.write((char*)&this->NgramCount, sizeof(this->NgramCount));
    for(Hash_t HashLoc=1; HashLoc<this->HashTableSize; ++HashLoc){
        ProgressBar.setValue(HashLoc);
        if (this->NGramHashTable[HashLoc].hashValue() > 0){
            BinFile.write((char*)&HashLoc, sizeof(Hash_t));
            BinFile.write((char*)&this->NGramHashTable[HashLoc].HashValueLevel, sizeof(quint8));
            BinFile.write((char*)&this->NGramHashTable[HashLoc].Prob, sizeof(LogP_t));
            BinFile.write((char*)&this->NGramHashTable[HashLoc].Backoff, sizeof(LogP_t));
        }
    }

    BinFile.close();

    ProgressBar.reset("Computing CheckSum", QFileInfo(_binFilePath).size() / 8192);
    QCryptographicHash Crypto(QCryptographicHash::Md5);
    if (BinFile.open(QFile::ReadOnly) == false)
        throw exLanguageModel("Unable to open <" + _binFilePath + "> For reading");
    int i=0;
    while(!BinFile.atEnd()){
        ProgressBar.setValue(++i);
        Crypto.addData(BinFile.read(8192));
    }
    BinFile.close();
    QByteArray Hash = Crypto.result();
    TargomanInfo(5, QString("BinFile Checksum: ") + Hash.toHex().constData());

    if (BinFile.open(QFile::Append) == false)
        throw exLanguageModel("Unable to open <" + _binFilePath + "> For writing");

    BinFile.write(Hash);
}

quint8 clsAbstractProbingModel::loadBinFile(const QString &_binFilePath)
{
    QFile BinFile(_binFilePath);
    if (BinFile.open(QFile::ReadOnly) == false)
        throw exLanguageModel("Unable to open <" + _binFilePath + "> For reading");

    if (BinFile.read(sizeof(BIN_FILE_HREADER)) != BIN_FILE_HREADER)
        throw exLanguageModel("Incompatible bin file");
    quint8 Order;
    try{
        QString Model = BinFile.read(this->modelHeaderSuffix().size());
        if (Model != this->modelHeaderSuffix())
            throw exLanguageModel(QString("Incompatible Bin File. %1 vs %2").arg(this->modelHeaderSuffix()).arg(Model));
        Order = BinFile.read(sizeof(quint8)).toUInt();
        this->HashTableSize = BinFile.read(sizeof(this->HashTableSize)).toULongLong();
        this->NgramCount = BinFile.read(sizeof(this->NgramCount)).toULongLong();
    }catch (exLanguageModel &e){
        throw;
    }catch(...){
        throw exLanguageModel("Invalid truncated BinFile");
    }

    if (this->HashTableSize == 0 ||
        this->NgramCount == 0 ||
        this->HashTableSize < this->NgramCount)
        throw exLanguageModel("Invalid bin file");

    BinFile.close();
    if (BinFile.open(QFile::ReadOnly) == false)
        throw exLanguageModel("Unable to open <" + _binFilePath + "> For reading");


    clsCmdProgressBar ProgressBar("Computing CheckSum", QFileInfo(_binFilePath).size() / 8192);
    QCryptographicHash Crypto(QCryptographicHash::Md5);
    if (BinFile.open(QFile::ReadOnly) == false)
        throw exLanguageModel("Unable to open <" + _binFilePath + "> For reading");
    int i=0;
    QByteArray MD5Sum;
    while(!BinFile.atEnd()){
        ProgressBar.setValue(++i);
        QByteArray Data = BinFile.read(8192);
        if (Data.size() < 8192 || BinFile.atEnd()){
            MD5Sum = Data.mid(Data.size() - 16);
            Data.truncate(Data.size() - 16); // Ignore last 16 byte where MD5Sum is stored
        }
        Crypto.addData(Data);
    }

    if (Crypto.result() != MD5Sum)
        throw exLanguageModel(QString("Checksum has failed: %1 vs %2").arg(
                                  MD5Sum.toHex().constData()).arg(
                                  Crypto.result().toHex().constData()));

    BinFile.close();

    if (this->NGramHashTable)
        delete [] this->NGramHashTable;

    TargomanInfo(5, "Allocating "<<this->HashTableSize * sizeof(stuNGramHash)<<
                 " Bytes for max "<<this->HashTableSize<<" Items. Curr Items = "<<this->NgramCount);
    this->NGramHashTable = new stuNGramHash[this->HashTableSize + 1];
    TargomanInfo(5, "Allocated");

    ProgressBar.reset("Loading BinaryLM", this->HashTableSize);
    if (BinFile.open(QFile::ReadOnly) == false)
        throw exLanguageModel("Unable to open <" + _binFilePath + "> For reading");

    for (Hash_t i; i<this->NgramCount; ++i){
        Hash_t HashLoc = BinFile.read(sizeof(Hash_t)).toULongLong();
        this->NGramHashTable[HashLoc].HashValueLevel = BinFile.read(sizeof(quint64)).toULongLong();
        this->NGramHashTable[HashLoc].Prob = BinFile.read(sizeof(LogP_t)).toFloat();
        this->NGramHashTable[HashLoc].Backoff = BinFile.read(sizeof(LogP_t)).toFloat();

        ProgressBar.setValue(i);
    }

    return Order;
}

bool clsAbstractProbingModel::isBinary(const QString &_binFilePath)
{
    QFile BinFile(_binFilePath);
    if (BinFile.open(QFile::ReadOnly) == false)
        throw exLanguageModel("Unable to open <" + _binFilePath + "> For reading");

    return (BinFile.read(sizeof(BIN_FILE_HREADER)) == BIN_FILE_HREADER);
}

/**
 * @brief Retrieves probability, backoff weight and ID of input NGram in Hash Table.
 * @param[in] _ngram        input NGram.
 * @param[in] _justSingle   is input NGram is unigram or not.
 * @return returns probability, backoff weight and ID of input NGram in a structure (stuProbAndBackoffWeights).
 */

stuProbAndBackoffWeights clsAbstractProbingModel::getNGramWeights(const char *_ngram, bool _justSingle) const
{
    Hash_t HashLoc, HashValue;
    size_t NGramLen = strlen(_ngram);

    HashLoc = (HashFunctions::murmurHash64(_ngram, NGramLen, 0) % this->HashTableSize) + 1;

    for (quint8 HashLevel = 1; HashLevel< MAX_HASH_LEVEL; ++HashLevel)
    {
        HashValue = HashFunctions::murmurHash64(_ngram, NGramLen, HashLevel);

        if (this->NGramHashTable[HashLoc].hashValue() == (HashValue & HASHVALUE_CONTAINER) &&
                this->NGramHashTable[HashLoc].hashLevel() == HashLevel-1){
            if (_justSingle && this->NGramHashTable[HashLoc].isMultiIndex())
                //Note that we didn't check continue flag for further search on input unigram.
                //That's because in program flow, we first insert unigrams then we insert multigrams.
                //So it is impossible for a unigram to set continue flag of a multiIndex cell.
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
