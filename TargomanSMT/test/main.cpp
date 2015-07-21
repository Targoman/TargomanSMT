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
 * @author Behrooz Vedadian <vedadian@targoman.com>
 * @author Saeed Torabzadeh <saeed.torabzadeh@targoman.com>
 */

#include "libTargomanCommon/Configuration/ConfigManager.h"
#include "libTargomanSMT/Private/GlobalConfigs.h"
#include "libTargomanCommon/Logger.h"
#include "libTargomanSMT/Translator.h"
#include "libTargomanCommon/clsSafeCoreApplication.h"


//JUST FOR DEBUG
#include "Private/RuleTable/clsBinaryRuleTable.h"


#include <QDebug>

#include <iostream>

using namespace Targoman::Common;
using namespace Targoman::Common::Configuration;
using namespace Targoman::SMT;

void printMemoryUsage(const QString& _stepString){
    QFile ProcessMemStatFile(QString("/proc/%1/status").arg(QCoreApplication::applicationPid()));
    ProcessMemStatFile.open(QFile::ReadOnly);
    QString Line;
    QTextStream ProcessMemInfoStream(ProcessMemStatFile.readAll());

    QString VirtualPeak;
    QString ResidentPeak;
    QString VirtualMemory;
    QString ResidentMemory;
    QString SharedMemory;


    while(ProcessMemInfoStream.atEnd() == false)
    {
      Line = ProcessMemInfoStream.readLine();
      if (Line.startsWith("VmPeak"))
        VirtualPeak = Line.replace(" ","").split(":").at(1).trimmed();
      else if (Line.startsWith("VmHWM"))
        ResidentPeak =Line.replace(" ","").split(":").at(1).trimmed();
      else if (Line.startsWith("VmSize"))
        VirtualMemory = Line.replace(" ","").split(":").at(1).trimmed();
      else if (Line.startsWith("VmRSS"))
        ResidentMemory = Line.replace(" ","").split(":").at(1).trimmed();
      else if (Line.startsWith("VmLib"))
        SharedMemory = Line.replace(" ","").split(":").at(1).trimmed();
    }
    std::cerr<<"********************************************************************"<<std::endl;
    std::cerr<<"Memory Stat ["<<qPrintable(_stepString)<<
              "]: VirtPeak:"<<qPrintable(VirtualMemory)<<
              " CurrVirt:"<<qPrintable(VirtualMemory)<<
              " ResPeak:"<<qPrintable(ResidentPeak)<<
              " CurRes:"<<qPrintable(ResidentMemory)<<
              " Shared:"<<qPrintable(SharedMemory)<<std::endl;
    std::cerr<<"********************************************************************"<<std::endl;
}

int main(int argc, char *argv[])
{
    //TARGOMAN_IO_SETTINGS.setDefault();
    //TARGOMAN_IO_SETTINGS.setDefault(7);
    //TARGOMAN_IO_SETTINGS.Debug.setDetails(true);
    TARGOMAN_IO_SETTINGS.setSilent();
    Logger::instance().setActive(false);

    try{
        clsSafeCoreApplication App(argc, argv);

        printMemoryUsage("@first");
        ConfigManager::instance().init("dummy-license", QStringList()<<"-c"<<"./Targoman.conf");

        //Targoman::SMT::Private::RuleTable::clsBinaryRuleTable BRT(1);

        printMemoryUsage("after init");
        Translator::init(ConfigManager::instance().configFilePath());
        printMemoryUsage("after load all");

//        Translator::saveBinaryRuleTable("phrase-table.BIG.kenlm.bin");
//        return 0;

        QFile File(argv[1]);
        QTextStream Stream(&File);
        Stream.setCodec("UTF-8");
        File.open(QFile::ReadOnly);

        //int LineNumber = 0;

        while(Stream.atEnd() == false)
        {
            std::cout << Translator::translate(Stream.readLine(), true).Translation.toStdString() << std::endl;
            //printMemoryUsage(QString::number(++LineNumber) + ": after translate");
        }

    }catch(exTargomanBase& e){
        qDebug()<<e.what();
        TargomanError(e.what());
    }
}


