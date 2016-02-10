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


tmplRangedConfigurable<quint16> TSManager::MaxTranslationTime(
        MAKE_CONFIG_PATH("MaxTranslationTime"),
        "Maximum time to wait for a translation response in seconds",
        1,5*60,
        120,
        ReturnTrueCrossValidator,
        "","","",
        Common::Configuration::enuConfigSource::File
        );

tmplRangedConfigurable<quint8> TSManager::MaxRetries(
        MAKE_CONFIG_PATH("MaxRetries"),
        "Maximum tries to translate if disconnected from server",
        1,30,
        5,
        ReturnTrueCrossValidator,
        "","","",
        Common::Configuration::enuConfigSource::File
        );

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

            QScopedPointer<clsTranslationServer> BestServer(new clsTranslationServer(
                        Dir,
                        PreferedServerInex,
                        "rpcTranslate",
                        _args));
            TargomanLogInfo(4,"Trying to translate using Server: "<<Dir<<":"<<BestServer->configIndex());
            connect(BestServer.data(), &clsTranslationServer::sigReadyForFirstRequest,
                    BestServer.data(), &clsTranslationServer::slotSendPredefinedRequest,
                    Qt::DirectConnection);
            BestServer->connect();

            quint32 ElapsedCentiSeconds = 0;
            //Wait until response unlocks
            Common::JSONConversationProtocol::stuResponse Response;
            while(BestServer->getSafeResponse(Response) == false){
                usleep(10000);
                QCoreApplication::processEvents();
                ++ElapsedCentiSeconds;
                if (ElapsedCentiSeconds > TSManager::MaxTranslationTime.value() * 100){
                    BestServer->deleteLater();
                    throw exTSManager("Translation TimedOut");
                }
            }

            if (Response.Type == JSONConversationProtocol::stuResponse::Pong &&
                    Response.Result.toString() == SERVER_DISCONNECTED){
                BestServer->blockSignals(true);
                BestServer->deleteLater();
                PreferedServerInex = -1;
                continue;
            }
            BestServer->blockSignals(true);
            BestServer->deleteLater();

            Response.Args.insert("Server",BestServer->configIndex());

            TargomanLogInfo(4,"Returning response from: "<<Dir<<BestServer->configIndex());
            return Response;
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
        qint32 _preferedServer,
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
