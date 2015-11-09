/*******************************************************************************
 * Targoman: A robust Statistical Machine Translation framework                *
 *                                                                             *
 * Copyright 2014-2015 by ITRC <http://itrc.ac.ir>                           *
 *                                                                             *
 * This file is part of Targoman.                                              *
 *                                                                             *
 * Targoman is free software: you can redistribute it and/or modify            *
 * it under the terms of the GNU Lesser General Public License as published by *
 * the Free Software Foundation, either version 3 of the License, or           *
 * (at your option) any later version.                                         *
 *                                                                             *
 * Targoman is distributed in the hope that it will be useful,                 *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of              *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the               *
 * GNU Lesser General Public License for more details.                         *
 * You should have received a copy of the GNU Lesser General Public License    *
 * along with Targoman. If not, see <http://www.gnu.org/licenses/>.            *
 *                                                                             *
 *******************************************************************************/
/**
 @author S. Mohammad M. Ziabary <ziabary@targoman.com>
 */

#ifndef TARGOMAN_APPS_MODULES_TARGOMANWEBSERVICE_H
#define TARGOMAN_APPS_MODULES_TARGOMANWEBSERVICE_H

#include "libQJsonRPC/qjsonrpcservice.h"

class TargomanWebService : public QJsonRpcService
{
    Q_OBJECT
    Q_CLASSINFO("serviceName", "TargomanAPI")

public:
    TargomanWebService(){}

public slots:
/*     QString     login($user, $pass, $salt);
     QJsonObject translate($ssid, $text, $dir, $tuid = 0, $detailed = True);
     QString     translateURL($ssid, $dir, $array, $url);
     QString     arrayResponse($ssid, $reqID, $from);
     bool        signUp($ssid, $login, $name, $sureName, $email, $mobile, $pass, $captcha='');
     QSring      userLogin($ssid, $user, $pass, $salt);
     bool        logout($ssid);
     bool        passChange($ssid, $oldPass, $oldPassSalt, $newPassMD5);
     bool        saveSuggestion($ssid, $dir, $sourceText, $suggestion);
     QVariantMap loadSuggestion($ssid, $dir, $sourceText);
     bool rankTranslation($ssid, $dir, $sourceText, $translated, $vote);
    */


};

#endif // TARGOMAN_APPS_MODULES_TARGOMANWEBSERVICE_H
