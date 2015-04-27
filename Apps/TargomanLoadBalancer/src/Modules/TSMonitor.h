/*************************************************************************
 * Copyright © 2012-2015, Targoman.com
 *
 * Published under the terms of TCRL(Targoman Community Research License)
 * You can find a copy of the license file with distributed source or
 * download it from http://targoman.com/License.txt
 *
 *************************************************************************/
/**
 @author S. Mohammad M. Ziabary <smm@ziabary.com>
 */

#ifndef TARGOMAN_APPS_MODULES_TSMONITOR_H
#define TARGOMAN_APPS_MODULES_TSMONITOR_H

#include <QThread>
#include <QtNetwork/QTcpSocket>
#include "Configs.h"

namespace Targoman {
namespace Apps{
namespace Modules {

class TSMonitor : Common::Configuration::intfModule, public QThread
{
    Q_OBJECT
public:
    void run();

public slots:
    void updateInfo();

private:
    void timerEvent(QTimerEvent *ev);

private:
    TARGOMAN_DEFINE_SINGLETONMODULE("TSMonitor", TSMonitor)

private:
    QMutex  Lock;
    QList<QTcpSocket*> ServersSocket;

    static Common::Configuration::tmplConfigurable<quint16> UpdtaeInterval;
};

}

#endif // TARGOMAN_APPS_MODULES_TSMONITOR_H
