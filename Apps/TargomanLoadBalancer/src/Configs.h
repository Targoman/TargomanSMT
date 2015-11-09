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

#ifndef TARGOMAN_APPS_TARGOMANLOADBALANACER_CONFIGS_H
#define TARGOMAN_APPS_TARGOMANLOADBALANACER_CONFIGS_H

#include <QtNetwork/QHostAddress>
#include "libTargomanCommon/Configuration/tmplConfigurable.h"
#include "libTargomanCommon/Configuration/tmplConfigurableMultiMap.hpp"
#include "libTargomanCommon/Configuration/Validators.hpp"
#include "libTargomanCommon/Macros.h"
#include "libTargomanCommon/exTargomanBase.h"

namespace Targoman {
namespace Apps {

TARGOMAN_ADD_EXCEPTION_HANDLER(exTargomanLoadBalancer, Common::exTargomanBase);

extern QString ActorUUID;
class gConfigs{
public:
    static inline QString appConfig(const QString& _name){
        return "App/" + _name;
    }
    struct stuServer{
        stuServer(const QString& _basePath);
        Common::Configuration::tmplConfigurable<QString> Host;
        Common::Configuration::tmplConfigurable<quint16> Port;
        Common::Configuration::tmplConfigurable<quint8>  AbsoluteScore;
        Common::Configuration::tmplConfigurable<bool>    Active;
        Common::Configuration::tmplConfigurable<QString> UserName;
        Common::Configuration::tmplConfigurable<QString> Password;
        struct stuStatistics{
            stuStatistics(const QString& _basePath);
            Common::Configuration::tmplConfigurable<qint8> Load1MinPercent;
            Common::Configuration::tmplConfigurable<qint8> Load15MinPercent;
            Common::Configuration::tmplConfigurable<qint8> FreeMemoryPercent;
            Common::Configuration::tmplConfigurable<qint8> TranslationQueuePercent;
        }Statistics;
    };

public:
    static Common::Configuration::tmplConfigurableMultiMap<stuServer> TranslationServers;
    static Common::Configuration::tmplConfigurable<quint16>           MaxConcurrentClients;
    static Common::Configuration::tmplConfigurable<QString>           DBHost;
    static Common::Configuration::tmplConfigurable<quint16>           DBPort;
    static Common::Configuration::tmplConfigurable<QString>           DBUser;
    static Common::Configuration::tmplConfigurable<QString>           DBPass;
    static Common::Configuration::tmplConfigurable<QString>           DBSchema;
};

}
}
#endif // TARGOMAN_APPS_TARGOMANLOADBALANACER_CONFIGS_H
