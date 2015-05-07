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

TARGOMAN_ADD_EXCEPTION_HANDLER(exTSMonitor, exTargomanLoadBalancer);

class TSMonitorPrivate : public QObject
{
    Q_OBJECT
public:
    class clsServerConnection : public QTcpSocket
    {
    public:
        size_t   ConfigIndex;
        quint16  TotalScore;
        QString LastRequestUUID;
        QTime   LastRequestTime;
        bool    LoggedIn;
        QMutex  Lock;

        clsServerConnection(size_t _configIndex = 0) {
            this->ConfigIndex = _configIndex;
            this->TotalScore = 0;
        }
    };
    TSMonitorPrivate(QObject *_parent = NULL);
public:
    void timerEvent(QTimerEvent *);
public slots:
    void slotSendRequest();
    void slotProcessDataOnReceive();
    void slotUpdateInfo();

public:
    bool                    InfoUpdated;
    gConfigs::stuServer*    BestServer;
    QList<clsServerConnection*>    Servers;
};

class TSMonitor : public QThread, public Common::Configuration::intfModule
{
    Q_OBJECT
public:
    const gConfigs::stuServer &bestServer();


private:
    void run();
    TSMonitor() : intfModule(this->moduleName()) {}

    TARGOMAN_DEFINE_SINGLETONMODULE("TSMonitor", TSMonitor)
private:
    QScopedPointer<TSMonitorPrivate> pPrivate;

    static Common::Configuration::tmplConfigurable<quint16> UpdtaeInterval;
    static Common::Configuration::tmplConfigurable<quint16> WaitOnUpdtae;
    friend class TSMonitorPrivate;
};

}
}
}

#endif // TARGOMAN_APPS_MODULES_TSMONITOR_H
