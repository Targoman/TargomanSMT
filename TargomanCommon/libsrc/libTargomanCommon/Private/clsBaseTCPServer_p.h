/*************************************************************************
 * Copyright © 2012-2015, Targoman.com
 *
 * Published under the terms of TCRL(Targoman Community Research License)
 * You can find a copy of the license file with distributed source or
 * download it from http://targoman.com/License.txt
 *
 * Originally based on Qt Torrent example.
 * http://doc.qt.digia.com/qt-maemo/network-torrent.html
 *************************************************************************/
/**
 @author S. Mohammad M. Ziabary <smm@ziabary.com>
 */

#ifndef TARGOMAN_COMMON_NETWORK_PRIVATE_CLSBASETCPSERVER_P_H
#define TARGOMAN_COMMON_NETWORK_PRIVATE_CLSBASETCPSERVER_P_H

#include <QHash>
#include <QObject>
#include <QTcpServer>

#include "intfTCPServerSecurity.hpp"

namespace Targoman{

namespace AAA{
namespace Private{
class clsAuthorization;
}
}

namespace Common{
namespace Network{

class clsBaseTCPServer;
class clsBaseWebSocketServer;

namespace Private{

class clsBaseTCPServerPrivate;

class clsNullTCPServerSecurity: public intfTCPServerSecurity
{
    public:
        inline virtual ~clsNullTCPServerSecurity(){}
        inline bool isConnectionAllowed(const QString& /*_ip*/, qint32 /*_port*/){return true;}
};

class clsTCPServersPool
{
    friend class clsBaseTCPServerPrivate;
    friend class Targoman::AAA::Private::clsAuthorization;

public:
    clsTCPServersPool(){}

private:
    void registerTCPServer(const QString& _name, clsBaseTCPServerPrivate* _instancePointer);
    const QStringList getTCPServersList();
    void setSecurityProvider(const QString& _name, intfTCPServerSecurity* _securityProvider);

    Q_DISABLE_COPY(clsTCPServersPool)

private:
    QHash<QString, clsBaseTCPServerPrivate*> TCPServers;
}extern gTCPServersPool;

class clsBaseTCPServerPrivate: public QObject
{
    Q_OBJECT
    friend class Targoman::Common::Network::clsBaseTCPServer;
    friend class Targoman::Common::Network::clsBaseWebSocketServer;
    friend void Targoman::Common::Network::Private::clsTCPServersPool::setSecurityProvider(const QString &_name,
                                                                           intfTCPServerSecurity *_securityProvider);

public slots:
  void slotRemoveClient();
  void slotRemoveClient(QAbstractSocket::SocketError);

public:
    clsBaseTCPServerPrivate(const QString& _name, clsBaseTCPServer* _pParent);
    ~clsBaseTCPServerPrivate();

protected:
    intfTCPServerSecurity* SecurityProvider;
    QString ActorUUID;

private:
    clsBaseTCPServer* pParent;
};

}
}
}
}
#endif // TARGOMAN_COMMON_NETWORK_PRIVATE_CLSBASETCPSERVER_P_H
