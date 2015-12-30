/**
 @author S. Mohammad M. Ziabary <ziabary@targoman.com>
 */

#ifndef QJSONRPCHTTPSERVERMULTITHREADED_H
#define QJSONRPCHTTPSERVERMULTITHREADED_H

#include <QSslConfiguration>
#include <QTcpServer>
#include <QHash>

#include "libQJsonRPC/qjsonrpcabstractserver.h"
#include "libQJsonRPC/qjsonrpcglobal.h"

class QJsonRpcHttpServerMultiThreadedPrivate;

class QJsonRpcHttpServerMultiThreaded: public QTcpServer, public QJsonRpcAbstractServer
{
    Q_OBJECT
public:
    /*virtual bool addService(QJsonRpcService *service){
        return QJsonRpcAbstractServer::addService(service);
    }*/
    QJsonRpcHttpServerMultiThreaded(quint16 _maxConcurrentClients, QObject *parent = 0);
    ~QJsonRpcHttpServerMultiThreaded();

    QSslConfiguration sslConfiguration() const;
    void setSslConfiguration(const QSslConfiguration &config);

    virtual int connectedClientCount() const;

Q_SIGNALS:
    void clientConnected();
    void clientDisconnected();

public Q_SLOTS:
    virtual void notifyConnectedClients(const QJsonRpcMessage &message);
    virtual void notifyConnectedClients(const QString &method, const QJsonArray &params);

protected:
#if QT_VERSION >= 0x050000
    virtual void incomingConnection(qintptr socketDescriptor);
#else
    virtual void incomingConnection(int socketDescriptor);
#endif
private:
    Q_DECLARE_PRIVATE(QJsonRpcHttpServerMultiThreaded)
    Q_DISABLE_COPY(QJsonRpcHttpServerMultiThreaded)
    QScopedPointer<QJsonRpcHttpServerMultiThreadedPrivate> d_ptr;
    Q_PRIVATE_SLOT(d_func(), void clientFinished())
};

#endif // QJSONRPCHTTPSERVERMULTITHREADED_H
