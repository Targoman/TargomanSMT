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

#ifndef TARGOMAN_CORE_GLOBALCONFIGS_H
#define TARGOMAN_CORE_GLOBALCONFIGS_H

#include "libTargomanCommon/exTargomanBase.h"
#include "libTargomanCommon/Configuration.h"

namespace Targoman {
namespace Core {

TARGOMAN_ADD_EXCEPTION_HANDLER(exTargomanCore, Targoman::Common::exTargomanBase);

struct stuGlobalConfigs{
    static Targoman::Common::clsConfigurable<QString> Separator;
};

extern stuGlobalConfigs gConfigs;

}
}
#endif // TARGOMAN_CORE_GLOBALCONFIGS_H
