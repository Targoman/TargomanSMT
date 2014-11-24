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
        std::string                Original;

        inline void setHashLevel(quint8 _level){
            this->HashValueLevel = (this->HashValueLevel & 0xFFFFFFFFFFFFFFF0LL) + (_level & 0x0F); }
        inline quint8  hashLevel(){ return this->HashValueLevel & 0x0F; }
        inline quint64 hashValue(){ return this->HashValueLevel & 0xFFFFFFFFFFFFFFE0LL; }
        inline bool    continues(){ return this->HashValueLevel & 0x10; }
        inline void    setContinues(){ this->HashValueLevel |= 0x10; }
    };

public:
    clsProbingModel(/*clsVocab* _vocab*/);
    void setUnknownWordDefaults(Targoman::Common::LogP_t _prob, Targoman::Common::LogP_t _backoff);
    void insert(const char *_ngram, Common::LogP_t _prob, Common::LogP_t _backoff = 0);
    void initHashTable(quint32 _maxNGramCount);
    Targoman::Common::LogP_t lookupNGram(const QStringList &_ngram, quint8& _foundedGram) const;
    void printStats(){
        qDebug()<<"MaxLevel:"<<this->MaxLevel<<"Avg:"<<
                  this->SumLevels / (double)this->NgramCount<<
                  "Remaining:"<<this->RemainingHashes.size();
    }
/*    quint8 maxHashLevel(){ return this->MaxLevel; }
    double averageHashLevel(){ return ((double)this->SumLevels) / (double)this->NgramCount; }
  */
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
