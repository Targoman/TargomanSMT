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

#include "UnitTest.h"

using namespace Targoman::Common;
using namespace Targoman::NLPLibs;

void UnitTest::initTestCase(){
    TARGOMAN_REGISTER_ACTOR("testLibCommon");
    OUTPUT_SETTINGS_DEBUG.set(10,true,true,true);
    OUTPUT_SETTINGS_ERROR.set(10,true,true,true);
    OUTPUT_SETTINGS_WARNING.set(10,true,true,true);
    OUTPUT_SETTINGS_INFO.set(10,true,true,true);
    OUTPUT_SETTINGS_HAPPY.set(10,true,true,true);
    Targoman::Common::Logger::instance().init("/dev/null");
}

QTEST_MAIN(UnitTest)

