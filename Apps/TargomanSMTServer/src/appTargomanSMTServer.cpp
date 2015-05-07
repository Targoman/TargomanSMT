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
 @author Behrooz Vedadian <vedadian@gmail.com>
 */

#include <unistd.h>
#include <QFile>
#include <QTextStream>
#include <sys/sysinfo.h>
#include "appTargomanSMTServer.h"
#include "libTargomanCommon/Configuration/ConfigManager.h"
#include "libTargomanCommon/CmdIO.h"
//#include "libTargomanSMT/clsTranslator.h"
#include "Configs.h"
#include "libTargomanCommon/SimpleAuthentication.h"
//#include "TranslationWriter.h"
//#include "clsTranslationJob.h"

namespace Targoman {
namespace Apps {

//using namespace SMT;
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


        Configuration::ConfigManager::instance().startAdminServer();

       // while(1)
       //         sleep(10);
        /*
        clsTranslator::init(Configuration::ConfigManager::instance().configFilePath());

        if(gConfigs::InputText.value().size()){
            clsTranslator Translator(gConfigs::InputText.value());
            TranslationWriter::instance().writeTranslation(1,Translator.translate(true).Translation);
        } else if (gConfigs::InputFile.value().size()) {
            QFile InFile(gConfigs::InputFile.value());
            if (InFile.open(QFile::ReadOnly) == false)
                throw exTargomanSMTConsole("Unable to open <" + InFile.fileName() + "> for reading.");

            QTextStream InStream(&InFile);
            InStream.setCodec("UTF-8");
            QThreadPool::globalInstance()->setMaxThreadCount(gConfigs::MaxThreads.value());
            int Index = 0;
            while(InStream.atEnd() == false){
                QThreadPool::globalInstance()->start(new clsTranslationJob(++Index, InStream.readLine()));
            }
        }
*/
       // QCoreApplication::exit(0);
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
        Args.insert("TQ", 5); //TODO complete me
    }
    return stuRPCOutput(1, Args);
}

}
}
