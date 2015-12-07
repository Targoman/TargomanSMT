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

#include "libTargomanCommon/CmdIO.h"
#include "libTargomanCommon/Logger.h"
#include "libTargomanCommon/exTargomanBase.h"
#include "libTargomanCommon/clsSafeCoreApplication.h"
#include "libTargomanCommon/Macros.h"
#include "libTargomanCommon/clsCmdProgressBar.h"
#include "libTargomanCommon/HashFunctions.hpp"
#include "libTargomanCommon/Configuration/ConfigManager.h"
#include "libTargomanCommon/Configuration/Validators.hpp"
#include "libTargomanCommon/Configuration/tmplModuleConfig.hpp"
#include "libTargomanCommon/tmplBoundedCache.hpp"
#include "libTargomanCommon/FStreamExtended.h"
#include "libTargomanCommon/PrefixTree/tmplPrefixTree.h"
#include "libTargomanCommon/Configuration/tmplConfigurableArray.hpp"
#include "libTargomanCommon/SimpleAuthentication.h"

#include <iostream>
#include <unistd.h>

using namespace Targoman::Common;

/*static Configuration::tmplConfigurable<qint8> A("/","fjkdfjkdsfjk",123);
static Configuration::tmplConfigurable<FilePath_t> B(
        "/s","fjkdfjkdsfjk","123",
        Configuration::Validators::tmplPathAccessValidator<
        (enuPathAccess::Type)(enuPathAccess::Dir | enuPathAccess::Readable)>);

//static Configuration::tmplConfigurable<double> C("/s2","fjkdfjkdsfjk",123,
//                                                  Configuration::Validators::tmplNumericValidator<double, -12, 5>);

static Configuration::tmplConfigurable<QString> D("/s2","fjkdfjkdsfjk","123",
                                                  [](const Targoman::Common::Configuration::intfConfigurable& ,QString& ){
                                                      return true;
                                                  });

static Configuration::tmplModuleConfig  LM("/Modules/LM", "LM Module", "Default");
*/
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

TARGOMAN_DEFINE_ENHANCED_ENUM(enuTest,
                              Val0,
                              Val1,
                              Val2,
                              Val3
                              );

TARGOMAN_DEFINE_ENUM(enuBaba,
                     qq=2,
                     cc=3)

