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

#include <unistd.h>
#include "TSManager.h"
#include "libTargomanCommon/JSONConversationProtocol.h"
#include "libTargomanCommon/SimpleAuthentication.h"
#include "TSMonitor.h"

namespace Targoman {
namespace Apps{
namespace Modules {

using namespace Common;
using namespace Common::Configuration;
const char*  SERVER_DISCONNECTED="SERVER_DISCONNECTED";


tmplConfigurable<quint16> TSManager::MaxTranslationTime(
        MAKE_CONFIG_PATH("MaxTranslationTime"),
        "Maximum time to wait for a translation response in seconds",
        120,
        Validators::tmplNumericValidator<quint32,1,5*60>,
        "","","",
        Common::Configuration::enuConfigSource::File
        );

tmplConfigurable<quint8> TSManager::MaxRetries(
        MAKE_CONFIG_PATH("MaxRetries"),
        "Maximum tries to translate if disconnected from server",
        5,
        Validators::tmplNumericValidator<quint16,1,30>,
        "","","",
        Common::Configuration::enuConfigSource::File
        );

void TSManager::slotServerDisconnected()
{
    clsTranslationServer* Server = dynamic_cast<clsTranslationServer*>(QObject::sender());
    if(Server){
        Server->Response = JSONConversationProtocol::stuResponse(
                    JSONConversationProtocol::stuResponse::Pong,
                    SERVER_DISCONNECTED);
        Server->RWLock.unlock();
    }
}

stuRPCOutput TSManager::rpcTranslate(const QVariantMap &_args){
    Common::JSONConversationProtocol::stuResponse Response = this->baseTranslation(_args);
    return stuRPCOutput(Response.Result, Response.Args);
}

Common::JSONConversationProtocol::stuResponse TSManager::baseTranslation(const QVariantMap& _args)
{
    QString Dir = _args.value("dir").toString();
    qint32  PreferedServerInex = _args.value("pref", -1).toInt();

    if (gConfigs::TranslationServers.contains(Dir) == false)
        throw exTSManager("No translation server found for direction: " + Dir);

    for(int i=0; i<TSManager::MaxRetries.value(); ++i){
        try{
            if (PreferedServerInex < 0 ||
                PreferedServerInex >= (qint32)gConfigs::TranslationServers.values(Dir).size() ||
                gConfigs::TranslationServers.values(Dir).at(
                        PreferedServerInex).constData().Active.value() == false)
                PreferedServerInex = Modules::TSMonitor::instance().bestServerIndex(Dir);

            clsTranslationServer* BestServer = new clsTranslationServer(
                        Dir,
                        PreferedServerInex,
                        "rpcTranslate",
                        _args);
            connect(BestServer, &clsTranslationServer::sigDisconnected,
                    this, &TSManager::slotServerDisconnected);
            connect(BestServer, &clsTranslationServer::sigReadyForFirstRequest,
                    BestServer, &clsTranslationServer::slotSendPredefinedRequest,
                    Qt::DirectConnection);
            BestServer->connect();

            quint32 ElapsedCentiSeconds = 0;
            //Wait until response unlocks
            while(BestServer->isResponseReady() == false){
                usleep(10000);
                QCoreApplication::processEvents();
                ++ElapsedCentiSeconds;
                if (ElapsedCentiSeconds > TSManager::MaxTranslationTime.value() * 100){
                    BestServer->deleteLater();
                    throw exTSManager("Translation TimedOut");
                }
            }

            if (BestServer->Response.Type == JSONConversationProtocol::stuResponse::Pong &&
                    BestServer->Response.Result.toString() == SERVER_DISCONNECTED){
                BestServer->blockSignals(true);
                BestServer->deleteLater();
                PreferedServerInex = -1;
                continue;
            }
            BestServer->blockSignals(true);
            BestServer->deleteLater();

            return BestServer->Response;
        }catch(exTSMonitor &e){
            throw exTSManager("No resources available");
        }
    }
    throw exTSManager("Unable to translate because max tries failed");
}

TSManager::TSManager()
{
#ifdef WITH_QJsonRPC
    ConfigManager::instance().registerJsonRPCModule(TSManagerJsonRPCService::instance());
#endif
    this->exportMyRPCs();
}

/************************************************************/
#ifdef WITH_QJsonRPC
TSManagerJsonRPCService::TSManagerJsonRPCService()
{}

QVariantList TSManagerJsonRPCService::translate(
        quint32 _preferedServer,
        QString _dir,
        QString _text,
        bool _brief,
        bool _keep)
{
    QVariantMap Args;
    Args.insert("txt", _text);
    Args.insert("brief", _brief);
    Args.insert("keep", _keep);
    Args.insert("pref", _preferedServer);
    Args.insert("dir", _dir);

    Common::JSONConversationProtocol::stuResponse Response =
            TSManager::instance().baseTranslation(Args);
    return QVariantList()<<Response.Result<<Response.Args;
}
#endif
}
}
}
