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

#include <QFile>
#include <QTextStream>
#include "appTargomanSMTConsole.h"
#include "libTargomanCommon/Configuration/ConfigManager.h"
#include "libTargomanCommon/CmdIO.h"
#include "libTargomanSMT/Translator.h"
#include "Configs.h"
#include "TranslationWriter.h"
#include "clsTranslationJob.h"

namespace Targoman {
namespace Apps {

using namespace SMT;
using namespace Common;

void appTargomanSMTConsole::slotExecute()
{
    try{
        TranslationWriter::instance(); //Just to initialize first instance in order to suppress multithreaded instantiation

        if(gConfigs::InputText.value().size()){
            Translator::init(Configuration::ConfigManager::instance().configFilePath());
            TranslationWriter::instance().writeTranslation(1,
                                                           Translator::translate(gConfigs::InputText.value(), true).Translation);
        } else if (gConfigs::InputFile.value().size()) {
            QFile InFile(gConfigs::InputFile.value());
            if (InFile.open(QFile::ReadOnly) == false)
                throw exTargomanSMTConsole("Unable to open <" + InFile.fileName() + "> for reading.");

            QTextStream InStream(&InFile);
            InStream.setCodec("UTF-8");
            QThreadPool::globalInstance()->setMaxThreadCount(gConfigs::MaxThreads.value());
            Translator::init(Configuration::ConfigManager::instance().configFilePath());
            int Index = 0;
            while(InStream.atEnd() == false){
                QThreadPool::globalInstance()->start(new clsTranslationJob(++Index, InStream.readLine()));
            }
            QThreadPool::globalInstance()->waitForDone();
            TranslationWriter::instance().finialize();
        }else{
            TargomanWarn(1,"No job to be done!!!");
        }

        QCoreApplication::exit(0);
        return;
    }catch(exTargomanBase& e){
        TargomanError(e.what());
    }catch (std::exception &e){
        TargomanError(e.what());
    }catch(...){
        TargomanError("FATAL Unrecognized exception");
    }

    QCoreApplication::exit(-1);
}

}

}
