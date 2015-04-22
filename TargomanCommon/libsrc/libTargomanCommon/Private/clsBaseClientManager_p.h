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

#ifndef TARGOMAN_COMMON_NETWORK_PRIVATE_CLSBASECLIENTMANAGER_P_H
#define TARGOMAN_COMMON_NETWORK_PRIVATE_CLSBASECLIENTMANAGER_P_H

#include <QTimer>

namespace Targoman{
namespace Common{
namespace Network{
namespace Private {

class clsBaseClientManagerPrivate
{
public:
    // Upload/download speed records
    qint64 UploadSpeedData[8];
    qint64 DownloadSpeedData[8];
    int    TransferSpeedTimer;

    // Timeout handling
    int  TimeoutTimer;
    bool InvalidateTimeout;
    bool CloseLater;

    int ConnectTimeout;
    int PendingRequestTimeout;

    QTimer PendingRequestTimer;
    QString IDString;
};

}
}
}
}
#endif // TARGOMAN_COMMON_NETWORK_PRIVATE_CLSBASECLIENTMANAGER_P_H

