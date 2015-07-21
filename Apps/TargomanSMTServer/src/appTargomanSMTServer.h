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
