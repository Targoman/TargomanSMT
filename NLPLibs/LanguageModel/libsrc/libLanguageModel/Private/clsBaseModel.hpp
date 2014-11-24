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
 */

#ifndef TARGOMAN_NLPLIBS_PRIVATE_TMPLGENERICMODEL_H
#define TARGOMAN_NLPLIBS_PRIVATE_TMPLGENERICMODEL_H

#include "../Definitions.h"
//#include "clsVocab.hpp"
#include "NGram.h"

namespace Targoman {
namespace NLPLibs {
namespace Private {

class clsBaseModel
{
public:
    clsBaseModel(enuMemoryModel::Type _type/*, clsVocab* _vocab*/) : Type(_type)/*, Vocab(_vocab)*/{}
/*    inline void add2Vocab(const QString& _word){
        this->Vocab->addWord(_word);
    }*/
    inline Targoman::Common::WordIndex_t add2Vocab(const std::string& _word){
        //return this->Vocab->addWord(_word);
    }
    virtual void initHashTable(quint32 _maxNGramCount) = 0;
    virtual void setUnknownWordDefaults(Targoman::Common::LogP_t _prob, Targoman::Common::LogP_t _backoff)=0;
    virtual void  insert(const char* _ngram, float _prob, float _backoff) = 0;
    virtual Targoman::Common::LogP_t lookupNGram(const QStringList &_ngram, quint8& _foundedGram) const = 0;
    virtual void printStats() = 0;

public:
  //  inline const clsVocab& vocab(){ return *this->Vocab;}

protected:
    enuMemoryModel::Type  Type;
  //  clsVocab* Vocab;
};

}
}
}

#endif // TARGOMAN_NLPLIBS_PRIVATE_TMPLGENERICMODEL_H
