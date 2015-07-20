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
#include <QFile>
#include <QTextStream>
#include "appTargomanLoadBalancer.h"
#include "libTargomanCommon/Configuration/ConfigManager.h"
#include "libTargomanCommon/CmdIO.h"
#include "Modules/TSMonitor.h"
#include "libTargomanCommon/SimpleAuthentication.h"

namespace Targoman {
namespace Apps {

using namespace Common;
using namespace Modules;
void appTargomanLoadBalancer::slotExecute()
{
    try{

        connect(&Configuration::ConfigManager::instance(),
                SIGNAL(sigPing(Targoman::Common::stuPong&)),
                this,
                SLOT(slotPong(Targoman::Common::stuPong&)),
                Qt::DirectConnection);
        connect(&Configuration::ConfigManager::instance(),
                SIGNAL(sigValidateAgent(QString&,QString,QString,bool&,bool&)),
                this,
                SLOT(slotValidateAgent(QString&,QString,QString,bool&,bool&)),
                Qt::DirectConnection);

        Modules::TSMonitor::instance().start();
        while(true){
            try{
                sleep(1);
                Modules::TSMonitor::instance().bestServerIndex();
                Configuration::ConfigManager::instance().startAdminServer();
                break;
            }catch(exTargomanLoadBalancer &e){
                TargomanInfo(1,"Waiting for at least one server to be available");
            }
        }
    }catch(exTargomanBase& e){
        TargomanError(e.what());
    }catch (std::exception &e){
        TargomanError(e.what());
    }catch(...){
        TargomanError("FATAL Unrecognized exception");
    }
}

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

void appTargomanLoadBalancer::slotPong(stuPong &_pong)
{
    //TODO complete me to be more verbose on status reporting
    _pong.Status = enuStatus::Ok;
    _pong.Message = QString("%1/%2").arg(TSMonitor::instance().connectedServers()).arg(gConfigs::TranslationServers.size());
}

}
}
