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

#ifndef TARGOMAN_COMMON_CONSTANTS_H
#define TARGOMAN_COMMON_CONSTANTS_H

#include "libTargomanCommon/Types.h"
#include "libTargomanCommon/Macros.h"

namespace Targoman {
namespace Common {

struct Constants
{
    static const LogP_t     LogP_Zero;            /* log(0) = -Infinity */
    static const LogP_t     LogP_Inf;             /* log(Inf) = Infinity */
    static const LogP_t     LogP_One;             /* log(1) = 0 */
    static const quint32    MaxFileIOBytes;       /* 8192 */
};

TARGOMAN_DEFINE_ENUM(enuPathAccess,
                     Readable   = 0x01,
                     Writeatble = 0x02,
                     Executable = 0x04,
                     File       = 0x08,
                     Dir        = 0x10
                    );

}
}

#endif // TARGOMAN_COMMON_CONSTANTS_H
