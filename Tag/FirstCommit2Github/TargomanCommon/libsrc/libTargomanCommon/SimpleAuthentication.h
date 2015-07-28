/******************************************************************************
 * Targoman: A robust Statistical Machine Translation framework               *
 *                                                                            *
 * Copyright 2014-2015 by ITRC <http://itrc.ac.ir>                            *
 *                                                                            *
 * This file is part of Targoman.                                             *
 *                                                                            *
 * Targoman is free software: you can redistribute it and/or modify           *
 * it under the terms of the GNU Lesser General Public License as published   *
 * by the Free Software Foundation, either version 3 of the License, or       *
 * (at your option) any later version.                                        *
 *                                                                            *
 * Targoman is distributed in the hope that it will be useful,                *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of             *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              *
 * GNU Lesser General Public License for more details.                        *
 * You should have received a copy of the GNU Lesser General Public License   *
 * along with Targoman. If not, see <http://www.gnu.org/licenses/>.           *
 *                                                                            *
 ******************************************************************************/
/**
 * @author S. Mohammad M. Ziabary <ziabary@targoman.com>
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
