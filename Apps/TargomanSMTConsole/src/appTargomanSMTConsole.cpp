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
#include "libTargomanSMT/clsTranslator.h"
#include "Configs.h"
#include "TranslationWriter.h"

namespace Targoman {
namespace Apps {

using namespace SMT;
using namespace Common;

appTargomanSMTConsole::appTargomanSMTConsole(int _argc, char** _argv) :
    clsSafeCoreApplication(_argc, _argv)
{
}

void appTargomanSMTConsole::slotExecute()
{
    try{
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
            while(InStream.atEnd() == false){

            }
        }

        this->exit(0);
    }catch(exTargomanBase& e){
        TargomanError(e.what());
    }catch (std::exception &e){
        TargomanError(e.what());
    }catch(...){
        TargomanError("FATAL Unrecognized exception");
    }

    this->exit(-1);
}

}

}
