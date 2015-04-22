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

#ifndef TARGOMAN_COMMON_NETWORK_PRIVATE_CLSBASERATECONTROLLER_P_H
#define TARGOMAN_COMMON_NETWORK_PRIVATE_CLSBASERATECONTROLLER_P_H

#include <QSet>
#include <QTime>
#include <QMutex>

namespace Targoman{
namespace Common{
namespace Network{
class clsBaseClientManager;

namespace Private {

class clsBaseRatecontrollerPrivate
{

public:
    QTime             StopWatch;
    QSet<clsBaseClientManager*>  Clients;
    int               UpLimit;
    int               DownLimit;
    bool              TransferScheduled;
    QMutex            mxClients;
};

}
}
}
}
#endif // TARGOMAN_COMMON_NETWORK_PRIVATE_CLSBASERATECONTROLLER_P_H
