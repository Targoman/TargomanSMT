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

#ifndef TARGOMAN_COMMON_NETWORK_CLSBASECONNECTIONMANAGER_H_
#define TARGOMAN_COMMON_NETWORK_CLSBASECONNECTIONMANAGER_H_

#include <QByteArray>
#include <QSet>

#include "libTargomanCommon/Macros.h"

namespace Targoman{
namespace Common{
namespace Network{

class clsBaseClientManager;

/**
 * This is a base class for all connections mostly used in TCP. It does not handle any physiscal connection but just
 * holds the logic of connection management. The base logic behind this class is to assign a client ID to each client.
 **/
class clsBaseConnectionManager
{
public:
  /**
   * @brief Constructor which just initializes private properties.
   *
   * @param _maxConnections  Maximum conections supported by subclass Defaults to 65000.
   **/
  clsBaseConnectionManager(int _maxConnections = 65000);
  virtual ~clsBaseConnectionManager(){}

  inline void addConnection(clsBaseClientManager* _client){this->Connections << _client;}
  inline void removeConnection(clsBaseClientManager* _client){this->Connections.remove(_client);}
  inline int  maxConnections(){return this->MaxConnections;}
  inline void setMaxConnections(int _maxConnections){this->MaxConnections = _maxConnections;}
  inline qint64 totalConnections(){return this->TotalConnections;}
  inline int currentConnections(){return this->TotalConnections;}

  virtual const QByteArray clientId() const;
  virtual inline bool canAddConnection() const{return (this->Connections.size() < this->MaxConnections);}


protected:
  int MaxConnections;
  QSet<clsBaseClientManager*> Connections;
  mutable quint64 TotalConnections;
  mutable QByteArray ServerID;
};

}
}
}

#endif // TARGOMAN_COMMON_NETWORK_CLSBASECONNECTIONMANAGER_H_
