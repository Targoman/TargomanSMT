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

#ifndef TARGOMAN_COMMON_NETWORK_CLSBASECLIENT_H_
#define TARGOMAN_COMMON_NETWORK_CLSBASECLIENT_H_

#include <QBitArray>
#include <QList>
#include <QTimer>
#include <QTcpSocket>

class QHostAddress;
class QTimerEvent;

namespace Targoman{
namespace Common{
namespace Network{
namespace Private{
class clsBaseClientManagerPrivate;
}

class clsBaseClientManager : public QTcpSocket
{
  Q_OBJECT

public:
  clsBaseClientManager(const QString& _id, QObject* _parent = 0, quint32 _connectionTimeout = 3 * 60);
  ~clsBaseClientManager();

  // Rate control
  qint64 writeToSocket(qint64 _bytes);
  qint64 readFromSocket(qint64 _bytes);
  qint64 downloadSpeed() const;
  qint64 uploadSpeed() const;

  inline qint64 socketBytesAvailable() const { return QTcpSocket::bytesAvailable(); }

  bool canTransferMore() const;
  QString idStr();

  int connectionTimeout();
  int pendingRequestTimeout();

  void setConnectionTimeout(int _ms);
  void setPendingRequestTimeout(int _ms);

  qint64 bytesAvailable();
  qint64 read(char* _data, qint64 _maxlen);
  qint64 write(const char* _data, qint64 _len);
  qint64 write(QByteArray _data);
  QByteArray getAllData();
  QByteArray read(qint64 _maxlen);

protected:
  virtual void timerEvent(QTimerEvent* _event);
  void disconnectFromHost();
  virtual void processIncomingData()=0;

signals:
    void sigReadyToTransfer();
    void sigBytesReceived(qint64 size);

protected slots:
    virtual void slotRequestTimeout();

protected:
    // Data waiting to be read/written
    QByteArray IncomingBuffer;
    QByteArray OutgoingBuffer;
private:
  QScopedPointer<Private::clsBaseClientManagerPrivate> pPrivate;
};

}
}
}

#endif // TARGOMAN_COMMON_NETWORK_CLSBASECLIENT_H_
