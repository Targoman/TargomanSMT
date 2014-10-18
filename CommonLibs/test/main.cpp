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

#include "libTargomanCommon/Debug.h"
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
    this->Message.append(">exSample: ");
  }
};

TARGOMAN_ADD_EXCEPTION_HANDLER(exSample2, exSample)

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


int main(int argc, char *argv[])
{
  Targoman::Common::printLoadedLibs();

//  QString ActorUUID;

//  ETSLOG_REGISTER_ACTOR("testLibDefinition");
  OUTPUT_SETTINGS_DEBUG.set(10,true,true,true,true);
  OUTPUT_SETTINGS_ERROR.set(10,true,true,true,true);
  OUTPUT_SETTINGS_WARNING.set(10,true,true,true,true);
  OUTPUT_SETTINGS_INFO.set(10,true,true,true,true);
  OUTPUT_SETTINGS_HAPPY.set(10,true,true,true,true);
  Targoman::Common::OUTPUT_SETTINGS_SHOWCOLORED = true;
//  ETSLOG_INIT("log.log",5);

  TargomanDebugLine
  TargomanDebug(5,"DEBUG");
  TargomanDebug(5,"%s b=%d","DEBUG",2);

  TargomanWarn(5,"Warn");
  TargomanWarn(5,"%s b=%d","Warn",2);

  TargomanInfo(5,"Info");
  TargomanInfo(5,"%s b=%d","Info",2);

  TargomanHappy(5,"Happy");
  TargomanHappy(5,"%s b=%d","Happy",2);

  TargomanError("Error");
  TargomanError("%s b=%d","Error",2);

  clsSample Sample;

  clsSafeCoreApplication App(argc, argv);

//  ETSLOG_WARN(5, "After Log")
  try{
    throw exSample2("Hello Wolrd");
    throw exSample("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor ", __LINE__);
  }catch (exSample& e){
    TargomanError(" Sample: %s", qPrintable(e.what()));
  }catch (exTargomanBase& e){
    TargomanError(" Base: %s", qPrintable(e.what()));
  }
/**/
  //  return a.exec();*/
}


