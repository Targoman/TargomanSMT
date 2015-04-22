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

#include <QtCore>
#include <QMutexLocker>

#include "clsBaseRateController.h"
#include "clsBaseClientManager.h"
#include "Private/clsBaseRateController_p.h"

const quint32 MAX_TRANSFARE_BUFFER = -1;

namespace Targoman{
namespace Common{
namespace Network{

clsBaseRateController::clsBaseRateController(int _upLimit, int _downLimit, QObject* _parent):
    QObject(_parent), pPrivate(new Private::clsBaseRatecontrollerPrivate)
{
  this->pPrivate->UpLimit = _upLimit;
  this->pPrivate->DownLimit = _downLimit;
  this->pPrivate->TransferScheduled = false;
}

clsBaseRateController::~clsBaseRateController()
{}

void clsBaseRateController::addClient(clsBaseClientManager* _client)
{
  connect(_client, SIGNAL(sigReadyToTransfer()), this, SLOT(slotScheduleTransfer()));
  _client->setReadBufferSize(this->pPrivate->DownLimit < 0? 0 : this->pPrivate->DownLimit * 4);
  QMutexLocker Locker(&this->pPrivate->mxClients);
  this->pPrivate->Clients << _client;
  this->slotScheduleTransfer();
}

void clsBaseRateController::removeClient(clsBaseClientManager* _client)
{
  disconnect(_client, SIGNAL(sigReadyToTransfer()), this, SLOT(slotScheduleTransfer()));
  QMutexLocker Locker(&this->pPrivate->mxClients);
  this->pPrivate->Clients.remove(_client);
}

void clsBaseRateController::setDownloadLimit(int _bps)
{
  this->pPrivate->DownLimit = _bps;
  QMutexLocker Locker(&this->pPrivate->mxClients);
  foreach (clsBaseClientManager* Client, this->pPrivate->Clients)
     Client->setReadBufferSize(this->pPrivate->DownLimit < 0? 0 : this->pPrivate->DownLimit * 4);
}

void clsBaseRateController::slotScheduleTransfer()
{
  if (this->pPrivate->TransferScheduled)
     return;
  this->pPrivate->TransferScheduled = true;
  QTimer::singleShot(50, this, SLOT(slotTransfer()));
}

void clsBaseRateController::slotTransfer()
{

  this->pPrivate->TransferScheduled = false;
  if (this->pPrivate->Clients.isEmpty())
     return;

  int mSecs = 1000;
  if (!this->pPrivate->StopWatch.isNull())
     mSecs = qMin(mSecs, this->pPrivate->StopWatch.elapsed());

  qint64 AllowedBytesToWrite = this->pPrivate->UpLimit < 0 ?
              MAX_TRANSFARE_BUFFER : (this->pPrivate->UpLimit  * mSecs) / 1000;
  qint64 AllowedBytesToRead = this->pPrivate->DownLimit < 0 ?
              MAX_TRANSFARE_BUFFER :(this->pPrivate->DownLimit * mSecs) / 1000;

  if (AllowedBytesToWrite == 0 && AllowedBytesToRead == 0) {
     this->slotScheduleTransfer();
     return;
  }

  QSet<clsBaseClientManager *> PendingClients;
  ///iteration on clients scope
  {
    QMutexLocker Locker(&this->pPrivate->mxClients);
    foreach (clsBaseClientManager* Client, this->pPrivate->Clients)
       if (Client->canTransferMore())
           PendingClients << Client;
  }

  if (PendingClients.isEmpty())
     return;

  this->pPrivate->StopWatch.start();

  bool CanTransferMore;
  do {
     CanTransferMore = false;
     qint64 WriteChunk = qMax<qint64>(1, AllowedBytesToWrite / PendingClients.size());
     qint64 ReadChunk  = qMax<qint64>(1, AllowedBytesToRead  / PendingClients.size());

     QSetIterator<clsBaseClientManager*> ClientIter(PendingClients);
     while (ClientIter.hasNext() && (AllowedBytesToWrite > 0 || AllowedBytesToRead > 0)) {
       clsBaseClientManager* Client = ClientIter.next();
       if (Client->state() != QAbstractSocket::ConnectedState) {
         PendingClients.remove(Client);
         continue;
       }

       bool DataTransferred = false;

       //-- READ -----------------------
       qint64 Available = qMin<qint64>(Client->socketBytesAvailable(), ReadChunk);
       if (Available > 0)
       {
         qint64 ReadBytes = Client->readFromSocket(qMin<qint64>(Available, AllowedBytesToRead));
         if (ReadBytes > 0) {
             AllowedBytesToRead -= ReadBytes;
             DataTransferred = true;
         }
       }

       //-- WRITE -----------------------
       if (this->pPrivate->UpLimit < 0 || this->pPrivate->UpLimit * 2 > Client->bytesToWrite())
       {
         qint64 ChunkSize = qMin<qint64>(WriteChunk, AllowedBytesToWrite);
         qint64 ToWrite =
             qMin(this->pPrivate->UpLimit < 0 ?
                      MAX_TRANSFARE_BUFFER : (this->pPrivate->UpLimit * 2 - Client->bytesToWrite()),
                  ChunkSize);
         if (ToWrite > 0) {
           qint64 WrittenBytes = Client->writeToSocket(ToWrite);
           if (WrittenBytes > 0) {
               AllowedBytesToWrite -= WrittenBytes;
               DataTransferred = true;
           }
         }
       }

       //--------------------------------
       if (DataTransferred && Client->canTransferMore())
         CanTransferMore = true;
       else
         PendingClients.remove(Client);
   }
  } while (CanTransferMore && (AllowedBytesToWrite > 0 || AllowedBytesToRead > 0) && !PendingClients.isEmpty());

  if (CanTransferMore || AllowedBytesToWrite == 0 || AllowedBytesToRead == 0)
     this->slotScheduleTransfer();
}

int clsBaseRateController::uploadLimit() {
    return this->pPrivate->UpLimit;
}
void clsBaseRateController::setUploadLimit(int _bps) {
    this->pPrivate->UpLimit = _bps;
}

int clsBaseRateController::downloadLimit() const {
   return this->pPrivate->DownLimit;
}

}
}
}
