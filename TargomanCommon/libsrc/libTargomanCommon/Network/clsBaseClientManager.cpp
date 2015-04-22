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

#include <QHostAddress>
#include <QTimerEvent>
#include "clsBaseClientManager.h"
#include "Private/clsBaseClientManager_p.h"

namespace Targoman{
namespace Common{
namespace Network{

static const int  RateControlTimerDelay = 2000;

// Reads a 32bit unsigned int from data in network order.
static inline quint32 fromNetworkData(const char *data)
{
  const unsigned char *udata = (const unsigned char *)data;
  return (quint32(udata[0]) << 24)
      | (quint32(udata[1]) << 16)
      | (quint32(udata[2]) << 8)
      | (quint32(udata[3]));
}

// Writes a 32bit unsigned int from num to data in network order.
static inline void toNetworkData(quint32 num, char *data)
{
  unsigned char *udata = (unsigned char *)data;
  udata[3] = (num & 0xff);
  udata[2] = (num & 0xff00) >> 8;
  udata[1] = (num & 0xff0000) >> 16;
  udata[0] = (num & 0xff000000) >> 24;
}

clsBaseClientManager::~clsBaseClientManager()
{}

// Constructs an unconnected PeerWire client and starts the connect timer.
clsBaseClientManager::clsBaseClientManager(const QString& _id, QObject* _parent,  quint32 _connectionTimeout)
    : QTcpSocket(_parent), pPrivate(new Private::clsBaseClientManagerPrivate)
{
  memset(this->pPrivate->UploadSpeedData,   0, sizeof(this->pPrivate->UploadSpeedData));
  memset(this->pPrivate->DownloadSpeedData, 0, sizeof(this->pPrivate->DownloadSpeedData));

  this->pPrivate->ConnectTimeout = _connectionTimeout * 1000;
  this->pPrivate->PendingRequestTimeout = 100000 * 1000;

  this->pPrivate->TransferSpeedTimer = this->startTimer(RateControlTimerDelay);
  this->pPrivate->TimeoutTimer = this->startTimer(this->pPrivate->ConnectTimeout);
  this->pPrivate->IDString = _id;
  this->pPrivate->CloseLater = false;
  this->pPrivate->InvalidateTimeout = true;

  connect(this, SIGNAL(readyRead()), this, SIGNAL(sigReadyToTransfer()));
  connect(this, SIGNAL(connected()), this, SIGNAL(sigReadyToTransfer()));
  connect(&this->pPrivate->PendingRequestTimer, SIGNAL(timeout()), this, SLOT(slotRequestTimeout()));
}

// Attempts to write 'bytes' bytes to the socket from the buffer.
// This is used by RateController, which precisely controls how much
// each client can write.
qint64 clsBaseClientManager::writeToSocket(qint64 _bytes)
{
  qint64 TotalWritten = 0;
  do {
      qint64 Written = this->write(this->OutgoingBuffer.constData(),
                                   qMin<qint64>(_bytes - TotalWritten, this->OutgoingBuffer.size()));
      if (Written <= 0)
                return TotalWritten ? TotalWritten : Written;

      TotalWritten += Written;
      this->pPrivate->UploadSpeedData[0] += Written;
      this->OutgoingBuffer.remove(0, Written);
  } while (TotalWritten < _bytes && !this->OutgoingBuffer.isEmpty());

  return TotalWritten;
}


// Attempts to read at most 'bytes' bytes from the socket.
qint64 clsBaseClientManager::readFromSocket(qint64 _bytes)
{
  char Buffer[1500];
  qint64 TotalRead = 0;
  do {
      qint64 BytesRead = this->read(Buffer, qMin<qint64>(sizeof(Buffer), _bytes - TotalRead));
      if (BytesRead <= 0)
          break;
      qint64 OldSize = this->IncomingBuffer.size();
      this->IncomingBuffer.resize(OldSize + BytesRead);
      memcpy(this->IncomingBuffer.data() + OldSize, Buffer, BytesRead);

      TotalRead += BytesRead;
  } while (TotalRead < _bytes);

  if (TotalRead > 0){
      this->pPrivate->DownloadSpeedData[0] += TotalRead;
      this->pPrivate->InvalidateTimeout=true;
      this->pPrivate->PendingRequestTimer.start(this->pPrivate->PendingRequestTimeout);
      emit sigBytesReceived(TotalRead);
      this->processIncomingData();
      this->pPrivate->PendingRequestTimer.stop();
  }

  return TotalRead;
}

// Returns the average number of bytes per second this client is
// downloading.
qint64 clsBaseClientManager::downloadSpeed() const
{
  qint64 Sum = 0;
  for (unsigned int i = 0; i < sizeof(this->pPrivate->DownloadSpeedData) / sizeof(qint64); ++i)
      Sum += this->pPrivate->DownloadSpeedData[i];
  return Sum / (8 * 2);
}

// Returns the average number of bytes per second this client is
// uploading.
qint64 clsBaseClientManager::uploadSpeed() const
{
  qint64 Sum = 0;
  for (unsigned int i = 0; i < sizeof(this->pPrivate->UploadSpeedData) / sizeof(qint64); ++i)
      Sum += this->pPrivate->UploadSpeedData[i];
  return Sum / (8 * 2);
}

void clsBaseClientManager::timerEvent(QTimerEvent* _event)
{
  if (_event->timerId() == this->pPrivate->TransferSpeedTimer) {
    // Rotate the upload / download records.
    for (int i = 6; i >= 0; --i) {
        this->pPrivate->UploadSpeedData[i + 1] = this->pPrivate->UploadSpeedData[i];
        this->pPrivate->DownloadSpeedData[i + 1] = this->pPrivate->DownloadSpeedData[i];
    }
    this->pPrivate->UploadSpeedData[0] = 0;
    this->pPrivate->DownloadSpeedData[0] = 0;
  } else if (_event->timerId() == this->pPrivate->TimeoutTimer) {
    // Disconnect if we timed out; otherwise the timeout is
    // restarted.
    if (this->pPrivate->InvalidateTimeout)
        this->pPrivate->InvalidateTimeout = false;
    else
        this->abort();
  }
  if (this->pPrivate->CloseLater && this->OutgoingBuffer.isEmpty())
    QTcpSocket::disconnectFromHost();

  QObject::timerEvent(_event);
}

qint64 clsBaseClientManager::read(char* _data, qint64 _size)
{
  int Bytes = qMin<int>(_size, this->IncomingBuffer.size());
  memcpy(_data, this->IncomingBuffer.constData(), Bytes);
  this->IncomingBuffer.remove(0, Bytes);
  return Bytes;
}

qint64 clsBaseClientManager::write(const char* _data, qint64 _size)
{
  int OldSize = this->OutgoingBuffer.size();
  this->OutgoingBuffer.resize(OldSize + _size);
  memcpy(this->OutgoingBuffer.data() + OldSize, _data, _size);
  emit sigReadyToTransfer();
  return _size;
}

QByteArray clsBaseClientManager::read(qint64 _size)
{
  if (_size <=0 )
    return QByteArray();

  QByteArray Data(_size,0);
  this->read(Data.data(),_size);
  return Data;
}

QByteArray clsBaseClientManager::getAllData()
{
  if (this->IncomingBuffer.size() <= 0)
    return QByteArray();

  QByteArray Data(this->IncomingBuffer.size(),0);
  this->read(Data.data(),this->IncomingBuffer.size());
  return Data;
}

qint64 clsBaseClientManager::write(QByteArray _data)
{
  return this->write(_data.constData(),_data.size());
}

bool clsBaseClientManager::canTransferMore() const
{
  return this->socketBytesAvailable() > 0 ||
      !this->OutgoingBuffer.isEmpty();
}

void clsBaseClientManager::disconnectFromHost()
{
  this->pPrivate->CloseLater = true;
  this->disconnect(SIGNAL(sigBytesReceived(qint64)));
}

void clsBaseClientManager::slotRequestTimeout()
{
  this->abort();
}

QString clsBaseClientManager::idStr(){
    return this->pPrivate->IDString;
}

int clsBaseClientManager::connectionTimeout(){
    return this->pPrivate->ConnectTimeout;
}
int clsBaseClientManager::pendingRequestTimeout(){
    return this->pPrivate->PendingRequestTimeout;
}

void clsBaseClientManager::setConnectionTimeout(int _ms){
    this->pPrivate->ConnectTimeout = _ms;
}
void clsBaseClientManager::setPendingRequestTimeout(int _ms){
    this->pPrivate->PendingRequestTimeout = _ms;
}

qint64 clsBaseClientManager::bytesAvailable() {
    return this->IncomingBuffer.size();
}

}
}
}
