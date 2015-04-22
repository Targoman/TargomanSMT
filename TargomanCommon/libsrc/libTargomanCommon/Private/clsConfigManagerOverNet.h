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
#include "Network/clsBaseTCPServer.h"
#include "Network/clsBaseClientManager.h"
#include "Configuration/tmplConfigurable.h"
#include "clsConfigManager_p.h"

namespace Targoman {
namespace Common {
namespace Configuration {
namespace Private {

TARGOMAN_ADD_EXCEPTION_HANDLER(exConfigurationServer, exConfiguration);

class clsTCPServer;

/******************************************************************************/
class clsConfigNetworkServer : public QThread
{
    Q_OBJECT
public:
    clsConfigNetworkServer(clsConfigManagerPrivate& _configManager, bool _useWebSocketServer = false);
    ~clsConfigNetworkServer();

    void checkPortAvailability();
    inline void startListening(){this->CanStartListening = true;}

private slots:
    void slotShutdown();

signals:
    void sigValidateAgent(INOUT QString&        _user,
                          const QString&  _pass,
                          const QString&  _ip,
                          OUTPUT bool&           _canView,
                          OUTPUT bool&           _canChange);

    void sigRPC(const QString&            _funcName,
                INOUT  QVariantMap&       _arguments,
                OUTPUT QVariant&          _returnVal);
    void sigPing(JSONConversationProtocol::stuPong& _pong);

private:
    void run();

private:
    clsConfigManagerPrivate& ConfigManagerPrivate;
    static tmplConfigurable<int>  ListenPort;
    static tmplConfigurable<bool> AdminLocal;
    static tmplConfigurable<int>  MaxSessionTime;
    static tmplConfigurable<int>  MaxIdleTime;
    static tmplConfigurable<quint16>  MaxConnections;

    bool                                CanStartListening;
    QScopedPointer<QTcpServer>          TCPServer;
    QString&                            ActorUUID;

    friend class clsConfigManagerPrivate;
};

/******************************************************************************/
class clsTCPServer: public Network::clsBaseTCPServer
{
  Q_OBJECT

public:
  clsTCPServer(clsConfigManagerPrivate& _configManager, int _maxConnections);

private:
    Network::clsBaseClientManager* newClientInstance(const QString&, qint32);

signals:
    void sigValidateAgent(INOUT QString&        _user,
                          const QString&        _pass,
                          const QString&        _ip,
                          OUTPUT bool&          _canView,
                          OUTPUT bool&          _canChange);

    void sigRPC(const QString&            _funcName,
                INOUT  QVariantMap&       _arguments,
                OUTPUT QVariant&          _returnVal);
    void sigPing(JSONConversationProtocol::stuPong& _pong);

private slots:
    void slotNewClient(const QHostAddress& _host, quint16 _port, const QString& _id);
    void slotClientRemoved(const QHostAddress& _host, quint16 _port, const QString& _id);

private:
    clsConfigManagerPrivate& ConfigManagerPrivate;
    QString&                            ActorUUID;
};
/******************************************************************************/
class clsConnectionManager : public Network::clsBaseConnectionManager
{
public:
  clsConnectionManager(int _maxConnections) : clsBaseConnectionManager(_maxConnections){}
  const QByteArray clientId() const;
};

/******************************************************************************/
class clsClientManager: public Network::clsBaseClientManager
{
  Q_OBJECT
public:
  clsClientManager(const QByteArray& _id, clsConfigManagerPrivate& _configManager, QObject* _parent);

private:
  void sendError(enuReturnType::Type _type, const QString& _message);
  void sendResult(const QString &_data);
  void processIncomingData();

signals:
  void sigValidateAgent(INOUT QString&        _user,
                        const QString&        _pass,
                        const QString&        _ip,
                        OUTPUT bool&          _canView,
                        OUTPUT bool&          _canChange);

  void sigRPC(const QString&            _funcName,
              INOUT  QVariantMap&       _arguments,
              OUTPUT QVariant&          _returnVal);
  void sigPing(JSONConversationProtocol::stuPong& _pong);

private:
  QString                    ActorName;
  bool                       AllowedToChange;
  bool                       AllowedToView;
  clsConfigManagerPrivate&   ConfigManagerPrivate;
  QString&                            ActorUUID;
};

}
}
}
}
#endif // TARGOMAN_COMMON_CONFIGURATION_PRIVATE_CLSCONFIGMANAGEROVERNET_H
