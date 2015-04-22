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

#ifndef TARGOMAN_COMMON_NETWORK_CLSBASERATECONTROLLER_H_
#define TARGOMAN_COMMON_NETWORK_CLSBASERATECONTROLLER_H_

#include <QObject>

namespace Targoman{
namespace Common{
namespace Network{

namespace Private{
class clsBaseRatecontrollerPrivate;
}

class clsBaseClientManager;

class clsBaseRateController : public QObject
{
  Q_OBJECT

public:
  clsBaseRateController(int _upLimit=-1, int _downLimit=-1, QObject* _parent = 0);
  ~clsBaseRateController();

  void addClient(clsBaseClientManager* _client);
  void removeClient(clsBaseClientManager* _client);

  int uploadLimit();
  void setUploadLimit(int _bps);

  int downloadLimit() const;
  void setDownloadLimit(int _bps);

public slots:
  void slotTransfer();
  void slotScheduleTransfer();

private:
  Q_DISABLE_COPY(clsBaseRateController)
  QScopedPointer<Private::clsBaseRatecontrollerPrivate> pPrivate;
};

}
}
}

#endif // TARGOMAN_COMMON_NETWORK_CLSBASERATECONTROLLER_H_

