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

#include <QCoreApplication>
#include <QTimer>
#include "libTargomanCommon/Macros.h"
#include "libTargomanCommon/Configuration/ConfigManager.h"
#include "libTargomanCommon/Configuration/tmplConfigurable.h"
#include "libTargomanCommon/SimpleAuthentication.h"
#include "libTargomanCommon/Configuration/Validators.hpp"

const char* LicenseStr =
        "Targoman Simple Authentication Manager Ver: %2 Build %3\n"
        "Published under the terms of GNU Lesser General Public License version 3\n\n"
        "%1 [-f File] [-a|-g|-r] [Arguments]\n"
        "Arguments: \n";

using namespace Targoman;
using namespace Targoman::Common;
using namespace Targoman::Common::Configuration;

TARGOMAN_DEFINE_ENUM(enuAction,
                     Add,
                     Remove,
                     Change,
                     Unknown);

enuAction::Type SelectedAction = enuAction::Unknown;

tmplConfigurable<bool> ActionAdd(
        clsConfigPath("Actions/Add"),
        "Add new user to specified file",
        false,
        [] (const intfConfigurable& _item, QString& _errorMessage) {
            if (_item.toVariant().toBool() == false)
                return true;
            if (SelectedAction != enuAction::Unknown){
                _errorMessage = "Just one action can be selected ";
                return false;
            }

            SelectedAction = enuAction::Add;
            return true;
        },
        "a","","add",enuConfigSource::Arg);
tmplConfigurable<bool> ActionRemove(
        clsConfigPath("Actions/Remove"),
        "Delete user from specified file",
        false,
            [] (const intfConfigurable& _item, QString& _errorMessage) {
            if (_item.toVariant().toBool() == false)
                return true;
            if (SelectedAction != enuAction::Unknown){
                _errorMessage = "Just one action can be selected ";
                return false;
            }
            SelectedAction = enuAction::Remove;
            return true;
        },
        "r","","remove",enuConfigSource::Arg);
tmplConfigurable<bool> ActionChangeParams(
        clsConfigPath("Actions/ChangeParams"),
        "Change Parameters or password of specified user",
        false,
        [] (const intfConfigurable& _item, QString& _errorMessage) {
            if (_item.toVariant().toBool() == false)
                return true;
            if (SelectedAction != enuAction::Unknown){
                _errorMessage = "Just one action can be selected ";
                return false;
            }
            SelectedAction = enuAction::Change;
            return true;
        },
        "g","","change",enuConfigSource::Arg);

tmplConfigurable<FilePath_t> UserInfoFile(
        clsConfigPath("File/Path"),
        "File to where store information",
        "./tsa.ini",
        Validators::tmplPathAccessValidator<
            (enuPathAccess::Type)(enuPathAccess::Writeatble),
            true>,
        "f",
        "FILE_PATH",
        "file");

tmplConfigurable<QString> UserName(
        clsConfigPath("Arguments/User"),
        "Username",
        "",
        ReturnTrueCrossValidator,
        "u",
        "USER_NAME",
        "user",enuConfigSource::Arg);

tmplConfigurable<QString> Password(
        clsConfigPath("Arguments/Pass"),
        "Password for specified user read from arguments set to - to read from stdin",
        "-",
        ReturnTrueCrossValidator,
        "p",
        "PASSWORD",
        "pass");

tmplConfigurable<QString> CIDR(
        clsConfigPath("Arguments/CIDR"),
        "Valid IP-Range from which login is allowed in CIDR format",
        "0.0.0.0/0",
        ReturnTrueCrossValidator,
        "i",
        "CIDR",
        "cidr",enuConfigSource::Arg);

tmplConfigurable<QString> Access(
        clsConfigPath("Arguments/Access"),
        "Access can be r = ReadOnly or w = ReadWrite ",
        "r",
        [] (const intfConfigurable& _item, QString& _errorMessage) {
            if (_item.toVariant().toString() != "r" &&
                _item.toVariant().toString() != "w" ){
                _errorMessage = "Invalid Access Specified";
                return false;
            }
            return true;
        },
        "s",
        "ACCESS",
        "access",enuConfigSource::Arg
        );


int main(int _argc, char *_argv[])
{
    //TODO handle backspace on password entry
    try{
        TARGOMAN_IO_SETTINGS.setDefault(0,4);
        QCoreApplication App(_argc, _argv);

        Configuration::ConfigManager::instance().init(
                    QString(LicenseStr).arg(_argv[0]).arg(TARGOMAN_M2STR(PROJ_VERSION)).arg(__DATE__),
                    App.arguments().mid(1),
                    [](){},
                    true
                    );
        TARGOMAN_IO_SETTINGS.setDefault(0,4);
        TARGOMAN_IO_SETTINGS.ShowColored = true;

        if (SelectedAction == enuAction::Unknown)
            throw exTargomanBase("No action specified");

        if(UserName.value().isEmpty())
            throw exTargomanBase("No user specified");

        SimpleAuthentication::UserInfoFile.setFromVariant(UserInfoFile.value());
        QString PassString;
        if (Password.value() == "-"){
            if (SelectedAction == enuAction::Change){
                PassString = CmdIO::getPassword("Password (Empty to keep unchanged)");
                if (PassString.size())
                    if (PassString != CmdIO::getPassword("Retype"))
                        throw exTargomanBase("Passwords do not match");
            }else{
                PassString = CmdIO::getPassword("Password");
                if (PassString.isEmpty())
                    throw exTargomanBase("Invalid Empty Password");
                if (PassString != CmdIO::getPassword("Retype"))
                    throw exTargomanBase("Passwords do not match");
            }
        }else{
            PassString = Password.value();
        }


        switch (SelectedAction){
        case enuAction::Add:
            SimpleAuthentication::registerUser(UserName.value(),
                                               PassString,
                                               CIDR.value(),
                                               true,
                                               Access.value().toUpper() == "W");
            TargomanHappy(1, "User <"<<UserName.value()<<"> Added to "<<UserInfoFile.value());
            break;
        case enuAction::Remove:
            SimpleAuthentication::removeUser(UserName.value());
            TargomanHappy(1, "User <"<<UserName.value()<<"> Removed from "<<UserInfoFile.value()<<"!!!");
            break;
        case enuAction::Change:
            SimpleAuthentication::updateUser(UserName.value(),
                                             PassString,
                                             CIDR.value(),
                                             true,
                                             Access.value().toUpper() == "W");
            TargomanHappy(1, "User <"<<UserName.value()<<"> updated in "<<UserInfoFile.value());
            break;
        default:
            break;
        }

        return 0;
    }catch(exTargomanBase& e){
        TargomanError(e.what());
    }catch (std::exception &e){
        TargomanError(e.what());
    }catch(...){
        TargomanError("FATAL Unrecognized exception");
    }
    return -1;
}


