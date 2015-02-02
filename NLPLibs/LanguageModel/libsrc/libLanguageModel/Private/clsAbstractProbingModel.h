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

#ifndef TARGOMAN_NLPLIBS_PRIVATE_CLSPROBINGMODEL_H
#define TARGOMAN_NLPLIBS_PRIVATE_CLSPROBINGMODEL_H

#include "intfBaseModel.hpp"
#include "WeightStructs.hpp"
#include "../Definitions.h"

#include <QHash>

namespace Targoman {
namespace NLPLibs {
namespace Private {

const quint8  MAX_HASH_LEVEL = 32;
const quint64 HASHVALUE_CONTAINER = 0xFFFFFFFFFFFFFF80LL;
typedef quint64 Hash_t;


class clsAbstractProbingModel : public intfBaseModel
{
    struct stuNGramHash{
        quint64                    HashValueLevel;
        Targoman::Common::LogP_t   Prob;
        Targoman::Common::LogP_t   Backoff;
        std::string*               Original;

        inline void setHashLevel(quint8 _level){
            Q_ASSERT(_level <= 0x1F);
            this->HashValueLevel = (this->HashValueLevel & (HASHVALUE_CONTAINER | 0x40 | 0x20)) + (_level & 0x1F); }
        inline quint8  hashLevel(){ return this->HashValueLevel & 0x1F; }
        inline quint64 hashValue(){ return this->HashValueLevel & HASHVALUE_CONTAINER; }
        inline bool    continues(){ return this->HashValueLevel & 0x20; }
        inline void    setContinues(){ this->HashValueLevel |= 0x20; }
        inline bool    isMultiIndex(){ return this->HashValueLevel & 0x40; }
        inline void    setMultiIndex(){ this->HashValueLevel |= 0x40; }
    };

public:
    clsAbstractProbingModel();
    void setUnknownWordDefaults(Targoman::Common::LogP_t _prob, Targoman::Common::LogP_t _backoff);
    virtual void insert(const char *_ngram, quint8 _order, Common::LogP_t _prob, Common::LogP_t _backoff = 0);
    void init(quint32 _maxNGramCount);
    inline quint64 getID(const char *_word) const {
        return this->getNGramWeights(_word).ID;
    }

    inline bool isValidIndex(Common::WordIndex_t _index) const {
        return (quint64)_index < this->HashTableSize + 1;
    }

    virtual Targoman::Common::LogP_t lookupNGram(const QStringList &_ngram, quint8& _foundedGram) const = 0;
    virtual Targoman::Common::LogP_t lookupNGram(const QList<Common::WordIndex_t> &_ngram, quint8& _foundedGram) const = 0;

    QString getStatsStr() const {
        return QString("MaxLevel: %1 AverageLevel: %2 QHashed: %3").arg(
                    this->MaxLevel).arg(
                    this->SumLevels / (double)this->NgramCount).arg(
                    this->RemainingHashes.size());
    }

protected:
    stuProbAndBackoffWeights getNGramWeights(const char* _ngram, bool _justSingle = false) const;

protected:
    quint32                     HashTableSize;
    quint32                     NgramCount;
    stuNGramHash*               NGramHashTable;
    QHash<QString, stuProbAndBackoffWeights> RemainingHashes;
    stuProbAndBackoffWeights    UnknownWeights;
    quint8                      MaxLevel;
    quint64                     SumLevels;
};

}
}
}

#endif // TARGOMAN_NLPLIBS_PRIVATE_CLSPROBINGMODEL_H
