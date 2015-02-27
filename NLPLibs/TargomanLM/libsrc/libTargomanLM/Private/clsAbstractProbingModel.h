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
  @author Behrooz Vedadian <vedadian@aut.ac.ir>
 */

#ifndef TARGOMAN_NLPLIBS_LANGUAGEMODEL_PRIVATE_CLSPROBINGMODEL_H
#define TARGOMAN_NLPLIBS_LANGUAGEMODEL_PRIVATE_CLSPROBINGMODEL_H

#include "intfBaseModel.hpp"
#include "WeightStructs.hpp"
#include "../Definitions.h"

#include <QHash>

namespace Targoman {
namespace NLPLibs {
namespace TargomanLM {

extern const QString BIN_FILE_HEADER;

namespace Private {

const quint8  MAX_HASH_LEVEL = 64;
const quint64 HASHVALUE_CONTAINER = 0xFFFFFFFFFFFFFF00LL;
typedef quint64 Hash_t;


class clsAbstractProbingModel : public intfBaseModel
{
    /**
     * @struct Our hash table is from this type.
     */
    struct stuNGramHash{
        /**
         * @brief HashValueLevel
         * The first lowest 5 bits of this variable, is for recording hash level (which can save a level between 0-31).
         * The Sixth bit of this variable is for continue flag (which specifies whether another value in hash table, have guessed this occupied place for its location or not).
         * The Seven bit of this variable is for multiIndex flag (which specifies whether value of cell is hash of a multi gram or unigram).
         * The remaining 57 bits of this variable is for recording hash value of Ngram.
         */
        quint64                    HashValueLevel;
        
        Targoman::Common::LogP_t   Prob;                /**< The probability of NGram. */
        Targoman::Common::LogP_t   Backoff;             /**< The Weight of Backoff. */

        /** @brief Sets level of hashing in #HashValueLevel . */
        inline void setHashLevel(quint8 _level){
            Q_ASSERT(_level <= 0x1F);
            this->HashValueLevel = (this->HashValueLevel & (HASHVALUE_CONTAINER | 0x80 | 0x40)) + (_level & 0x3F); }
        /** @return returns level of hashing from #HashValueLevel. */
        inline quint8  hashLevel() const{ return this->HashValueLevel & 0x3F; }
        /** @return returns hash value from #HashValueLevel. */
        inline quint64 hashValue() const{ return this->HashValueLevel & HASHVALUE_CONTAINER; }
        /** @brief Does continue flag of cell is set or not. */
        inline bool    continues() const{ return this->HashValueLevel & 0x40; }
        /** @brief Sets continue flag of cell in #HashValueLevel. */
        inline void    setContinues(){ this->HashValueLevel |= 0x40; }
        /** @brief Does continue flag of cell is set or not. */
        inline bool    isMultiIndex() const{ return this->HashValueLevel & 0x80; }
        /** @brief Sets continue flag of cell in #HashValueLevel. */
        inline void    setMultiIndex(){ this->HashValueLevel |= 0x80; }

        stuNGramHash(){
            this->HashValueLevel = 0;
        }
    };
protected:
    inline Hash_t getHashValue(Hash_t _hash) const{
        return Q_LIKELY(_hash & HASHVALUE_CONTAINER) ? (_hash & HASHVALUE_CONTAINER) : HASHVALUE_CONTAINER;
    }

public:
    clsAbstractProbingModel();
    ~clsAbstractProbingModel();

    void setUnknownWordDefaults(Targoman::Common::LogP_t _prob, Targoman::Common::LogP_t _backoff);
    virtual void insert(const char *_ngram, quint8 _order, Common::LogP_t _prob, Common::LogP_t _backoff = 0);
    void init(quint32 _maxNGramCount);
    inline quint64 getID(const char *_word) const {
        return this->getNGramWeights(_word).ID;
    }

    inline QString getWordByID(Common::WordIndex_t _wordIndex) const{
        return this->Vocab.value(_wordIndex, LM_UNKNOWN_WORD);
    }

    inline bool isValidIndex(Common::WordIndex_t _index) const {
        return (quint64)_index < this->HashTableSize + 1;
    }

    virtual Targoman::Common::LogP_t lookupNGram(const QStringList &_ngram, quint8& _foundedGram) const = 0;
    virtual Targoman::Common::LogP_t lookupNGram(const QList<Common::WordIndex_t> &_ngram, quint8& _foundedGram) const = 0;
    virtual void    saveBinFile(const QString& _binFilePath, quint8 _order);
    virtual quint8 loadBinFile(const QString& _binFilePath, bool _computeChecksum);
    virtual QString modelHeaderSuffix() = 0;

    QString getStatsStr() const {
        return QString("Count: %4 MaxLevel: %1 AverageLevel: %2 QHashed: %3").arg(
                    this->MaxLevel).arg(
                    this->SumLevels / (double)this->StoredInHashTable).arg(
                    this->RemainingHashes.size()).arg(
                    this->StoredInHashTable+this->RemainingHashes.size());
    }


protected:
    stuProbAndBackoffWeights getNGramWeights(const char* _ngram, bool _justSingle = false) const;

protected:
    quint32                     HashTableSize;                  /**< Size of hash table. */
    quint32                     NgramCount;                     /**< Max NGram Existed in language model. */
    QScopedArrayPointer<stuNGramHash> NGramHashTable;           /**< Hash table of NGram. */
    QHash<QString, stuProbAndBackoffWeights> RemainingHashes;   /**< A QHash container to insert NGram that can not be inserted in #NGramHashTable. */
    stuProbAndBackoffWeights    UnknownWeights;                 /**< Weight of unknown word. */
    quint8                      MaxLevel;                       /**< Maximum level that was needed during inserting NGrams in #NGramHashTable . */
    quint64                     SumLevels;                      /**< sum of levels of hash levels, calculated during inserting NGrams in #NGramHashTable . */
    quint64                     StoredInHashTable;                    /**< count of items stored, calculated during inserting NGrams in #NGramHashTable . */
    QHash<Common::WordIndex_t, QString>  Vocab;
};

}
}
}
}

#endif // TARGOMAN_NLPLIBS_LANGUAGEMODEL_PRIVATE_CLSPROBINGMODEL_H
