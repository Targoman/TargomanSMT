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

#ifndef TARGOMAN_COMMON_CONFIGURATION_PRIVATE_CLSCONFIGMANAGEROVERNET_H
#define TARGOMAN_COMMON_CONFIGURATION_PRIVATE_CLSCONFIGMANAGEROVERNET_H

#include <QThread>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QTime>
#include "Configuration/tmplConfigurable.h"
#include "clsConfigManager_p.h"

namespace Targoman {
namespace Common {
namespace Configuration {
namespace Private {

TARGOMAN_ADD_EXCEPTION_HANDLER(exConfigurationServer, exConfiguration);

/******************************************************************************/
class clsConfigNetworkServer : private QTcpServer
{
    Q_OBJECT
public:
    clsConfigNetworkServer(clsConfigManagerPrivate& _configManager);
    ~clsConfigNetworkServer();
    void start(bool _justCheck = false);
    bool check();
    bool isActive(){
        return this->ListenPort.value() > 0;
    }

private:
    void incomingConnection(qintptr _socketDescriptor);

private:
    clsConfigManagerPrivate&          ConfigManagerPrivate;
    static tmplConfigurable<int>      ListenPort;
    static tmplConfigurable<bool>     WaitPortReady;
    static tmplConfigurable<bool>     AdminLocal;
    static tmplConfigurable<int>      MaxSessionTime;
    static tmplConfigurable<int>      MaxIdleTime;
    static tmplConfigurable<quint16>  MaxConnections;

    QString&                            ActorUUID;
};

/******************************************************************************/
class clsClientThread : public QThread
{
  Q_OBJECT
public:
  clsClientThread(qintptr _socketDescriptor,
                  clsConfigManagerPrivate& _configManager,
                  QObject* _parent);

private:
  void run();
  void sendError(enuReturnType::Type _type, const QString& _message);
  void sendResult(const QString &_data);

private slots:
  void slotReadyRead();
  void slotDisconnected();

signals:
  void error(QTcpSocket::SocketError socketerror);

private:
  qintptr                    SocketDescriptor;
  QString                    ActorName;
  bool                       AllowedToChange;
  bool                       AllowedToView;
  clsConfigManagerPrivate&   ConfigManagerPrivate;
  QString&                   ActorUUID;
  QTcpSocket*                Socket;
};

}
}
}
}
#endif // TARGOMAN_COMMON_CONFIGURATION_PRIVATE_CLSCONFIGMANAGEROVERNET_H
