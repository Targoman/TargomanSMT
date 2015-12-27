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
#ifdef WITH_QJsonRPC
class TSManagerJsonRPCService : public QJsonRpcService
{
    Q_OBJECT
    Q_CLASSINFO("serviceName", "TSManager")
public:
    static TSManagerJsonRPCService& instance(){
        static TSManagerJsonRPCService* Instance = NULL;
        return *(Q_LIKELY(Instance) ? Instance : (Instance = new TSManagerJsonRPCService));
    }

public slots:
    QVariantList translate(
            quint32 _preferedServer,
            QString _dir,
            QString _text,
            bool _brief,
            bool _keep);
private:
    TSManagerJsonRPCService();
    Q_DISABLE_COPY(TSManagerJsonRPCService)
};
#endif

class TSManager :
        public Common::Configuration::intfRPCExporter,
        public Common::Configuration::intfModule
{
    Q_OBJECT
public:
    Common::JSONConversationProtocol::stuResponse baseTranslation(const QVariantMap &_args);

public slots:
    void slotServerDisconnected();

public slots:
    Common::Configuration::stuRPCOutput rpcTranslate(const QVariantMap& _args);

private:
    TSManager();

    TARGOMAN_DEFINE_SINGLETON_MODULE(TSManager)
private:
    static Common::Configuration::tmplConfigurable<quint16> MaxTranslationTime;
    static Common::Configuration::tmplConfigurable<quint8>  MaxRetries;
};

}
}
}
#endif // TARGOMAN_APPS_MODULES_TSMANAGER_H
