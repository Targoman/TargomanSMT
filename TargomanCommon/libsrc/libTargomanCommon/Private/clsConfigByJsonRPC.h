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

#ifndef TARGOMAN_COMMON_CONFIGURATION_PRIVATE_CLSCONFIGBYJSONRPC_H
#define TARGOMAN_COMMON_CONFIGURATION_PRIVATE_CLSCONFIGBYJSONRPC_H

#ifdef WITH_QJsonRPC

#include <QScopedPointer>
#include <QTcpServer>
#include "libQJsonRPC/qjsonrpcservice.h"
#include "libQJsonRPC/qjsonrpcabstractserver.h"
#include "clsBaseConfigOverNet.h"
#include "clsConfigManager_p.h"
#include "intfConfigManagerOverNet.hpp"

namespace Targoman {
namespace Common {
namespace Configuration {
namespace Private {

class clsConfigByJsonRPC : public intfConfigManagerOverNet
{
public:
    enum enuType{
        TCP,
        HTTP
    };

public:
    clsConfigByJsonRPC(enuType _type,clsConfigManagerPrivate& _configManager);
    ~clsConfigByJsonRPC();
    inline bool addService(QJsonRpcService *_service){
        return dynamic_cast<QJsonRpcAbstractServer*>(this->Server.data())->addService(_service);
    }
};
/*************************************************************************/

class clsConfigurationService : public QJsonRpcService, public clsBaseConfigOverNet
{
    Q_OBJECT
    Q_CLASSINFO("serviceName", "Configuration")
public:
    clsConfigurationService(clsConfigManagerPrivate& _configManager);

public slots:
    bool login(QString _login, QString _pass="");
    QVariantList walk(bool _showDetails);
    QVariant query(QString _path);
    QVariantList bulkQuery(QString _parentPath,
                          bool _isRegex,
                          bool _showDetails,
                          bool _justUpdatable,
                          QString _justType,
                          QString _stripString);
    QVariant    set(QString _path, QVariant _value);
    stuPong     ssidPing(QString _ssid);
    quint8 simpleping();
};

}
}
}
}

#endif //WITH_QJsonRPC
#endif // TARGOMAN_COMMON_CONFIGURATION_PRIVATE_CLSCONFIGBYJSONRPC_H
