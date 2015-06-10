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
#include "libTargomanCommon/Configuration/tmplConfigurable.h"

namespace Targoman {
namespace Common {

TARGOMAN_ADD_EXCEPTION_HANDLER(exSimpleAuthentication, exTargomanBase);

class SimpleAuthentication
{
public:
    struct stuLoginInfo{
        bool CanView;
        bool CanChange;
        stuLoginInfo(bool _canView = false, bool _canChange = false){
            this->CanChange = _canChange;
            this->CanView = _canView;
        }
    };

public:
    static QString hashPass(const QString &_user, const QString& _pass, const QString& _salt = "");
    static stuLoginInfo checkLogin(const QString& _user,
                                   const QString& _pass,
                                   const QString& _ip);

    static stuLoginInfo checkLogin(const QString& _user,
                                   const QString& _pass,
                                   const QString& _salt,
                                   const QString& _ip);
    static void registerUser(const QString& _user,
                             const QString& _pass,
                             const QString& _cidr,
                             bool  _canView,
                             bool  _canChange);
    static void removeUser(const QString& _user);
    static void updateUser(const QString& _user,
                             const QString& _pass,
                             const QString& _cidr,
                             bool  _canView,
                             bool  _canChange);

public:
    static Common::Configuration::tmplConfigurable<FilePath_t> UserInfoFile;
};

}
}
#endif // TARGOMAN_COMMON_SIMPLEAUTHENTICATION_H
