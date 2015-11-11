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
using namespace Configuration;

void appTargomanSMTConsole::slotExecute()
{
    try{
        switch(gConfigs::Mode.value()){
        case enuAppMode::MakeBinary:
            Translator::init(ConfigManager::instance().configSettings());
            Translator::saveBinaryRuleTable(gConfigs::OutputFile.value());
            break;
        case enuAppMode::Training:
            //TODO Implement training
            break;
        case enuAppMode::Translation:
            TranslationWriter::instance(); //Just to initialize first instance in order to suppress multithreaded instantiation

            if(gConfigs::InputText.value().size()){
                Translator::init(ConfigManager::instance().configSettings());
                TranslationWriter::instance().writeTranslation(1,
                                                               Translator::translate(gConfigs::InputText.value(), true).Translation);
            } else if (gConfigs::InputFile.value().size()) {
                QFile InFile(gConfigs::InputFile.value());
                if (InFile.open(QFile::ReadOnly) == false)
                    throw exTargomanSMTConsole("Unable to open <" + InFile.fileName() + "> for reading.");

                QTextStream InStream(&InFile);
                InStream.setCodec("UTF-8");
                QThreadPool::globalInstance()->setMaxThreadCount(gConfigs::MaxThreads.value());
                Translator::init(ConfigManager::instance().configSettings());
                int Index = 0;
                while(InStream.atEnd() == false){
                    QThreadPool::globalInstance()->start(new clsTranslationJob(++Index, InStream.readLine()));
                }
                QThreadPool::globalInstance()->waitForDone();
                TranslationWriter::instance().finialize();
            }else{
                TargomanWarn(1,"No job to be done!!!");
            }
            break;
        default:
            break;
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
