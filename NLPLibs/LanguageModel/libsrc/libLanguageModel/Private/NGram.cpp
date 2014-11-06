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

#include "NGram.h"

namespace Targoman {
namespace NLPLibs {
namespace Private {

const uint PrimeNumbers[] = { 11, 59, 17, 47, 23, 29, 31, 37, 41, 43, 19, 53, 13 };

}
}
}

uint qHash(const Targoman::NLPLibs::Private::NGram_t &_key)
{
    uint Hash = 0;
    for (int i=0; i<_key.size(); i++)
        Hash+=_key.at(i) * Targoman::NLPLibs::Private::PrimeNumbers[i];
    return Hash;
}



