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

#ifndef TARGOMAN_APPS_MODULES_TSMANAGER_H
#define TARGOMAN_APPS_MODULES_TSMANAGER_H

#include "libTargomanCommon/JSONConversationProtocol.h"
#include "libTargomanCommon/Configuration/intfRPCExporter.hpp"
#include "Configs.h"

namespace Targoman {
namespace Apps{
namespace Modules {

TARGOMAN_ADD_EXCEPTION_HANDLER(exTSManager, exTargomanLoadBalancer);

class TSManager : public Common::Configuration::intfRPCExporter, public Common::Configuration::intfModule
{
public slots:
    void slotSendRequest();
    void slotProcessResponse(const Common::JSONConversationProtocol::stuResponse &_response);
    void slotServerDisconnected();

public slots:
    Common::Configuration::stuRPCOutput rpcTranslate(const QVariantMap& _args);
    Common::Configuration::stuRPCOutput rpcArrayTranslate(const QVariantMap& _args);

private:
    TSManager() : intfModule(this->moduleName()) {}

    TARGOMAN_DEFINE_SINGLETONMODULE("TSManager", TSManager)
private:
    static Common::Configuration::tmplConfigurable<quint16> MaxTranslationTime;
    static Common::Configuration::tmplConfigurable<quint8> MaxRetries;
};

}
}
}
#endif // TARGOMAN_APPS_MODULES_TSMANAGER_H
