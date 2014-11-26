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

#include "clsBaseModel.hpp"
#include "WeightStructs.hpp"
#include "../Definitions.h"

#include <QHash>

namespace Targoman {
namespace NLPLibs {
namespace Private {

class clsProbingModel : public clsBaseModel
{
    struct stuNGramHash{
        quint64                    HashValueLevel;
        Targoman::Common::LogP_t   Prob;
        Targoman::Common::LogP_t   Backoff;
        std::string*               Original;

        inline void setHashLevel(quint8 _level){
            this->HashValueLevel = (this->HashValueLevel & 0xFFFFFFFFFFFFFFE0LL) + (_level & 0x1F); }
        inline quint8  hashLevel(){ return this->HashValueLevel & 0x1F; }
        inline quint64 hashValue(){ return this->HashValueLevel & 0xFFFFFFFFFFFFFFC0LL; }
        inline bool    continues(){ return this->HashValueLevel & 0x20; }
        inline void    setContinues(){ this->HashValueLevel |= 0x20; }
    };

public:
    clsProbingModel();
    void setUnknownWordDefaults(Targoman::Common::LogP_t _prob, Targoman::Common::LogP_t _backoff);
    void insert(const char *_ngram, Common::LogP_t _prob, Common::LogP_t _backoff = 0);
    void init(quint32 _maxNGramCount);
    inline quint64 getID(const char *_word){
        return this->lookupNGram(_word).ID;
    };

    Targoman::Common::LogP_t lookupNGram(const QStringList &_ngram, quint8& _foundedGram) const;


    QString getStatsStr(){
        return QString("MaxLevel: %1 AverageLevel: %2 QHashed: %3").arg(
                    this->MaxLevel).arg(
                    this->SumLevels / (double)this->NgramCount).arg(
                    this->RemainingHashes.size());
    }

private:
    stuProbAndBackoffWeights lookupNGram(const char* _ngram) const;

private:
    //tmplNGramHashTable<stuProbAndBackoffWeights> LMData;
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
