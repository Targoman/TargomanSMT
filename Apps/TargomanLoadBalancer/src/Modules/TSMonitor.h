/******************************************************************************
 * Targoman: A robust Statistical Machine Translation framework               *
 *                                                                            *
 * Copyright 2014-2015 by ITRC <http://itrc.ac.ir>                            *
 *                                                                            *
 * This file is part of Targoman.                                             *
 *                                                                            *
 * Targoman is free software: you can redistribute it and/or modify           *
 * it under the terms of the GNU Lesser General Public License as published   *
 * by the Free Software Foundation, either version 3 of the License, or       *
 * (at your option) any later version.                                        *
 *                                                                            *
 * Targoman is distributed in the hope that it will be useful,                *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of             *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              *
 * GNU Lesser General Public License for more details.                        *
 * You should have received a copy of the GNU Lesser General Public License   *
 * along with Targoman. If not, see <http://www.gnu.org/licenses/>.           *
 *                                                                            *
 ******************************************************************************/
/**
 * @author S. Mohammad M. Ziabary <ziabary@targoman.com>
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
    QMultiMap<QString, clsTranslationServer*>    Servers;
};

class TSMonitor : public QThread, public Common::Configuration::intfModule
{
    Q_OBJECT
public:
    quint16 bestServerIndex(const QString& _dir);
    size_t connectedServers(){return this->pPrivate->ConnectedServers; }
    void wait4AtLeastOneServerAvailable();

private:
    void run();
    TSMonitor() {}

    TARGOMAN_DEFINE_SINGLETON_MODULE(TSMonitor)
private:
    QScopedPointer<TSMonitorPrivate> pPrivate;

    static Common::Configuration::tmplConfigurable<quint16> UpdateInterval;
    static Common::Configuration::tmplConfigurable<quint16> WaitOnUpdtae;
    friend class TSMonitorPrivate;
};

}
}
}

#endif // TARGOMAN_APPS_MODULES_TSMONITOR_H
