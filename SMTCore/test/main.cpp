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
#include "libTargomanCore/Private/GlobalConfigs.h"
#include "libTargomanCommon/Logger.h"
//#include "libTargomanCore/clsTranslator.h"
#include "libTargomanCommon/clsSafeCoreApplication.h"




#include <QDebug>


using namespace Targoman::Common;
using namespace Targoman::Common::Configuration;
using namespace Targoman::Core;

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
    TARGOMAN_IO_SETTINGS.setDefault();
    //TARGOMAN_IO_SETTINGS.Info.setLevel(5);
    //TARGOMAN_IO_SETTINGS.setSilent();

    try{
        clsSafeCoreApplication App(argc, argv);

        printMemoryUsage("@first");
#if 0
        ConfigManager::instance().init("kdsjh", QStringList()<<"-c"<<"../../Example/Model-fa2en/Targoman.conf");
#else
        ConfigManager::instance().init("kdsjh", QStringList()<<"-c"<<"../../Example/Model-fa2en.full/Targoman.conf");
#endif
        printMemoryUsage("after init");
        clsTranslator::init(ConfigManager::instance().configFilePath());

        printMemoryUsage("after load all");
        clsTranslator MyTranslator(QString::fromUtf8(argv[1]));
        MyTranslator.translate();
        printMemoryUsage("after translate");
    }catch(exTargomanBase& e){
        qDebug()<<e.what();
        TargomanError(e.what());
    }

    TargomanInfo(1,"Entering the infinite loop...");
   // while(1);
}


