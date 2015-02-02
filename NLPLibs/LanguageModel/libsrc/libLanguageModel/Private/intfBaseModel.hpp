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

namespace Targoman {
namespace NLPLibs {
namespace Private {

class intfBaseModel
{
public:
    intfBaseModel(enuMemoryModel::Type _type) : Type(_type){}

    virtual void init(quint32 _maxNGramCount) = 0;
    virtual void setUnknownWordDefaults(Targoman::Common::LogP_t _prob, Targoman::Common::LogP_t _backoff)=0;
    virtual void  insert(const char* _ngram, quint8 _order, float _prob, float _backoff) = 0;
    virtual Targoman::Common::LogP_t lookupNGram(const QStringList &_ngram, quint8& _foundedGram) const = 0;
    virtual Targoman::Common::LogP_t lookupNGram(const QList<Common::WordIndex_t> &_ngram, quint8& _foundedGram) const = 0;
    virtual QString getStatsStr() const = 0;
    virtual quint64 getID(const char* _word) const = 0;
protected:
    enuMemoryModel::Type  Type;
};

}
}
}

#endif // TARGOMAN_NLPLIBS_PRIVATE_TMPLGENERICMODEL_H
