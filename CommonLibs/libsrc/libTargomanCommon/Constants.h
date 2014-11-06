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

#ifndef TARGOMAN_DEFINITIONS_CONSTANTS_H
#define TARGOMAN_DEFINITIONS_CONSTANTS_H

#include "libTargomanCommon/Types.h"

namespace Targoman {
namespace Common {

struct Constants
{
    static const LogP_t LogP_Zero;            /* log(0) = -Infinity */
    static const LogP_t LogP_Inf;             /* log(Inf) = Infinity */
    static const LogP_t LogP_One;             /* log(1) = 0 */

};

}
}

#endif // TARGOMAN_DEFINITIONS_CONSTANTS_H
