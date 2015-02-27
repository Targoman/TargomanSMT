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
 */

#ifndef TARGOMAN_COMMON_TYPES_H
#define TARGOMAN_COMMON_TYPES_H

#include <QtCore>
#include "limits.h"
namespace Targoman {
namespace Common {

typedef float LogP_t;
typedef double Cost_t;

typedef unsigned WordIndex_t;
typedef union { float AsFloat; quint32 AsUInt32; } FloatEncoded_t;

#define OUTPUT
#define INOUT

}
}

#endif // TARGOMAN_COMMON_TYPES_H
