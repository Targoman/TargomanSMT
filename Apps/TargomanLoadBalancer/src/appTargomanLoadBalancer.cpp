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
#include <QFile>
#include <QTextStream>
#include "appTargomanLoadBalancer.h"
#include "libTargomanCommon/Configuration/ConfigManager.h"
#include "libTargomanCommon/CmdIO.h"
#include "libTargomanCommon/SimpleAuthentication.h"
#include "Modules/TSMonitor.h"

namespace Targoman {
namespace Apps {

using namespace Common;
using namespace Modules;
appTargomanLoadBalancer::appTargomanLoadBalancer()
{
}

void appTargomanLoadBalancer::slotExecute()
{
    try{
        connect(&Configuration::ConfigManager::instance(),
                &Configuration::ConfigManager::sigPing,
                this,
                &appTargomanLoadBalancer::slotPong,
                Qt::DirectConnection);
        connect(&Configuration::ConfigManager::instance(),
                &Configuration::ConfigManager::sigValidateAgent,
                this,
                &appTargomanLoadBalancer::slotValidateAgent,
                Qt::DirectConnection);

        Modules::TSMonitor::instance().start();
        sleep(1);
        Modules::TSMonitor::instance().wait4AtLeastOneServerAvailable();
        Configuration::ConfigManager::instance().startAdminServer();
    }catch(exTargomanBase& e){
        TargomanError(e.what());
    }catch (std::exception &e){
        TargomanError(e.what());
    }catch(...){
        TargomanError("FATAL Unrecognized exception");
    }
}

/*Configuration::stuRPCOutput appTargomanLoadBalancer::rpcTTS(const QVariantMap &_args)
{
    Q_UNUSED(_args)
    throw exTargomanMustBeImplemented("rpcTTS is not implemented yet!");
}*/

void appTargomanLoadBalancer::slotValidateAgent(QString &_user, const QString &_pass, const QString &_ip, bool &_canView, bool &_canChange)
{
    try{
        SimpleAuthentication::stuLoginInfo LoginInfo =
                SimpleAuthentication::checkLogin(_user, _pass, _ip);
        _canChange = LoginInfo.CanChange;
        _canView = LoginInfo.CanView;
    }catch(exSimpleAuthentication &e){
        TargomanLogError(e.what());
        _canView = false;
        _canChange = false;
        throw;
    }
}

void appTargomanLoadBalancer::slotPong(QString _ssid, stuPong &_pong)
{
    Q_UNUSED(_ssid)
    //TODO complete me to be more verbose on status reporting
    _pong.Status = enuStatus::Ok;
    _pong.Message = QString("%1/%2").arg(
                TSMonitor::instance().connectedServers()).arg(gConfigs::TranslationServers.size());
}

}
}
