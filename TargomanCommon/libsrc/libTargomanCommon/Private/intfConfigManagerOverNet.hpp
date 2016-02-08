/*******************************************************************************
 * Targoman: A robust Statistical Machine Translation framework                *
 *                                                                             *
 * Copyright 2014-2015 by ITRC <http://itrc.ac.ir>                           *
 *                                                                             *
 * This file is part of Targoman.                                              *
 *                                                                             *
 * Targoman is free software: you can redistribute it and/or modify            *
 * it under the terms of the GNU Lesser General Public License as published by *
 * the Free Software Foundation, either version 3 of the License, or           *
 * (at your option) any later version.                                         *
 *                                                                             *
 * Targoman is distributed in the hope that it will be useful,                 *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of              *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the               *
 * GNU Lesser General Public License for more details.                         *
 * You should have received a copy of the GNU Lesser General Public License    *
 * along with Targoman. If not, see <http://www.gnu.org/licenses/>.            *
 *                                                                             *
 *******************************************************************************/
/**
 @author S. Mohammad M. Ziabary <ziabary@targoman.com>
 */

#ifndef TARGOMAN_COMMON_CONFIGURATION_PRIVATE_INTFCONFIGMANAGEROVERNET_HPP
#define TARGOMAN_COMMON_CONFIGURATION_PRIVATE_INTFCONFIGMANAGEROVERNET_HPP

#include <QTcpServer>
#include <unistd.h>
#include "Configuration/tmplConfigurable.h"

namespace Targoman {
namespace Common {
namespace Configuration {
namespace Private {

TARGOMAN_ADD_EXCEPTION_HANDLER(exConfigOverNet, exConfiguration);

class intfConfigManagerOverNet : public QObject{
    Q_OBJECT
public:
    intfConfigManagerOverNet(QTcpServer* _server):
        Server(_server)
    {}

    void start(bool _justCheck = false){
        bool PrintMessage = true;
        do{
            if (!this->Server->listen(this->AdminLocal.value() ? QHostAddress::LocalHost : QHostAddress::Any,
                              this->Port.value())){
                if (this->WaitPortReady.value() == false)
                    throw exConfigOverNet(QString("Unable to Start Server on: %1:%2").arg(
                                                    this->AdminLocal.value() ? "localhost" : "0.0.0.0").arg(
                                                    this->Port.value()));
                else{
                    if (PrintMessage && _justCheck == false){
                        TargomanLogInfo(5, "Waiting for port "<<this->Port.value()<<" to become ready...");
                        PrintMessage = false;
                    }
                    usleep(500000);
                }
            }else
                break;
        }while(_justCheck == false);

        if (_justCheck)
            this->Server->close();
        else
            TargomanInfo(5, QString("Administration server has been started on %1:%2").arg(
                             this->AdminLocal.value() ? "localhost" : "0.0.0.0").arg(
                             this->Port.value()))
    }

    inline void checkPortAvailable(){  this->start(true); }

protected:
    QScopedPointer<QTcpServer>        Server;

public:
    static tmplRangedConfigurable<unsigned>      Port;
    static tmplConfigurable<bool>     WaitPortReady;
    static tmplConfigurable<bool>     AdminLocal;
    static tmplRangedConfigurable<int>      MaxSessionTime;
    static tmplRangedConfigurable<int>      MaxIdleTime;
    static tmplConfigurable<quint16>  MaxConnections;
};

}
}
}
}
#endif // TARGOMAN_COMMON_CONFIGURATION_PRIVATE_INTFCONFIGMANAGEROVERNET_HPP
