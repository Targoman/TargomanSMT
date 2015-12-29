/******************************************************************************
 * Targoman: A robust Statistical Machine Translation framework               *
 *                                                                            *
 * Copyright 2014-2015 by ITRC <http://itrc.ac.ir>                            *
 *                                                                            *
 * This file is part of Targoman.                                             *
 *                                                                            *
 * Targoman is free software: you can redistribute it and/or modify           *
 * it under the terms of the GNU Lesser General Public License as published   *
 * by the Free Software Foundation, either version 3 of the License, or       *
 * (at your option) any later version.                                        *
 *                                                                            *
 * Targoman is distributed in the hope that it will be useful,                *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of             *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              *
 * GNU Lesser General Public License for more details.                        *
 * You should have received a copy of the GNU Lesser General Public License   *
 * along with Targoman. If not, see <http://www.gnu.org/licenses/>.           *
 *                                                                            *
 ******************************************************************************/
/**
 * @author S. Mohammad M. Ziabary <ziabary@targoman.com>
 * @author Behrooz Vedadian <vedadian@targoman.com>
 * @author Saeed Torabzadeh <saeed.torabzadeh@targoman.com>
 */

#include <stdlib.h>
#include "libTargomanCommon/Constants.h"
#include "libTargomanCommon/HashFunctions.hpp"
#include "libTargomanCommon/FastOperations.hpp"
#include "libTargomanCommon/clsCmdProgressBar.h"
#include "clsAbstractProbingModel.h"
#include "../Definitions.h"
#include "libTargomanCommon/Logger.h"

using namespace Targoman::Common;

namespace Targoman {
namespace NLPLibs {
namespace TargomanLM {
namespace Private {

clsAbstractProbingModel::clsAbstractProbingModel() : intfBaseModel(enuMemoryModel::Probing)
{
    this->SumLevels = 0;
    this->MaxLevel = 0;
    this->StoredInHashTable = 0;
}

clsAbstractProbingModel::~clsAbstractProbingModel()
{
    //Just To suppress Compiler Error using QScopped Pointer
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
        this->setUnknownWordDefaults(_prob, _backoff);
        this->NGramHashTable[LM_UNKNOWN_WINDEX].Prob      = _prob;
        this->NGramHashTable[LM_UNKNOWN_WINDEX].Backoff   = _backoff;
        return;
    }
    //Our first guess.
    HashLoc = (HashFunctions::murmurHash64(_ngram, NGramLen, 0) % this->HashTableSize) + 1;;
    for (quint8 HashLevel = 1; HashLevel< MAX_HASH_LEVEL; ++HashLevel)
    {
        HashValue = HashFunctions::murmurHash64(_ngram, NGramLen, HashLevel) ;

        if (this->NGramHashTable[HashLoc].HashValueLevel){
            if (this->NGramHashTable[HashLoc].hashValue() == this->getHashValue(HashValue)  &&
                    this->NGramHashTable[HashLoc].hashLevel() == HashLevel-1)
                throw exLanguageModel(QString("Fatal Collision found on: %1 (%2, %3, %4)").arg(
                                          _ngram).arg(
                                          HashLoc).arg(
                                          this->NGramHashTable[HashLoc].hashValue()).arg(
                                          HashLevel));
            this->NGramHashTable[HashLoc].setContinues();
        }else{
            this->NGramHashTable[HashLoc].HashValueLevel = this->getHashValue(HashValue) + HashLevel-1;
            this->NGramHashTable[HashLoc].Prob      = _prob;
            this->NGramHashTable[HashLoc].Backoff   = _backoff;

            this->MaxLevel = qMax(this->MaxLevel, HashLevel);
            this->SumLevels += HashLevel;
            ++this->StoredInHashTable;

            if(_order == 1)
                this->Vocab.insert(HashLoc, QString::fromUtf8(_ngram));

            return;
        }
        HashLoc = (HashValue % this->HashTableSize) + 1;
    }

    Q_ASSERT(_order == 1);

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
    for (size_t i =0; i< (size_t)qMax(1, TargomanGoodHashTableSizesCount - 10); i++)
        if (_maxNGramCount < TargomanGoodHashTableSizes[i]){
            this->HashTableSize = TargomanGoodHashTableSizes[i+10];
            break;
        }

    if (this->HashTableSize){
        TargomanInfo(5, "Allocating "<<this->HashTableSize * sizeof(stuNGramHash)<<
                     " Bytes for max "<<this->HashTableSize<<" Items. Curr Items = "<<_maxNGramCount);
        this->NGramHashTable.reset(new stuNGramHash[this->HashTableSize + 1]);
        TargomanInfo(5, "Allocated");
        this->NgramCount = _maxNGramCount;
    }else
        throw exLanguageModel("Count of NGrams("+QString::number(_maxNGramCount)+") exeeds valid Table Sizes");

