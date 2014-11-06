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

#ifndef TARGOMAN_COMMON_BITMANAGER_HPP
#define TARGOMAN_COMMON_BITMANAGER_HPP

#include <QtCore>

namespace Targoman {
namespace Common {

typedef union { float AsFloat; quint32 AsUInt32; } FloatEncoded_t;

const quint32 INT32_SIGN_BIT  = 0x80000000;

}
}


#endif // TARGOMAN_COMMON_BITMANAGER_HPP
