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

#include "libTargomanCommon/Configuration/ConfigManager.h"
#include "libTargomanSMT/Private/GlobalConfigs.h"
#include "libTargomanCommon/Logger.h"
#include "libTargomanSMT/clsTranslator.h"
#include "libTargomanCommon/clsSafeCoreApplication.h"

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
    qDebug()<<"********************************************************************";
    qDebug()<<"Memory Stat ["<<_stepString<<
              "]: VirtPeak:"<<VirtualMemory<<
              " CurrVirt:"<<VirtualMemory<<
              " ResPeak:"<<ResidentPeak<<
              " CurRes:"<<ResidentMemory<<
              " Shared:"<<SharedMemory;
    qDebug()<<"********************************************************************";
}


int main(int argc, char *argv[])
{
    //TARGOMAN_IO_SETTINGS.setDefault();
    TARGOMAN_IO_SETTINGS.setDefault(7);
    TARGOMAN_IO_SETTINGS.Debug.setDetails(true);
    //TARGOMAN_IO_SETTINGS.setSilent();

    try{
        clsSafeCoreApplication App(argc, argv);

        printMemoryUsage("@first");
        ConfigManager::instance().init("dummy-license", QStringList()<<"-c"<<"./Targoman.conf");

        printMemoryUsage("after init");
        clsTranslator::init(ConfigManager::instance().configFilePath());
        printMemoryUsage("after load all");


        QFile File(argv[1]);
        QTextStream Stream(&File);
        Stream.setCodec("UTF-8");
        File.open(QFile::ReadOnly);

        while(Stream.atEnd() == false)
        {
            clsTranslator MyTranslator(Stream.readLine());
            stuTranslationOutput output = MyTranslator.translate();
            std::cout << output.Translation.toStdString() << std::endl;
            printMemoryUsage("after translate");
        }

    }catch(exTargomanBase& e){
        qDebug()<<e.what();
        TargomanError(e.what());
    }
}

