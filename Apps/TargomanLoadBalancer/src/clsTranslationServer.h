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

    clsTranslationServer(const QString& _dir,
                         size_t _configIndex,
                         const QString& _requestRPC = "",
                         const QVariantMap& _requestArgs = QVariantMap());

    ~clsTranslationServer();

    void connect();
    void disconnectFromHost();
    bool isConnected();

    inline void setRequestRPC(const QString& _requestRPC){this->RequestRPC = _requestRPC;}
    inline const QString& requestRPC() const {return this->RequestRPC;}
    inline void setRequestArgs(const QVariantMap& _requestArgs){this->RequestArgs = _requestArgs;}
    inline const QVariantMap& requestArgs() const {return this->RequestArgs;}

    inline quint32 configIndex(){return this->ConfigIndex;}
    inline const QString& dir(){return this->Dir;}
    inline bool isLoggedIn() {return this->LoggedIn;}
    bool getSafeResponse(Common::JSONConversationProtocol::stuResponse& _response);

    inline quint16 totalScore() {
        return this->TotalScore;
    }
    void resetScore();
    void reset();
    void updateStatistics(quint32 _load1min, quint32 _load15min, quint32 _freeMem, quint32 _translationQueue, quint16 _score);

    qint64 sendRequest(const QString &_rpc, const QVariantMap &_args = QVariantMap());
signals:
    void sigResponse(Common::JSONConversationProtocol::stuResponse _response);
    void sigDisconnected();
    void sigReadyForFirstRequest();

public slots:
    qint64 slotSendPredefinedRequest();

private slots:
    void slotConnected();
    void slotReadyRead();
    void slotDisconnected();
    void slotError(QAbstractSocket::SocketError _socketError);

public:
    quint16            TotalScore;
    QTime              LastRequestTime;
    Common::JSONConversationProtocol::stuResponse         Response;

private:
    QMutex     ResponseLock;
    bool               IsResponseReady;
    gConfigs::stuServer& Configs;
    QString    LastRequestUUID;
    QScopedPointer<QTcpSocket> Socket;
    quint32    ConfigIndex;
    QString    Dir;
    bool       LoggedIn;
    QString     RequestRPC;
    QVariantMap RequestArgs;
};

}
}

#endif // TARGOMAN_APPS_CLSSERVERCONNECTION_H
