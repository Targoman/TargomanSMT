/**
 @author S. Mohammad M. Ziabary <ziabary@targoman.com>
 */

#include <QStringList>
#include <QDateTime>

#if QT_VERSION >= 0x050000
#include <QJsonDocument>
#else
#include "json/qjsondocument.h"
#endif

#include "qjsonrpcsocket.h"
#include "qjsonrpcmessage.h"
#include "Private/qjsonrpchttpservermultithreaded_p.h"
#include "qjsonrpchttpservermultithreaded.h"

QJsonRpcHttpServerMultiThreaded::QJsonRpcHttpServerMultiThreaded(quint16 maxConcurrentClients, QObject *parent)
    : QTcpServer(parent),
      d_ptr(new QJsonRpcHttpServerMultiThreadedPrivate(maxConcurrentClients, this))
{

}

QJsonRpcHttpServerMultiThreaded::~QJsonRpcHttpServerMultiThreaded()
{}

QSslConfiguration QJsonRpcHttpServerMultiThreaded::sslConfiguration() const
{
    Q_D(const QJsonRpcHttpServerMultiThreaded);
    return d->sslConfiguration;
}

void QJsonRpcHttpServerMultiThreaded::setSslConfiguration(const QSslConfiguration &config)
{
    Q_D(QJsonRpcHttpServerMultiThreaded);
    d->sslConfiguration = config;
}

int QJsonRpcHttpServerMultiThreaded::connectedClientCount() const
{
    Q_D(const QJsonRpcHttpServerMultiThreaded);
    return d->connectedClients;
}

void QJsonRpcHttpServerMultiThreaded::notifyConnectedClients(const QJsonRpcMessage &message)
{
    Q_UNUSED(message);
}

void QJsonRpcHttpServerMultiThreaded::notifyConnectedClients(const QString &method, const QJsonArray &params)
{
    Q_UNUSED(method);
    Q_UNUSED(params);
}

#if QT_VERSION >= 0x050000
void QJsonRpcHttpServerMultiThreaded::incomingConnection(qintptr socketDescriptor)
#else
void QJsonRpcHttpServerMultiThreaded::incomingConnection(int socketDescriptor)
#endif
{
    Q_D(QJsonRpcHttpServerMultiThreaded);
    if(d->connectedClients > d->maxConcurrentConnections){
        QTcpSocket* Socket = new QTcpSocket;
        qJsonRpcDebug() << Q_FUNC_INFO << "Ignoring connection as max connections has reached";
        Socket->setSocketDescriptor(socketDescriptor);
        Socket->close();
        Socket->deleteLater();
        return;
    }

    clsJsonRpcHttpClientThread* CLT = new clsJsonRpcHttpClientThread(socketDescriptor,
                                               d->sslConfiguration,
                                               this);

    connect(CLT, SIGNAL(finished()), this, SLOT(clientFinished()));

    CLT->start();
    ++d->connectedClients;
    emit clientConnected();
}

/***********************************************************************************/
void QJsonRpcHttpServerMultiThreadedPrivate::clientFinished()
{
    Q_Q(QJsonRpcHttpServerMultiThreaded);

    clsJsonRpcHttpClientThread* CLT = qobject_cast<clsJsonRpcHttpClientThread*>(q->sender());
    CLT->deleteLater();
    --this->connectedClients;
    emit q->clientDisconnected();
}

/***********************************************************************************/

clsJsonRpcHttpClientThread::clsJsonRpcHttpClientThread(qintptr socketDescriptor,
                                                       const QSslConfiguration &config,
                                                       QJsonRpcServiceProvider *_serviceProvider) :
    sslConfiguration(config),
    socketDescriptor(socketDescriptor),
    serviceProvider(_serviceProvider)
{
}

clsJsonRpcHttpClientThread::~clsJsonRpcHttpClientThread()
{
}

void clsJsonRpcHttpClientThread::run()
{
    this->socket = new QJsonRpcHttpServerSocket();
    if (!this->socket->setSocketDescriptor(socketDescriptor)) {
        qJsonRpcDebug() << Q_FUNC_INFO << "unable to set socket descriptor";
        emit error(this->socket->error());
        this->socket->deleteLater();
        return;
    }

    if (!this->sslConfiguration.isNull()) {
        this->socket->setSslConfiguration(this->sslConfiguration);
        this->socket->startServerEncryption();
        // connect ssl error signals etc

        // NOTE: unsafe
        connect(this->socket, SIGNAL(sslErrors(QList<QSslError>)),
                this->socket, SLOT(ignoreSslErrors()), Qt::DirectConnection);
    }

    connect(this->socket, SIGNAL(disconnected()), this, SLOT(slotDisconnected()), Qt::DirectConnection);
    connect(this->socket, SIGNAL(messageReceived(QJsonRpcMessage)),
            this, SLOT(processIncomingMessage(QJsonRpcMessage)), Qt::DirectConnection);
    this->rpcSocket = new QJsonRpcHttpServerRpcSocket(this->socket, this->socket);
    this->exec();
}

void clsJsonRpcHttpClientThread::processIncomingMessage(const QJsonRpcMessage &message)
{
    this->serviceProvider->processMessage(this->rpcSocket, message);
}

void clsJsonRpcHttpClientThread::slotDisconnected()
{
    qJsonRpcDebug() << Q_FUNC_INFO << "client "<<this->socketDescriptor<<" disconnected";

    this->rpcSocket->deleteLater();
    this->socket->deleteLater();
    exit(0);
}

#include "moc_qjsonrpchttpservermultithreaded.cpp"

