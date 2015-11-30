/**
 @author S. Mohammad M. Ziabary <ziabary@targoman.com>
 */

#ifndef QJSONRPCHTTPSERVERMULTITHREADED_P_H
#define QJSONRPCHTTPSERVERMULTITHREADED_P_H
#include <QHash>
#include <QSslSocket>
#include <QSslConfiguration>
#include <QThread>

#include "qjsonrpcsocket.h"
#include "qjsonrpcmessage.h"
#include "qjsonrpcserviceprovider.h"
#include "qjsonrpcabstractserver_p.h"
#include "qjsonrpchttpserver_p.h"

#include "Private/http-parser/http_parser.h"

class QJsonRpcHttpServerMultiThreaded;
class QJsonRpcHttpServerMultiThreadedPrivate : public QJsonRpcAbstractServerPrivate
{
public:
    QJsonRpcHttpServerMultiThreadedPrivate(quint16 maxConcurrent, QJsonRpcHttpServerMultiThreaded *qq)
        : q_ptr(qq),
          connectedClients(0),
          maxConcurrentConnections(maxConcurrent)
    {
    }

    QJsonRpcHttpServerMultiThreaded * const q_ptr;
    QSslConfiguration sslConfiguration;
    quint16           connectedClients;
    quint16           maxConcurrentConnections;

    Q_DECLARE_PUBLIC(QJsonRpcHttpServerMultiThreaded)

    //slot
    void clientFinished();
};

/******************************************************************************/
class clsJsonRpcHttpClientThread : public QThread
{
  Q_OBJECT
public:
  clsJsonRpcHttpClientThread(qintptr socketDescriptor,
                             const QSslConfiguration &config,
                             QJsonRpcServiceProvider *_serviceProvider);
  ~clsJsonRpcHttpClientThread();

private:
  void run();

private Q_SLOTS:
  void processIncomingMessage(const QJsonRpcMessage &message);
  void slotDisconnected();

signals:
  void error(QTcpSocket::SocketError socketerror);

private:
  QSslConfiguration sslConfiguration;

  qintptr                    socketDescriptor;
  QJsonRpcSocket*            rpcSocket;
  QJsonRpcHttpServerSocket*  socket;
  QJsonRpcServiceProvider*   serviceProvider;
};

#endif // QJSONRPCHTTPSERVERMULTITHREADED_P_H
