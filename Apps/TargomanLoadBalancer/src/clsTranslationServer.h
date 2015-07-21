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

#ifndef TARGOMAN_APPS_CLSSERVERCONNECTION_H
#define TARGOMAN_APPS_CLSSERVERCONNECTION_H

#include <QtNetwork/QTcpSocket>
#include "Configs.h"
#include "libTargomanCommon/JSONConversationProtocol.h"

namespace Targoman {
namespace Apps{

class clsTranslationServer : public QObject
{
    Q_OBJECT
public:

    clsTranslationServer(size_t _configIndex = 0);
    void connect();
    bool isConnected();

    qint64 sendRequest(const QString& _name = "", const QVariantMap& _args = QVariantMap());

    inline quint32 configIndex(){return this->ConfigIndex;}

signals:
    void sigNextRequest();
    void sigResponse(Common::JSONConversationProtocol::stuResponse _response);
    void sigDisconnected();

private slots:
    void slotConnected();
    void slotReadyRead();
    void slotDisconnected();

public:
    quint16            TotalScore;
    QTime              LastRequestTime;
    QReadWriteLock     RWLock;
    Common::JSONConversationProtocol::stuResponse         Response;

private:
    const gConfigs::stuServer& Configs;
    QString    LastRequestUUID;
    QTcpSocket Socket;
    quint32    ConfigIndex;
    bool       LoggedIn;
};

}
}

#endif // TARGOMAN_APPS_CLSSERVERCONNECTION_H
