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
    Targoman::NLPLibs::TextProcessor::instance().normalizeText(QStringLiteral(_check), false, _lang) == QStringLiteral(_desired)

void UnitTest::normalizeText()
{
    QVERIFY(VERIFY_NORMALIZE("fa",
    "من با دم خود میگفتم که با معرفت ترین ها یشان هم نا رفیق بوده اند",
    "من با دم خود می‌گفتم که بامعرفت‌ترین‌هایشان هم نارفیق بوده‌اند"
    ));
    QVERIFY(VERIFY_NORMALIZE("fa",
    "خانه ی",
    "خانه"
    ));
    QVERIFY(VERIFY_NORMALIZE("fa",
    "می روم",
    "می‌روم"
    ));
    QVERIFY(VERIFY_NORMALIZE("fa",
    "میخوردم",
    "می‌خوردم"
    ));
    QVERIFY(VERIFY_NORMALIZE("fa",
    "می خورده‌ام",
    "می‌خورده‌ام"
    ));
    QVERIFY(VERIFY_NORMALIZE("fa",
    "می خورده‌بودم",
    "می‌خورده‌بودم"
    ));
    QVERIFY(VERIFY_NORMALIZE("fa",
    "نا مردهایشان",
    "نامردهایشان"
    ));
    QVERIFY(VERIFY_NORMALIZE("fa",
    "نامردهایشان",
    "نامردهایشان"
    ));
    QVERIFY(VERIFY_NORMALIZE("fa",
    "نا مردترینشان",
    "نامردترین‌شان"
    ));
    QVERIFY(VERIFY_NORMALIZE("fa",
    "نا مردیهایشان",
    "نامردی‌هایشان"
    ));
    QVERIFY(VERIFY_NORMALIZE("fa",
    "بی شعوری ها یشان",
    "بی‌شعوری‌هایشان"
    ));




}
