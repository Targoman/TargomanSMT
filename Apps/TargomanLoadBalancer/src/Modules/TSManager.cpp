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
