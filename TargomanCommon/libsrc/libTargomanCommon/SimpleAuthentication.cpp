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

namespace Targoman {
namespace Common {

QString SimpleAuthentication::hashPass(const QString &_pass, const QString &_salt)
{
    return QString("s:%1:%2").arg(
                _salt).arg(
                QCryptographicHash::hash(_salt.toUtf8() +
                                         QCryptographicHash::hash(_pass.toUtf8(), QCryptographicHash::Md5),
                                         QCryptographicHash::Md5).constData());
}

}
}