    this->NGramHashTable[LM_UNKNOWN_WINDEX].Prob      = this->UnknownWeights.Prob;
    this->NGramHashTable[LM_UNKNOWN_WINDEX].Backoff   = this->UnknownWeights.Backoff;
    this->NGramHashTable[LM_UNKNOWN_WINDEX].HashValueLevel = 1; // Just to inform save binary that there is a data stored
    ++this->StoredInHashTable;
}

void clsAbstractProbingModel::saveBinFile(const QString &_binFilePath, quint8 _order)
{
    QFile BinFile(_binFilePath);
    if (BinFile.open(QFile::WriteOnly) == false)
        throw exLanguageModel("Unable to open <" + _binFilePath + "> For writing");

    /***********************************************************************************
          Write Whole Data to file
     ***********************************************************************************/
    clsCmdProgressBar ProgressBar("Storing Bin Data", this->HashTableSize);
    QDataStream OutputStream(&BinFile);

    BinFile.write(BIN_FILE_HEADER.toLatin1());
    BinFile.write(this->modelHeaderSuffix().toLatin1());
    OutputStream.setVersion(QDataStream::Qt_5_0);
    OutputStream << _order;
    OutputStream << this->HashTableSize;
    OutputStream << this->StoredInHashTable;
    OutputStream << this->NgramCount;

    /*
    quint64 Inserted = 0;
    for(Hash_t HashLoc=0; HashLoc<=this->HashTableSize; ++HashLoc){
        if (this->NGramHashTable[HashLoc].HashValueLevel != 0){
            ProgressBar.setValue(HashLoc);
            OutputStream << HashLoc;
            OutputStream << this->NGramHashTable[HashLoc].HashValueLevel;
            OutputStream << this->NGramHashTable[HashLoc].Prob;
            OutputStream << this->NGramHashTable[HashLoc].Backoff;
            ++Inserted;
        }
    }

    if (Inserted != this->StoredInHashTable)
        throw exLanguageModel(QString("Count of HashTable Items differs. %1 must be %2").arg(Inserted).arg(this->StoredInHashTable));
    */

    int BulkSize = Constants::MaxFileIOBytes / sizeof(stuNGramHash);
    size_t i = 0;
    for(; i < this->HashTableSize - BulkSize + 1; i += BulkSize) {
        BinFile.write((char*)&this->NGramHashTable[i], BulkSize * sizeof(stuNGramHash));
        ProgressBar.setValue(i);
    }
    if(i < this->HashTableSize)
        BinFile.write((char*)&this->NGramHashTable[i], (this->HashTableSize - i) * sizeof(stuNGramHash));
    ProgressBar.finalize(true);

    OutputStream << this->RemainingHashes;

    OutputStream << this->Vocab;

    BinFile.close();

    /***********************************************************************************
         Compute MD5Sum of the stored file
     ***********************************************************************************/
    ProgressBar.reset("Computing CheckSum", QFileInfo(_binFilePath).size() / Constants::MaxFileIOBytes);
    QCryptographicHash Crypto(QCryptographicHash::Md5);
    if (BinFile.open(QFile::ReadOnly) == false)
        throw exLanguageModel("Unable to open <" + _binFilePath + "> For reading");
    i=0;
    while(!BinFile.atEnd()){
        ProgressBar.setValue(++i);
        Crypto.addData(BinFile.read(Constants::MaxFileIOBytes));
    }
    QByteArray Checksum = Crypto.result();
    TargomanInfo(5, QString("BinFile Checksum: ") + Checksum.toHex().constData());
    BinFile.close();

    /***********************************************************************************
         Append Checksum to file
     ***********************************************************************************/
    if (BinFile.open(QFile::Append) == false)
        throw exLanguageModel("Unable to open <" + _binFilePath + "> For writing");

    BinFile.write(Checksum, Checksum.size());
    BinFile.close();
}

quint8 clsAbstractProbingModel::loadBinFile(const QString &_binFilePath, bool _computeChecksum)
{
    TargomanLogInfo(5, "Loading binaryLM from: " + _binFilePath);
    QFile BinFile(_binFilePath);

    if (BinFile.open(QFile::ReadOnly) == false)
        throw exLanguageModel("Unable to open <" + _binFilePath + "> For reading");

    /***********************************************************************************
         Primary check for header
     ***********************************************************************************/
    QByteArray Header, Model;
    quint8 Order;
    try{
        QDataStream InputStream(&BinFile);

        Header = BinFile.read(BIN_FILE_HEADER.size());
        if (Header != BIN_FILE_HEADER.toLatin1())
            throw exLanguageModel(QString("Incompatible Bin file: %1").arg(Header.append('\0').constData()));
        Model = BinFile.read(this->modelHeaderSuffix().size());
        if (Model != this->modelHeaderSuffix())
            throw exLanguageModel(QString("Incompatible Bin File. %1 vs %2").arg(this->modelHeaderSuffix()).arg(Model.append('\0').constData()));
        InputStream.setVersion(QDataStream::Qt_5_0);
        //TODO load bulk from file
        InputStream >> Order;
        InputStream >> this->HashTableSize;
        InputStream >> this->StoredInHashTable;
        InputStream >> this->NgramCount;
    }catch (exLanguageModel&){
        throw;
    }catch(...){
        throw exLanguageModel("Invalid truncated BinFile");
    }

    if (this->HashTableSize == 0 ||
        this->NgramCount == 0 ||
        this->StoredInHashTable == 0 ||
        this->HashTableSize < this->NgramCount)
        throw exLanguageModel("Invalid bin file or corrupted header");

    BinFile.close();
    /***********************************************************************************
        Compute checksum
     ***********************************************************************************/
    clsCmdProgressBar ProgressBar;
    if(_computeChecksum){
        const quint64 FileSize  = QFileInfo(_binFilePath).size();
        const quint32 LoopCount = (FileSize - 16) / Constants::MaxFileIOBytes;
        const quint32 Remainder = FileSize - (LoopCount * Constants::MaxFileIOBytes) - 16;
        QByteArray CheckSum;

        ProgressBar.reset("Computing CheckSum", LoopCount + 1);
        QCryptographicHash Crypto(QCryptographicHash::Md5);
        if (BinFile.open(QFile::ReadOnly) == false)
            throw exLanguageModel("Unable to open <" + _binFilePath + "> For reading");
        try{
            for(quint64 i = 0; i < LoopCount; ++i){
                ProgressBar.setValue(i+1);
                Crypto.addData(BinFile.read(Constants::MaxFileIOBytes));
            }
            Crypto.addData(BinFile.read(Remainder));
            ProgressBar.setValue(LoopCount+1);
            CheckSum = BinFile.read(16);
        }catch(...){
            throw exLanguageModel("Invalid truncated BinFile");
        }

        if (Crypto.result() != CheckSum)
            throw exLanguageModel(QString("Checksum has failed: %1 vs %2").arg(
                                      CheckSum.toHex().constData()).arg(
                                      Crypto.result().toHex().constData()));
        BinFile.close();
    }
    /***********************************************************************************
         Load Bin File
     ***********************************************************************************/
    TargomanInfo(5, "Allocating "<<this->HashTableSize * sizeof(stuNGramHash)<<
                 " Bytes for max "<<this->HashTableSize<<" Items. Curr Items = "<<this->NgramCount);
    this->NGramHashTable.reset(new stuNGramHash[this->HashTableSize + 1]);
    TargomanInfo(5, "Allocated");

    if (BinFile.open(QFile::ReadOnly) == false)
        throw exLanguageModel("Unable to open <" + _binFilePath + "> For reading");


    QDataStream InputStream(&BinFile);
    /** Dummy Reads **/
    BinFile.read(BIN_FILE_HEADER.size());
    BinFile.read(this->modelHeaderSuffix().size());
    InputStream.setVersion(QDataStream::Qt_5_0);
    InputStream >> Order;
    InputStream >> this->HashTableSize;
    InputStream >> this->StoredInHashTable;
    InputStream >> this->NgramCount;

    ProgressBar.reset("Loading BinaryLM (Phase 1)", this->StoredInHashTable);

    /*
    for (Hash_t i = 1; i<=this->StoredInHashTable; ++i){
        Hash_t HashLoc;
        InputStream >> HashLoc;
        InputStream >> this->NGramHashTable[HashLoc].HashValueLevel;
        InputStream >> this->NGramHashTable[HashLoc].Prob;
        InputStream >> this->NGramHashTable[HashLoc].Backoff;

        ProgressBar.setValue(i);
    }
    */

    int BulkSize = Constants::MaxFileIOBytes / sizeof(stuNGramHash);
    size_t i = 0;
    for(; i < this->HashTableSize - BulkSize + 1; i += BulkSize) {
        BinFile.read((char*)&this->NGramHashTable[i], BulkSize * sizeof(stuNGramHash));
        ProgressBar.setValue(i);
    }
    if(i < this->HashTableSize)
        BinFile.read((char*)&this->NGramHashTable[i], (this->HashTableSize - i) * sizeof(stuNGramHash));

    clsAbstractProbingModel::setUnknownWordDefaults(this->NGramHashTable[LM_UNKNOWN_WINDEX].Prob,
                                                    this->NGramHashTable[LM_UNKNOWN_WINDEX].Backoff);

    ProgressBar.reset("Loading BinaryLM (Phase 2)", 1);
    InputStream >> this->RemainingHashes;
    ProgressBar.finalize(true);

    ProgressBar.reset("Loading BinaryLM (Phase 3)", 1);
    InputStream >> this->Vocab;
    ProgressBar.finalize(true);

    TargomanLogInfo(5, "Binary LM File Loaded. " + this->getStatsStr());
    return Order;
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

        if (this->NGramHashTable[HashLoc].hashValue() == this->getHashValue(HashValue) &&
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
}
