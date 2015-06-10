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

#include "SimpleAuthentication.h"
#include <QCryptographicHash>
#include <QSettings>
#include <QtNetwork/QHostAddress>
#include "Configuration/Validators.hpp"

namespace Targoman {
namespace Common {

using namespace Configuration;

tmplConfigurable<FilePath_t> SimpleAuthentication::UserInfoFile(
        "Authentication/UserInfoFile",
        "Path to file containing valid usernames and passwords",
        "",
        Validators::tmplPathAccessValidator<
            (enuPathAccess::Type)(enuPathAccess::File | enuPathAccess::Readable),
            false>,
        "","",""/*,
        enuConfigSource::File,
        false*/
        );

QRegExp rxValidUser("^[a-z][a-z0-9_]+");
QRegExp rxValidCIDR("^(([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])\\.){3}([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])(\\/([0-9]|[1-2][0-9]|3[0-2]))$");

QString SimpleAuthentication::hashPass(const QString& _user, const QString &_pass, const QString &_salt)
{
    if (_salt.size())
        return QString("s:%1:%2").arg(
                    _salt).arg(
                    QCryptographicHash::hash((_salt +
                                             SimpleAuthentication::hashPass(_user, _pass)).toUtf8(),
                                             QCryptographicHash::Md5).toHex().constData());
    else
        return QCryptographicHash::hash((_user.toLower() + _pass).toUtf8(),
                                        QCryptographicHash::Md5).toHex().constData();
}

SimpleAuthentication::stuLoginInfo SimpleAuthentication::checkLogin(const QString &_user, const QString &_pass, const QString &_ip)
{
    QStringList PassParts = _pass.split(":");
    if (PassParts.first() == "s" && PassParts.size() == 3)
        return SimpleAuthentication::checkLogin(_user, PassParts.at(2), PassParts.at(1), _ip);

    throw exSimpleAuthentication("Unsupported password protocol");
}

SimpleAuthentication::stuLoginInfo SimpleAuthentication::checkLogin(const QString &_user,
                                                                    const QString &_pass,
                                                                    const QString &_salt,
                                                                    const QString &_ip)
{
    QSettings Settings(SimpleAuthentication::UserInfoFile.value(), QSettings::IniFormat);
    if (Settings.contains(_user.toLower() + "/Pass") == false)
        throw exSimpleAuthentication("Invalid user/password");

    QString Hash = QCryptographicHash::hash(
                (_salt + Settings.value(_user.toLower() + "/Pass").toString()).toUtf8(),
                QCryptographicHash::Md5).toHex().constData();
    if (Hash != _pass)
        throw exSimpleAuthentication("Invalid user/password");

    if (QHostAddress(_ip).isInSubnet(
                QHostAddress::parseSubnet(Settings.value(_user.toLower() + "/CIDR").toString())) == false)
        throw exSimpleAuthentication("User <" + _user + "> is not allowed to login from: " + _ip);

    return stuLoginInfo(Settings.value(_user.toLower() + "/CanView").toBool(),
                        Settings.value(_user.toLower() + "/CanChange").toBool());
}

void SimpleAuthentication::registerUser(const QString &_user, const QString &_pass, const QString &_cidr, bool _canView, bool _canChange)
{
    QSettings Settings(SimpleAuthentication::UserInfoFile.value(), QSettings::IniFormat);
    if (Settings.isWritable() == false)
        throw exSimpleAuthentication("Unable to open <" +
                                     SimpleAuthentication::UserInfoFile.value() +
                                     "> for writing");
    QString User = _user.toLower();
    if (rxValidUser.exactMatch(User) == false)
        throw exSimpleAuthentication("Invalid username: <"+ User + ">");
    if (Settings.contains(User+"/"+"Pass"))
        throw exSimpleAuthentication("User <" + _user + "> Was registered before.");

    if (rxValidCIDR.exactMatch(_cidr) == false)
        throw exSimpleAuthentication("Invalid CIDR format");

    Settings.setValue(User + "/Pass",
                      SimpleAuthentication::hashPass(User, _pass));
    Settings.setValue(User + "/CIDR", _cidr);
    Settings.setValue(User + "/CanView", _canView);
    Settings.setValue(User + "/CanChange", _canChange);
}

void SimpleAuthentication::removeUser(const QString &_user)
{
    QSettings Settings(SimpleAuthentication::UserInfoFile.value(), QSettings::IniFormat);
    if (Settings.isWritable() == false)
        throw exSimpleAuthentication("Unable to open <" +
                                     SimpleAuthentication::UserInfoFile.value() +
                                     "> for writing");
    QString User = _user.toLower();
    if (Settings.contains(User+"/"+"Pass") == false)
        throw exSimpleAuthentication("User <" + _user + "> Not found.");
    Settings.remove(User + "/Pass");
    Settings.remove(User + "/CIDR");
    Settings.remove(User + "/CanView");
    Settings.remove(User + "/CanChange");
}

void SimpleAuthentication::updateUser(const QString &_user,
                                      const QString &_pass,
                                      const QString &_cidr,
                                      bool _canView,
                                      bool _canChange)
{
    QSettings Settings(SimpleAuthentication::UserInfoFile.value(), QSettings::IniFormat);
    if (Settings.isWritable() == false)
        throw exSimpleAuthentication("Unable to open <" +
                                     SimpleAuthentication::UserInfoFile.value() +
                                     "> for writing");
    QString User = _user.toLower();
    if (Settings.contains(User+"/"+"Pass") == false)
        throw exSimpleAuthentication("User <" + _user + "> Not found.");

    if (_cidr.size() && rxValidCIDR.exactMatch(_cidr) == false)
        throw exSimpleAuthentication("Invalid CIDR format");

    if (_pass.size())
        Settings.setValue(User + "/Pass",
                          SimpleAuthentication::hashPass(User, _pass));
    if (_cidr.size())
        Settings.setValue(User + "/CIDR", _cidr);

    Settings.setValue(User + "/CanView", _canView);
    Settings.setValue(User + "/CanChange", _canChange);
}

}
}
