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

#ifndef TARGOMAN_COMMON_FASTOPERATIONS_HPP
#define TARGOMAN_COMMON_FASTOPERATIONS_HPP

#include "libTargomanCommon/Types.h"

namespace Targoman {
namespace Common {

const quint32 INT32_SIGN_BIT  = 0x80000000;

inline bool isNegativFloat(float _num){
    return ((FloatEncoded_t*)&_num)->AsUInt32 & INT32_SIGN_BIT;
}

inline bool isPositiveFloat(float _num){
    return ! isNegativFloat(_num);
}

inline quint32 rotl32 ( quint32 x, qint8 r )
{
  return (x << r) | (x >> (32 - r));
}

inline quint64 rotl64 ( quint32 x, qint8 r )
{
  return (x << r) | (x >> (64 - r));
}

}
}


#endif // TARGOMAN_COMMON_FASTOPERATIONS_HPP
