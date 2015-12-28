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
#include <sys/sysinfo.h>
#include "appTargomanSMTServer.h"
#include "libTargomanCommon/Configuration/ConfigManager.h"
#include "libTargomanCommon/CmdIO.h"
#include "Configs.h"
#include "libTargomanCommon/SimpleAuthentication.h"
#include "libTargomanTextProcessor/TextProcessor.h"
#include "clsTranslationJob.h"

namespace Targoman {
namespace Apps {


using namespace Common;
using namespace Common::Configuration;

QString ActorUUID;

void appTargomanSMTServer::slotExecute()
{
    try{
        connect(&Configuration::ConfigManager::instance(),
                &Configuration::ConfigManager::sigPing,
                this,
                &appTargomanSMTServer::slotPong,
                Qt::DirectConnection);
        connect(&Configuration::ConfigManager::instance(),
                &Configuration::ConfigManager::sigValidateAgent,
                this,
                &appTargomanSMTServer::slotValidateAgent,
                Qt::DirectConnection);

        clsTranslationJob::SourceLanguage =
                ConfigManager::instance().getConfig("/Common/Language/Source").toString();

        Targoman::SMT::Translator::init(ConfigManager::instance().configSettings());
        Targoman::NLPLibs::TargomanTextProcessor::instance().init(ConfigManager::instance().configSettings());

        QThreadPool::globalInstance()->setMaxThreadCount(gConfigs::MaxThreads.value());
        Configuration::ConfigManager::instance().startAdminServer();

        /*clsTranslationJob* TRJ = new clsTranslationJob(false, false);
        TRJ->doJob("یا مدت برای یا مدت زمان");*/

    }catch(exTargomanBase& e){
        TargomanError(e.what());
        QCoreApplication::exit(-1);
    }catch (std::exception &e){
        TargomanError(e.what());
        QCoreApplication::exit(-1);
    }catch(...){
        TargomanError("FATAL Unrecognized exception");
        QCoreApplication::exit(-1);
    }
}

void appTargomanSMTServer::slotValidateAgent(QString &_user, const QString &_pass, const QString &_ip, bool &_canView, bool &_canChange)
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

void appTargomanSMTServer::slotPong(QString _ssid, stuPong &_pong)
{
    Q_UNUSED(_ssid)
    //TODO complete me to be more verbose on status reporting
    _pong.Status = enuStatus::Ok;
    _pong.Message = "Ok";
}

stuRPCOutput appTargomanSMTServer::rpcGetStatistics(const QVariantMap &)
{
    QVariantMap Args;
    struct sysinfo SysInfo;

    if (sysinfo(&SysInfo) == 0){
        float shiftfloat=(float)(1<<SI_LOAD_SHIFT);

        Args.insert("L1", qMin(int((float(SysInfo.loads[0])/shiftfloat/get_nprocs()) * 100.), 100));
        Args.insert("L15",qMin(int((float(SysInfo.loads[2])/shiftfloat/get_nprocs()) * 100.), 100));
        Args.insert("FM", qMin(int(double(SysInfo.freeram)/double(SysInfo.totalram) * 100.), 100));
        Args.insert("TQ", qMin(int(double(QThreadPool::globalInstance()->activeThreadCount()) /
                                   double(QThreadPool::globalInstance()->maxThreadCount())*100.),100));
    }

    return stuRPCOutput(1, Args);
}

stuRPCOutput appTargomanSMTServer::rpcTranslate(const QVariantMap &_args)
{
    QString Text  = _args.value("txt").toString();
    if (Text.isEmpty())
        throw exTargomanSMTServer("Obligatory argument 'txt' missing");

    TargomanLogDebug(7,QString("rpcTranslate(brief=%1,keep=%2,txt=%3) => Result not logged").arg(
                         _args.value("brief",false).toBool()).arg(
                         _args.value("keep",false).toBool()).arg(
                         _args.value("txt").toString().replace("\n","\\n")));
    QVariantMap Result;
    Result.insert("t",clsTranslationJob(_args.value("brief",false).toBool(),
                                        _args.value("keep",false).toBool()).doJob(Text));
    return stuRPCOutput(1, Result);
}

}
}
