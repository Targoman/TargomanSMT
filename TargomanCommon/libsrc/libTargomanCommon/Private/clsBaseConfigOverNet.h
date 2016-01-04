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
 @author Behrooz Vedadian <vedadian@targoman.com>
 */

#ifndef TARGOMAN_COMMON_CONFIGURATION_PRIVATE_CLSBASECONFIGOVERNET_H
#define TARGOMAN_COMMON_CONFIGURATION_PRIVATE_CLSBASECONFIGOVERNET_H

#include "clsConfigManager_p.h"

namespace Targoman {
namespace Common {
namespace Configuration {
namespace Private {

TARGOMAN_ADD_EXCEPTION_HANDLER(exNoLogin, exConfigOverNet);
TARGOMAN_ADD_EXCEPTION_HANDLER(exInvalidAction, exConfigOverNet);
TARGOMAN_ADD_EXCEPTION_HANDLER(exInvalidLogin, exConfigOverNet);
TARGOMAN_ADD_EXCEPTION_HANDLER(exObjectNotFound, exConfigOverNet);
TARGOMAN_ADD_EXCEPTION_HANDLER(exInvalidUpdateSource, exConfigOverNet);
TARGOMAN_ADD_EXCEPTION_HANDLER(exInvalidData, exConfigOverNet);

class clsBaseConfigOverNet
{
protected:
    clsBaseConfigOverNet(clsConfigManagerPrivate& _configManager);
    QVariantList walk(bool _showDetails);
    QVariant query(const QString &_path);
    QVariantList bulkQuery(const QString &_parentPath,
                          bool _isRegex,
                          bool _showDetails,
                          bool _justUpdatable,
                          const QString &_justType,
                          const QString &_stripString);
    QVariant    set(const QString &_path, const QVariant& _newValue);
    stuPong     ssidPing(const QString &_ssid);

protected:
    clsConfigManagerPrivate&   ConfigManagerPrivate;
    QString                    ActorName;
    bool                       AllowedToChange;
    bool                       AllowedToView;
};

}
}
}
}

#endif // TARGOMAN_COMMON_CONFIGURATION_PRIVATE_CLSBASECONFIGOVERNET_H
