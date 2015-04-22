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

#ifndef TARGOMAN_COMMON_NETWORK_CLSBASESERVER_H_
#define TARGOMAN_COMMON_NETWORK_CLSBASESERVER_H_

#include <QList>
#include <QTcpServer>

#include "libTargomanCommon/Network/clsBaseClientManager.h"
#include "libTargomanCommon/Network/clsBaseRateController.h"
#include "libTargomanCommon/Network/clsBaseConnectionManager.h"
#include "libTargomanCommon/exTargomanBase.h"

namespace Targoman{
namespace Common{
namespace Network{
namespace Private{
class clsBaseTCPServerPrivate;
}

TARGOMAN_ADD_EXCEPTION_HANDLER(exTargomanNetwork, exTargomanBase);
TARGOMAN_ADD_EXCEPTION_HANDLER(exInvalidServerName, exTargomanNetwork);

class clsBaseTCPServer : public QTcpServer
{
    Q_OBJECT

public:
    /**
    * @brief clsBaseTCPServer
    * @param _fullName Indicates purpose of this server it must consist of at least two parts first part is domain and
    * last part will be server purpose (ex. TargomanSMT/Configuration)
    * @param _conMngr
    * @param _rateCtrl
    */
  clsBaseTCPServer(const QString& _fullName,
                   clsBaseConnectionManager* _conMngr = new clsBaseConnectionManager,
                   clsBaseRateController* _rateCtrl = new clsBaseRateController);
  ~clsBaseTCPServer();

protected:
  virtual void incomingConnection(int _socketDescriptor);
  virtual clsBaseClientManager* newClientInstance(const QString& _ip, qint32 _port)=0;
  inline void setMaxConnections(int _count){this->ConnectionManager->setMaxConnections(_count);}
  inline void setUploadLimit(int _limit){this->RateController->setUploadLimit(_limit);}
  inline void setDownloadLimit(int _limit){this->RateController->setDownloadLimit(_limit);}
  inline int uploadLimit(){return this->RateController->uploadLimit();}
  inline int downloadLimit(){return this->RateController->downloadLimit();}
  inline int maxConnections(){return this->ConnectionManager->maxConnections();}
  inline int currentConnections(){return this->ConnectionManager->currentConnections();}
  inline qint64 totalConnections(){return this->ConnectionManager->totalConnections();}

signals:
  void sigNewClient(const QHostAddress& _host, quint16 _port, const QString& _id);
  void sigClientRemoved(const QHostAddress& _host, quint16 _port, const QString& _id);

protected:
    clsBaseConnectionManager* ConnectionManager;
    clsBaseRateController* RateController;

private:
    QScopedPointer<Private::clsBaseTCPServerPrivate> pPrivate;
    friend class Private::clsBaseTCPServerPrivate;
};

}
}
}

#endif // TARGOMAN_COMMON_NETWORK_CLSBASESERVER_H_
