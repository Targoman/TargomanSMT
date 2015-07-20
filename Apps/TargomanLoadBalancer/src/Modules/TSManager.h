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
