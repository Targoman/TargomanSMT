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

#ifndef TARGOMAN_NLPLIBS_PRIVATE_CLSINDEXBASEDPROBINGMODEL_H
#define TARGOMAN_NLPLIBS_PRIVATE_CLSINDEXBASEDPROBINGMODEL_H

#include "clsAbstractProbingModel.h"

namespace Targoman {
namespace NLPLibs {
namespace Private {

/**
 * @brief This class is a derivation of clsAbstractProbingModel class which overloads and adds some function to faciliate storage and retrieval of index based language models.
 */
class clsIndexBasedProbingModel : public clsAbstractProbingModel
{
public:
    clsIndexBasedProbingModel(){;}
    void insert(const char *_ngram, quint8 _order, Common::LogP_t _prob, Common::LogP_t _backoff);
    Common::LogP_t lookupNGram(const QList<Common::WordIndex_t> &_ngram, quint8 &_foundedGram) const;
    Targoman::Common::LogP_t lookupNGram(const QStringList &_ngram, quint8& _foundedGram) const;

private:
    void insert(QList<Common::WordIndex_t> _ngram, Common::LogP_t _prob, Common::LogP_t _backoff);
    stuProbAndBackoffWeights getNGramWeights(QList<Common::WordIndex_t> _ngram) const;
};

}
}
}

#endif // TARGOMAN_NLPLIBS_PRIVATE_CLSINDEXBASEDPROBINGMODEL_H
