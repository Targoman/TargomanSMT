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

#include <sys/socket.h>
#include <arpa/inet.h>
#include <QStringList>
#include <unistd.h>

#include "clsBaseTCPServer.h"
#include "libTargomanCommon/Logger.h"
#include "Private/clsBaseTCPServer_p.h"

namespace Targoman{
namespace Common{
namespace Network{

Private::clsTCPServersPool Private::gTCPServersPool;

void Private::clsTCPServersPool::registerTCPServer(const QString& _name, clsBaseTCPServerPrivate* _instancePointer)
{
    if (this->TCPServers.contains(_name))
        throw Targoman::Common::Network::exInvalidServerName(QString("Invalid Server name %1").arg(_name));

    this->TCPServers.insert(_name, _instancePointer);
}

const QStringList Private::clsTCPServersPool::getTCPServersList()
{
    return this->TCPServers.keys();
}

void Private::clsTCPServersPool::setSecurityProvider(const QString& _name, intfTCPServerSecurity* _securityProvider)
{
    clsBaseTCPServerPrivate* TCPServer = this->TCPServers.value(_name, NULL);
    if (TCPServer == NULL)
        throw Targoman::Common::Network::exInvalidServerName(QString("No TCP Server named %1 found").arg(_name));

    if(dynamic_cast<intfTCPServerSecurity*>(_securityProvider) == NULL)
        throw exTargomanNetwork("Invalid Security Provider pointer");

    if (dynamic_cast<clsNullTCPServerSecurity*>(TCPServer->SecurityProvider) != NULL)
    {
        delete static_cast<clsNullTCPServerSecurity*>(TCPServer->SecurityProvider);
        TCPServer->SecurityProvider= NULL;
    }

    //Just clsNullTCPServerSecurity Provider can be changed runtime
    if (TCPServer->SecurityProvider)
        throw exTargomanNetwork("Runtime change of Network security provider is not allowed");

    TCPServer->SecurityProvider = _securityProvider;
}

Private::clsBaseTCPServerPrivate::clsBaseTCPServerPrivate(const QString& _name, clsBaseTCPServer *_pParent)
{
    this->SecurityProvider = new clsNullTCPServerSecurity;
    gTCPServersPool.registerTCPServer(_name, this);
    this->pParent = _pParent;
    TARGOMAN_REGISTER_ACTOR(_name);
}

Private::clsBaseTCPServerPrivate::~clsBaseTCPServerPrivate()
{
    if(this->SecurityProvider)
        delete this->SecurityProvider;
}

void Private::clsBaseTCPServerPrivate::slotRemoveClient()
{
    clsBaseClientManager* Client = qobject_cast<clsBaseClientManager *>(sender());
    if (Client) {
        this->pParent->RateController->removeClient(Client);
        this->pParent->ConnectionManager->removeConnection(Client);
        emit this->pParent->sigClientRemoved(Client->peerAddress(),Client->peerPort(), Client->idStr());
        Client->deleteLater();
    }
}

void Private::clsBaseTCPServerPrivate::slotRemoveClient(QAbstractSocket::SocketError)
{
    clsBaseClientManager* Client = qobject_cast<clsBaseClientManager *>(sender());
    Client->close();
}

/******************************************************************************************/
clsBaseTCPServer::clsBaseTCPServer(const QString &_name, clsBaseConnectionManager* _conMngr,
                                   clsBaseRateController* _rateCtrl):
    pPrivate(new Private::clsBaseTCPServerPrivate(_name, this))
{
  Q_ASSERT(_conMngr);
  Q_ASSERT(_rateCtrl);

  this->ConnectionManager = _conMngr;
  this->RateController = _rateCtrl;
}

clsBaseTCPServer::~clsBaseTCPServer()
{}

void clsBaseTCPServer::incomingConnection(int _socketDescriptor)
{
  socklen_t Len;
  struct sockaddr_storage Addr;
  char IPString[INET6_ADDRSTRLEN];
  int Port;

  Len = sizeof Addr;
  getpeername(_socketDescriptor, (struct sockaddr*)&Addr, &Len);


  if (Addr.ss_family == AF_INET) {
      struct sockaddr_in *SocketAddr = (struct sockaddr_in *)&Addr;
      Port = ntohs(SocketAddr->sin_port);
      inet_ntop(AF_INET, &SocketAddr->sin_addr, IPString, sizeof IPString);
  } else { // AF_INET6
      struct sockaddr_in6 *SocketAddr = (struct sockaddr_in6 *)&Addr;
      Port = ntohs(SocketAddr->sin6_port);
      inet_ntop(AF_INET6, &SocketAddr->sin6_addr, IPString, sizeof IPString);
  }

  TargomanDebug(7, "New Connection request from: %s : %d", IPString, Port);

  if (!this->pPrivate->SecurityProvider->isConnectionAllowed(IPString, Port)) {
      TargomanDebug(5, "Client connection refused by SecurityProvider");
      ::close(_socketDescriptor);
      return;
  }

  clsBaseClientManager* Client = this->newClientInstance(IPString, Port);
  if (!Client) {
      TargomanDebug(5, "Client connection refused by subclasses");
      return;
  }

  if (Client->setSocketDescriptor(_socketDescriptor)) {
    if (this->ConnectionManager->canAddConnection()) {
      connect(Client, SIGNAL(error(QAbstractSocket::SocketError)),
             this->pPrivate.data(), SLOT(slotRemoveClient(QAbstractSocket::SocketError)));
      connect(Client, SIGNAL(disconnected()),
             this->pPrivate.data(), SLOT(slotRemoveClient()));

//      this->addPendingConnection(Client); // This has been removed to disable nextPendingconnection function
      this->RateController->addClient(Client);
      this->ConnectionManager->addConnection(Client);
      emit sigNewClient(Client->peerAddress(),Client->peerPort(), Client->idStr());
      return;
    }
  }
  Client->abort();
  delete Client;
}

/*****************************************************************************************************************/
}
}
}
