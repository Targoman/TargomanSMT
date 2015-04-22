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

#include "clsBaseConnectionManager.h"

#include <QByteArray>
#include <QDateTime>


namespace Targoman{
namespace Common{
namespace Network{

clsBaseConnectionManager::clsBaseConnectionManager(int _maxConnections):
  MaxConnections(_maxConnections),TotalConnections(0)
{
}

const QByteArray clsBaseConnectionManager::clientId() const
{
  if (this->ServerID.isEmpty())
  {
      // Generate peer id
      int StartupTime = int(QDateTime::currentDateTime().toTime_t());

      QString Str;
      Str.sprintf("-QT%04x-", (QT_VERSION % 0xffff00) >> 8);
      this->ServerID += Str.toLatin1();
      this->ServerID += QByteArray::number(StartupTime, 10);
      this->ServerID += QByteArray(20 - this->ServerID.size(), '-');
  }
  this->TotalConnections++;
  return this->ServerID+QByteArray::number(this->TotalConnections);
}

}
}
}
