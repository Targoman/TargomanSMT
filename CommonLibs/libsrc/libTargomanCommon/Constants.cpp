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

#include "Constants.h"
#include <cmath>

namespace Targoman {
namespace Common {

const LogP_t Constants::LogP_Zero = -HUGE_VAL;            /* log(0) = -Infinity */
const LogP_t Constants::LogP_Inf  = HUGE_VAL;             /* log(Inf) = Infinity */
const LogP_t Constants::LogP_One  = 0.0;                  /* log(1) = 0 */

}
}
