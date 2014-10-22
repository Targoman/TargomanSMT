/*************************************************************************
 * Copyright © 2012-2014, Targoman.com
 *
 * Published under the terms of TCRL(Targoman Community Research License)
 * You can find a copy of the license file with distributed source or
 * download it from http://targoman.com/License.txt
 *
 *************************************************************************/
/**
 @author S. Mohammad M. Ziabary <smm@ziabary.com>
 */

#include "libTargomanCommon/CmdIO.h"
#include "libTargomanCommon/Logger.h"
#include "libTargomanCommon/exTargomanBase.h"
#include "libTargomanCommon/clsSafeCoreApplication.h"
#include "libTargomanCommon/Macros.h"

#include <iostream>

using namespace Targoman::Common;

class exSample: public exTargomanBase
{
public:
  exSample(const QString& _message, int _line) throw () :
    exTargomanBase(_message, _line)
  {
    this->Message.append(" >;exSample:");
  }
};

TARGOMAN_ADD_EXCEPTION_HANDLER(exSample2, exSample);

class clsSample
{
  public:
  clsSample()
  {
    TargomanDebugLine;
    TargomanDebug(5, "Hello World!");
    TargomanDebug(5,"%s", "Hello World!!");
  }
};

TARGOMAN_DEFINE_ENHANCED_ENUM_BEGIN(enuTest)
Val0,
Val1,
Val2,
Val3
TARGOMAN_DEFINE_ENHANCED_ENUM_STRINGS
"Val0",
"Val1",
"Val2",
"Val3"
TARGOMAN_DEFINE_ENHANCED_ENUM_END

void checkOutput()
{
    TargomanDebugLine
    TargomanDebug(6,"DEBUG"<<" Stream"<<1);
    TargomanDebug(5,"%s b=%d","DEBUG",2);
    TargomanInlineDebug(5,"Checking...");
    TargomanFinishInlineDebug(TARGOMAN_COLOR_HAPPY,"Ok");

    TargomanWarn(6,"Warn"<<" Stream"<<1);
    TargomanWarn(5,"%s b=%d","Warn",2);

    TargomanInfo(6,"Info"<<" Stream"<<1);
    TargomanInfo(5,"%s b=%d","Info",2);
    TargomanInlineInfo(5,"Checking...");
    TargomanFinishInlineInfo(TARGOMAN_COLOR_ERROR,"Failed");

    TargomanOut(6,"Normal"<<" Stream"<<1);
    TargomanOut(5,"%s b=%d","Normal",2);
    TargomanInlineOut(5,"Checking...");
    TargomanFinishInlineOut(TARGOMAN_COLOR_WARNING,"Warn");

    TargomanHappy(6,"Happy"<<" Stream"<<1);
    TargomanHappy(5,"%s b=%d","Happy",2);

    TargomanError("Error"<<" Stream"<<1);
    TargomanError("%s b=%d","Error",2);
}

int main(int argc, char *argv[])
{
  Targoman::Common::printLoadedLibs();

  QString ActorUUID;

  TARGOMAN_REGISTER_ACTOR("testLibCommon");
  OUTPUT_SETTINGS_DEBUG.set(10,true,true,true,true);
  OUTPUT_SETTINGS_ERROR.set(10,true,true,true,true);
  OUTPUT_SETTINGS_WARNING.set(10,true,true,true,true);
  OUTPUT_SETTINGS_INFO.set(10,true,true,true,true);
  OUTPUT_SETTINGS_HAPPY.set(10,true,true,true,true);
  Targoman::Common::Logger::instance().init("log.log");

  Targoman::Common::OUTPUT_SETTINGS_SHOWCOLORED = false;
  checkOutput();
  Targoman::Common::OUTPUT_SETTINGS_SHOWCOLORED = true;
  checkOutput();
  Targoman::Common::silent();
  qDebug("************************* Silented");
  checkOutput();
  qDebug("************************* After Silence");

  OUTPUT_SETTINGS_DEBUG.set(5,true);
  OUTPUT_SETTINGS_ERROR.set(5,true);
  OUTPUT_SETTINGS_WARNING.set(5,true);
  OUTPUT_SETTINGS_INFO.set(5,true);
  OUTPUT_SETTINGS_HAPPY.set(5,true);
  OUTPUT_SETTINGS_NORMAL.set(5,true);

  checkOutput();


  clsSample Sample;

  clsSafeCoreApplication App(argc, argv);

  TargomanLogWarn(5, " Log Warn");
  TargomanLogInfo(5, " Log Info");
  TargomanLogHappy(5, " Log Happy");
  TargomanLogDebug(5, " Log Debug");
  TargomanLogError(" Log Error");


  try{
    throw exSample2("Hello Wolrd to exception");
    throw exSample("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor ", __LINE__);
  }catch (exSample& e){
    TargomanError(" %s", qPrintable(e.what()));
  }catch (exTargomanBase& e){
    TargomanError(" %s", qPrintable(e.what()));
  }

  try{
    throw exTargomanInvalidParameter("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor ", __LINE__);
  }catch (exSample& e){
    TargomanError(" %s", qPrintable(e.what()));
  }catch (exTargomanBase& e){
    TargomanError(" %s", qPrintable(e.what()));
  }
/**/
  //  return a.exec();*/
}