void checkOutput()
{

    TargomanDebugLine
            QString myQString ("Debug QString");
    std::string myString ("Debug String");

    TargomanDebug(6,"DEBUG"<<" Stream"<<1);
    TargomanDebug(5,"%s b=%d","DEBUG",2);
    //    TargomanDebug(5,myQString);
    //    TargomanDebug(5,"myString");
    TargomanInlineDebug(5,"Checking...");
    //    TargomanInlineDebug(5,myQString);
    TargomanFinishInlineDebug(TARGOMAN_COLOR_ERROR,"Ok");

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


class SampleLamda{
public:
    SampleLamda(const std::function< bool(int) >& _crossValidator){
        this->CrossValidator = _crossValidator;
    }

    std::function<bool(int)> CrossValidator;
};

struct stuServers{
    stuServers(const QString& _basePath) :
        Address(_basePath + "Address",
                "kjkjkldfj"),
        Active(_basePath + "Active",
               "fdsfdf",
               true)
    { }

    Targoman::Common::Configuration::tmplConfigurable<QString> Address;
    Targoman::Common::Configuration::tmplConfigurable<bool>    Active;
};

static Targoman::Common::Configuration::tmplConfigurableArray<stuServers> Servers(
        "App/Servers",
        "sdfdf",
        1,
        5);

#include "FastOperations.hpp"
#include "sampleAgent.hpp"
#include "test.h"






int main(int argc, char *argv[])
{
    try{
    Targoman::Common::printLoadedLibs();

/*    tmplExpirableCache<QMap,  QString, int> FF;
    tmplExpirableCache<QMap,  QString, int> DD(FF);
*/

    QCoreApplication App(argc, argv);

/*    qDebug()<<TestE.value();
    WildPattern.setFromVariant("*.xml");
    qDebug()<<WildPattern.value();
    RegexPattern.setFromVariant("^[a-z]*");
    qDebug()<<RegexPattern.value();

    qDebug()<<SimpleAuthentication::hashPass("mehran", "1234");
    exit(0);
*/


    /*Targoman::Common::tmplExpirableCache<QHash,int,QString> A;
    qDebug()<<getTypeStr(A);
    enuBaba::Type B = enuBaba::cc;
    qDebug()<<getTypeStr(B);

    A.clear();
*/
    clsCmdProgressBar PB("Marquee");
    for (int i=0;i<10;i++){
        PB.setValue(i);
        usleep(10000);
    }
    PB.finalize(true, 100000);

    //  std::function<bool(int)> Lambda = ;
    SampleLamda AAA([] (int x) {
        return x > 0; }
    );

    std::cout<<AAA.CrossValidator(5)<<std::endl;
    std::cout<<AAA.CrossValidator(-5)<<std::endl;


        QString ActorUUID;

        TARGOMAN_REGISTER_ACTOR("testLibCommon");
        TARGOMAN_IO_SETTINGS.setFull();
        Targoman::Common::Logger::instance().init("log.log");
        Configuration::ConfigManager::instance().init("Testlicense", App.arguments().mid(1));
        TARGOMAN_IO_SETTINGS.setFull();

        if (Configuration::ConfigManager::instance().isNetworkManagable()){
            new clsSampleAgent;
            QTimer::singleShot(2000,&Configuration::ConfigManager::instance(), SLOT(startAdminServer()));
            return App.exec();
        }

        Targoman::Common::TARGOMAN_IO_SETTINGS.ShowColored = false;
        checkOutput();
        Targoman::Common::TARGOMAN_IO_SETTINGS.ShowColored = true;
        checkOutput();
        Targoman::Common::TARGOMAN_IO_SETTINGS.setSilent();
        qDebug("************************* Silented");
        checkOutput();
        qDebug("************************* After Silence");

        TARGOMAN_IO_SETTINGS.setDefault(6, 6);

        checkOutput();

        //A.toVariant();

        clsSample Sample;

        clsSafeCoreApplication App2(argc, argv);

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

        QString Test = "this is a test";
        TargomanHappy(1,HashFunctions::murmurHash32(Test.toUtf8().constData(), Test.length(),1));
        TargomanHappy(1,HashFunctions::murmurHash32(Test.toUtf8().constData(), Test.length(),2));
        TargomanHappy(1,HashFunctions::murmurHash32(Test.toUtf8().constData(), Test.length(),1));

        TargomanHappy(1,HashFunctions::murmurHash64(Test.toUtf8().constData(), Test.length(),1));
        TargomanHappy(1,HashFunctions::murmurHash64(Test.toUtf8().constData(), Test.length(),2));
        TargomanHappy(1,HashFunctions::murmurHash64(Test.toUtf8().constData(), Test.length(),1));
        TargomanHappy(1,HashFunctions::murmurHash64(Test.toUtf8().constData(), Test.length(),1192));

        // Configuration::ConfigManager::instance().init("Sample");
        // Configuration::ConfigManager::instance().init("Sample", QStringList()<<"-h");
        clsCmdProgressBar PB2("Test Progress", 10000);
        for (int i=0; i< 10001; i++){
            PB2.setValue(i);
            usleep(100);
        }

        clsCmdProgressBar PBMarquee("Test Marquee", 0);
        for (int i=0; i< 10001; i++){
            PBMarquee.setValue(i);
            usleep(100);
        }
        PBMarquee.finalize(true);

        TARGOMAN_IO_SETTINGS.setFull();

    }catch(exTargomanBase &e){
        TargomanError(e.what());
        return 1;
    }

    return 0;
}


