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

#ifndef TARGOMAN_COMMON_SIMPLEAUTHENTICATION_H
#define TARGOMAN_COMMON_SIMPLEAUTHENTICATION_H

#include "libTargomanCommon/exTargomanBase.h"

namespace Targoman {
namespace Common {

TARGOMAN_ADD_EXCEPTION_HANDLER(exSimpleAuthentication, exTargomanBase);

class SimpleAuthentication
{
public:
    static QString hashPass(const QString& _pass, const QString& _salt = "");
};

}
}
#endif // TARGOMAN_COMMON_SIMPLEAUTHENTICATION_H
