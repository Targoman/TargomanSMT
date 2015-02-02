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

#ifndef TARGOMAN_CORE_PRIVATE_PHRASETABLE_CLSTARGETRULE_H
#define TARGOMAN_CORE_PRIVATE_PHRASETABLE_CLSTARGETRULE_H

#include <QList>
#include "libTargomanCommon/Types.h"

namespace Targoman {
namespace Core {
namespace Private {
namespace PhraseTable{

class clsTargetRule : public QSharedData
{
public:
    clsTargetRule();

    Common::Cost_t getPhraseCost();
    inline Common::WordIndex_t operator [](int _index){
        return this->TargetPhrase.at(_index);
    }

    inline size_t size(){
        return this->TargetPhrase.size();
    }

private:

private:
    mutable QList<Common::WordIndex_t> TargetPhrase;
    mutable QList<Common::Cost_t>      Costs;

    mutable Common::LogP_t  AllCosts;
    mutable bool            AllCostsComputed;
    mutable Common::LogP_t  LMCosts;
    mutable Common::LogP_t  LMCostsWithoutFirstWord;
    mutable bool            LMCostsComputed;

    float   Counts;
    float   Marginals;
    float   RealCount;
};

}
}
}
}

#endif // TARGOMAN_CORE_PRIVATE_PHRASETABLE_CLSTARGETRULE_H
