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
#include <sys/sysinfo.h>
#include "appTargomanSMTServer.h"
#include "libTargomanCommon/Configuration/ConfigManager.h"
#include "libTargomanCommon/CmdIO.h"
#include "Configs.h"
#include "libTargomanCommon/SimpleAuthentication.h"
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
                SIGNAL(sigPing(Targoman::Common::stuPong&)),
                this,
                SLOT(slotPong(Targoman::Common::stuPong&)),
                Qt::DirectConnection);
        connect(&Configuration::ConfigManager::instance(),
                SIGNAL(sigValidateAgent(QString&,QString,QString,bool&,bool&)),
                this,
                SLOT(slotValidateAgent(QString&,QString,QString,bool&,bool&)),
                Qt::DirectConnection);

    //        Translator::init(ConfigManager::instance().configFilePath());

        QThreadPool::globalInstance()->setMaxThreadCount(gConfigs::MaxThreads.value());
        Configuration::ConfigManager::instance().startAdminServer();
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

void appTargomanSMTServer::slotPong(stuPong &_pong)
{
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

    QVariantMap Result;
    Result.insert("t",clsTranslationJob(_args.value("brief",false).toBool(),
                                        _args.value("keep",false).toBool()).doJob(Text));
    return stuRPCOutput(1, Result);
}

}
}
