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

#include "UnitTest.h"

#define VERIFY_IXML2TXT(_check, _desired) \
    Targoman::NLPLibs::TargomanTextProcessor::instance().ixml2Text(QStringLiteral(_check)) == QStringLiteral(_desired)

void UnitTest::ixml2Text()
{
//    QVERIFY(VERIFY_IXML2TXT("this ' <Number>12</Number> ' , \" I 'm \" a test for \" me \" and <URL>you</URL> . \"",
//                            "this '12', \"I'm\" a test for \"me\" and you.\""));
//    QVERIFY(VERIFY_IXML2TXT("این نیز بگذرد .",
//                            "این نیز بگذرد."));
}

