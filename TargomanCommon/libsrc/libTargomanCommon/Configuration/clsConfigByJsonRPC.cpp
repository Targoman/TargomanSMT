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

#ifdef WITH_QJsonRPC

#include "Private/clsConfigByJsonRPC.h"
#include "libQJsonRPC/qjsonrpchttpservermultithreaded.h"
#include "libQJsonRPC/qjsonrpctcpserver.h"
#include "libQJsonRPC/qjsonrpcsocket.h"

namespace Targoman {
namespace Common {
namespace Configuration {
namespace Private {


clsConfigByJsonRPC::clsConfigByJsonRPC(enuType _type, clsConfigManagerPrivate &_configManager) :
    intfConfigManagerOverNet(_type == TCP ?
                                 (QTcpServer*)new QJsonRpcTcpServer :
                                 ( QTcpServer*)new QJsonRpcHttpServerMultiThreaded(MaxConnections.value()))
{
    ((QJsonRpcAbstractServer*)this)->addService(new clsConfigurationService(_configManager));
}

clsConfigByJsonRPC::~clsConfigByJsonRPC()
{
    //Just to suppress compiler error using QScopedPointer
}

clsConfigurationService::clsConfigurationService(clsConfigManagerPrivate &_configManager):
    clsBaseConfigOverNet(_configManager)
{ }

bool clsConfigurationService::login(QString _login, QString _pass)
{

    if (_pass.isEmpty())
        this->ActorName = "$SSID$" + _login;
    else
        this->ActorName = _login;

    if (this->ActorName.isEmpty())
        this->ActorName = "UNKNOWN";

    QTcpSocket* Socket = qobject_cast<QTcpSocket*>(this->currentRequest().socket()->device());
    if (!Socket)
        throw exConfigOverNet("Invalid socket type");

    emit this->ConfigManagerPrivate.Parent.sigValidateAgent(
                this->ActorName,
                _pass,
                Socket->peerAddress().toString(),
                this->AllowedToView,
                this->AllowedToChange);

    if (this->AllowedToView) {
        TargomanLogInfo(5, QString("User <%1> Logged in with %2 Access").arg(
                            this->ActorName).arg(
                            this->AllowedToChange ? "ReadWrite" : "ReadOnly"));

        if (this->AllowedToChange)
            return 3;
        else
            return 1;
    } else if (this->ActorName.isEmpty()) {
        TargomanLogWarn(6, "Attemp to login from <"<<
                        Socket->peerAddress().toString()<<":"<<
                        Socket->peerPort()<<"> Failed");
        throw exInvalidLogin("Invalid User/Password");
    } else {
        TargomanLogWarn(6,
                        QString("User: %1 attemped to Login but not enough access").arg(
                            this->ActorName));
        throw exInvalidLogin("Not enough access");
    }
}

QVariantList clsConfigurationService::walk(bool _showDetails)
{
    return clsBaseConfigOverNet::walk(_showDetails);
}

QVariant clsConfigurationService::query(QString _path)
{
    return clsBaseConfigOverNet::query(_path);
}

QVariantList clsConfigurationService::bulkQuery(QString _parentPath,
                                               bool _isRegex,
                                               bool _showDetails,
                                               bool _justUpdatable,
                                               QString _justType,
                                               QString _stripString)
{
    return clsBaseConfigOverNet::bulkQuery(
                _parentPath, _isRegex, _showDetails, _justUpdatable, _justType,_stripString);
}

QVariant clsConfigurationService::set(QString _path, QVariant _value)
{
    return clsBaseConfigOverNet::set(_path, _value);
}

stuPong clsConfigurationService::ssidPing(QString _ssid)
{
    return clsBaseConfigOverNet::ssidPing(_ssid);
}

quint8 clsConfigurationService::simpleping()
{
    return 2;
}

}
}
}
}
#endif //WITH_QJsonRPC
