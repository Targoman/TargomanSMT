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

#ifndef TARGOMAN_COMMON_CONSTANTS_H
#define TARGOMAN_COMMON_CONSTANTS_H

#include "libTargomanCommon/Types.h"
#include "libTargomanCommon/Macros.h"

namespace Targoman {
namespace Common {

struct Constants
{
    static const LogP_t LogP_Zero;            /* log(0) = -Infinity */
    static const LogP_t LogP_Inf;             /* log(Inf) = Infinity */
    static const LogP_t LogP_One;             /* log(1) = 0 */

};

class PathAccess{
public:
    enum Option{
        Readable   = 0x01,
        Writeatble = 0x02,
        Executable = 0x04,
        File       = 0x08,
        Dir        = 0x10
    };
    Q_DECLARE_FLAGS(Options, Option) /**<  helps to treat logical "and" and logical "or" of Option enumeration as type of enumeration*/
};

}
}

Q_DECLARE_OPERATORS_FOR_FLAGS(Targoman::Common::PathAccess::Options)

#endif // TARGOMAN_COMMON_CONSTANTS_H
