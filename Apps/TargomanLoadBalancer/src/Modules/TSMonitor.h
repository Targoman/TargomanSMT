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
 @author Behrooz Vedadian <vedadian@gmail.com>
 */

#ifndef TARGOMAN_APPS_MODULES_TSMONITOR_H
#define TARGOMAN_APPS_MODULES_TSMONITOR_H

#include <QThread>
#include "Configs.h"
#include "clsTranslationServer.h"

namespace Targoman {
namespace Apps{
namespace Modules {

TARGOMAN_ADD_EXCEPTION_HANDLER(exTSMonitor, exTargomanLoadBalancer);

class TSMonitorPrivate : public QObject
{
    Q_OBJECT
public:
    TSMonitorPrivate(QObject *_parent = NULL);
public:
    void timerEvent(QTimerEvent *);

public slots:
    void slotSendRequest();
    void slotProcessResponse(Common::JSONConversationProtocol::stuResponse _response);
    void slotUpdateInfo();
    void slotServerDisconnected();

public:
    bool                           InfoUpdated;
    size_t                         ConnectedServers;
    QMutex                         ListLock;
    QList<clsTranslationServer*>    Servers;
};

class TSMonitor : public QThread, public Common::Configuration::intfModule
{
    Q_OBJECT
public:
    quint16 bestServerIndex();
    size_t connectedServers(){return this->pPrivate->ConnectedServers; }

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
