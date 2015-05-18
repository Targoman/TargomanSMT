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

#ifndef TARGOMAN_APPS_APPTARGOMANSMTCONSOLE_H
#define TARGOMAN_APPS_APPTARGOMANSMTCONSOLE_H

#include "libTargomanCommon/clsSafeCoreApplication.h"
#include "libTargomanCommon/Types.h"
#include "libTargomanCommon/JSONConversationProtocol.h"
#include "libTargomanCommon/Configuration/intfRPCExporter.hpp"

namespace Targoman {
namespace Apps{

class appTargomanSMTServer : public Common::Configuration::intfRPCExporter
{
    Q_OBJECT
public:
    appTargomanSMTServer() {
        this->exportMyRPCs();
        this->PendingTranslations = 0;
        this->TotalTranslations = 0;
    }

public slots:
    void slotExecute();
    void slotValidateAgent(INOUT QString&        _user,
                          const QString&        _pass,
                          const QString&        _ip,
                          OUTPUT bool&          _canView,
                          OUTPUT bool&          _canChange);

    void slotPong(Targoman::Common::stuPong& _pong);

public slots:
    Common::Configuration::stuRPCOutput rpcGetStatistics(const QVariantMap&);
    Common::Configuration::stuRPCOutput rpcTranslate(const QVariantMap& _args);

private:
    quint64 TotalTranslations;
    quint64 PendingTranslations;
};

}
}

#endif // TARGOMAN_APPS_APPTARGOMANSMTCONSOLE_H
