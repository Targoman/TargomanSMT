#ifndef TESTHTTPSERVER_H
#define TESTHTTPSERVER_H

#include <QTcpSocket>
#include <QTcpServer>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include "http-parser/http_parser.h"

class TestHttpServerRequestHandler;

class TestHttpServerPrivate
{
public:
    TestHttpServerPrivate();
    virtual ~TestHttpServerPrivate();

    // http parsing
    http_parser *httpParser;
    http_parser_settings httpParserSettings;
    QByteArray currentHeaderField;
    QByteArray currentHeaderValue;

    static int onMessageBegin(http_parser *parser);
    static int onUrl(http_parser *parser, const char *at, size_t length);
    static int onHeaderField(http_parser *parser, const char *at, size_t length);
    static int onHeaderValue(http_parser *parser, const char *at, size_t length);
    static int onHeadersComplete(http_parser *parser);
    static int onBody(http_parser *parser, const char *at, size_t length);
    static int onMessageComplete(http_parser *parser);

    // private slots
    void _q_socketReadyRead();
    void _q_socketError(QAbstractSocket::SocketError error);
    void _q_handleRequest();

    TestHttpServerRequestHandler *requestHandler;
    QPointer<QTcpSocket> socket;

    QNetworkAccessManager::Operation operation;
    QNetworkRequest request;
    QByteArray body;
};

class TestHttpServerRequestHandler
{
public:
    virtual ~TestHttpServerRequestHandler() {}
    virtual QByteArray handleRequest(QNetworkAccessManager::Operation operation,
                                     const QNetworkRequest &request, const QByteArray &body) = 0;
};

class TestHttpServerPrivate;
class TestHttpServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit TestHttpServer(QObject *parent = 0);
    TestHttpServer(TestHttpServerRequestHandler *requestHandler, QObject *parent = 0);
    virtual ~TestHttpServer();

    TestHttpServerRequestHandler *requestHandler() const;
    void setRequestHandler(TestHttpServerRequestHandler *requestHandler);

    void setResponseData(const QByteArray &data);

protected:
#if QT_VERSION >= 0x050000
    virtual void incomingConnection(qintptr socketDescriptor);
#else
    virtual void incomingConnection(int socketDescriptor);
#endif

private:
    Q_DISABLE_COPY(TestHttpServer)
    Q_DECLARE_PRIVATE(TestHttpServer)
    QScopedPointer<TestHttpServerPrivate> d_ptr;

    Q_PRIVATE_SLOT(d_func(), void _q_socketReadyRead())
    Q_PRIVATE_SLOT(d_func(), void _q_socketError(QAbstractSocket::SocketError))

};

#endif
