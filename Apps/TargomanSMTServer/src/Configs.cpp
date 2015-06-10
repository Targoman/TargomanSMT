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

#include "Configs.h"

namespace Targoman {
namespace Apps {

using namespace Common;
using namespace Common::Configuration;

tmplConfigurable<quint8>     gConfigs::MaxThreads(
        gConfigs::appConfig("MaxThreads"),
        "Maximum Concurrent Translations",
        64,
        Validators::tmplNumericValidator<quint8, 0, 64>,
        "t",
        "MAX_THREADS",
        "max-threads");

}
}
