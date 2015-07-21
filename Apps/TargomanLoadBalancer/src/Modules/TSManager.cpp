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


tmplConfigurable<quint16> TSManager::MaxTranslationTime(
        TSManager::instance().baseConfigPath() + "/MaxTranslationTime",
        "Maximum time to wait for a translation response in seconds",
        30,
        Validators::tmplNumericValidator<quint32,1,5*60>,
        "","","",
        Common::Configuration::enuConfigSource::File
        );

tmplConfigurable<quint8> TSManager::MaxRetries(
        TSManager::instance().baseConfigPath() + "/MaxRetries",
        "Maximum tries to translate if disconnected from server",
        5,
        Validators::tmplNumericValidator<quint16,1,30>,
        "","","",
        Common::Configuration::enuConfigSource::File
        );


void TSManager::slotProcessResponse(const JSONConversationProtocol::stuResponse& _response)
{
    clsTranslationServer* Server = dynamic_cast<clsTranslationServer*>(sender());
    if(Server){
        Server->Response = _response;
        Server->RWLock.unlock();
    }
}

void TSManager::slotServerDisconnected()
{
    clsTranslationServer* Server = dynamic_cast<clsTranslationServer*>(sender());
    if(Server){
        Server->Response = JSONConversationProtocol::stuResponse(
                    JSONConversationProtocol::stuResponse::Pong,
                    "SERVER_DISCONNECTED");
        Server->RWLock.unlock();
    }
}

stuRPCOutput TSManager::rpcTranslate(const QVariantMap &_args)
{
    for(int i=0; i<TSManager::MaxRetries.value(); ++i){
        try{
            clsTranslationServer* BestServer = new clsTranslationServer(TSMonitor::instance().bestServerIndex());
            BestServer->connect();
            connect(BestServer,SIGNAL(sigResponse(Common::JSONConversationProtocol::stuResponse)),
                    this, SLOT(slotProcessResponse(Common::JSONConversationProtocol::stuResponse)),
                    Qt::DirectConnection);
            connect(BestServer,SIGNAL(sigDisconnected()), this, SLOT(slotServerDisconnected()));
            connect(BestServer, SIGNAL(sigNextRequest()), this, SLOT(slotSendRequest()),Qt::DirectConnection);

            BestServer->RWLock.lockForWrite();

            BestServer->sendRequest("rpcTranslate", _args);
            quint32 ElapsedCentiSeconds = 0;
            //Wait until response unlocks
            while(!BestServer->RWLock.tryLockForRead()){
                usleep(10000);
                ++ElapsedCentiSeconds;
                if (ElapsedCentiSeconds > TSManager::MaxTranslationTime.value() * 100){
                    BestServer->deleteLater();
                    QVariantMap Error;
                    Error.insert("msg","Translation Timed Out");
                    return stuRPCOutput(-1, Error);
                }
            }

            if (BestServer->Response.Type == JSONConversationProtocol::stuResponse::Pong &&
                    BestServer->Response.Result.toString() == "SERVER_DISCONNECTED"){
                BestServer->deleteLater();
                continue;
            }

            return stuRPCOutput(BestServer->Response.Result, BestServer->Response.Args);
        }catch(exTSMonitor &e){
            throw exTSManager("No resources available");
        }
    }
    throw exTSManager("Unable to translate because max tries failed");
}




}
}
}
