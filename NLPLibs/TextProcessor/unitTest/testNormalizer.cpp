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

#include "libTargomanTextProcessor/TextProcessor.h"
using namespace Targoman::NLPLibs;

#define VERIFY_NORMALIZE(_lang, _check, _desired) \
    Targoman::NLPLibs::TextProcessor::instance().normalizeText(QString::fromUtf8(_check), false, _lang) == \
        QString::fromUtf8(_desired)

void UnitTest::normalizeText()
{
    QVERIFY(VERIFY_NORMALIZE("fa",
    "من با دم خود می گفتم که با معرفت ترین ها یشان هم نا رفیق بوده اند",
    "من با دم خود می‌گفتم که بامعرفت‌ترین‌هایشان هم نارفیق بوده‌اند"
    ));
    QVERIFY(VERIFY_NORMALIZE("fa",
    "",
    ""
    ));
    QVERIFY(VERIFY_NORMALIZE("fa",
    "",
    ""
    ));
    QVERIFY(VERIFY_NORMALIZE("fa",
    "",
    ""
    ));
    QVERIFY(VERIFY_NORMALIZE("fa",
    "",
    ""
    ));

}
