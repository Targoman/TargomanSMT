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
 @author Behrooz Vedadian <vedadian@gmail.com>
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
