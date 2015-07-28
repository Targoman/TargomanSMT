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

#include "UnitTest.h"

#include "libTargomanTextProcessor/TextProcessor.h"
using namespace Targoman::NLPLibs;

#define VERIFY_NORMALIZE(_lang, _check, _desired) \
    Targoman::NLPLibs::TargomanTextProcessor::instance().normalizeText(QStringLiteral(_check), false, _lang) == QStringLiteral(_desired)

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
//    QVERIFY(VERIFY_NORMALIZE("fa",
//    "نا مردترینشان",
//    "نامردترین‌شان"
//    ));
    QVERIFY(VERIFY_NORMALIZE("fa",
    "نا مردیهایشان",
    "نامردی‌هایشان"
    ));

    QVERIFY(VERIFY_NORMALIZE("",
    "ﺍﺟﺘﻤﺎﻋـﯽ ـ ﺳﻴﺎﺳﯽ",
    "اجتماعی - سیاسی"
    ));

    QVERIFY(VERIFY_NORMALIZE("fa",
    "بی شعوری ها یشان",
    "بی‌شعوری‌هایشان"
    ));

    QVERIFY(VERIFY_NORMALIZE("fa",
                             "می دهند"
                             , "می‌دهند"));
    QVERIFY(VERIFY_NORMALIZE("fa",
                             "اویزه",
                             "آویزه"));
//    QVERIFY(VERIFY_NORMALIZE("fa",
//                            "\"کوری\"",
//                            "\" کوری \""
//                            ));
    QVERIFY(VERIFY_NORMALIZE("fa",
                            "غیبگو تر",
                            "غیبگوتر"
                            ));
    QVERIFY(VERIFY_NORMALIZE("fa",
                            "غیبگو تر ها",
                            "غیبگوترها"
                            ));
    QVERIFY(VERIFY_NORMALIZE("fa",
                            "غیبگو ترین ها",
                            "غیبگوترین‌ها"
                            ));
    QVERIFY(VERIFY_NORMALIZE("fa",
                            "غیبگو ترین هایشان",
                            "غیبگوترین‌هایشان"
                            ));
    QVERIFY(VERIFY_NORMALIZE("fa",
                            "می زند",
                            "می‌زند"
                            ));
    QVERIFY(VERIFY_NORMALIZE("fa",
                            "می گوید",
                            "می‌گوید"
                            ));
//    QVERIFY(VERIFY_NORMALIZE("fa",
//                            "ماهیانه ای",
//                            "ماهیانه‌ای"
//                            ));
    QVERIFY(VERIFY_NORMALIZE("fa",
                            "مهم",
                            "مهم"
                            ));
    QVERIFY(VERIFY_NORMALIZE("fa",
                            "میسر",
                            "میسر"
                            ));
    QVERIFY(VERIFY_NORMALIZE("fa",
                            "صرفاً",
                            "صرفا"
                            ));
    QVERIFY(VERIFY_NORMALIZE("fa",
                            "صرفا\"",
                            "صرفا"
                            ));
    QVERIFY(VERIFY_NORMALIZE("fa",
                            "ماههای",
                            "ماهه‌ای"
                            ));
//    QVERIFY(VERIFY_NORMALIZE("fa",
//                            "چشمهاش",
//                            "چشمهاش"
//                            ));
    QVERIFY(VERIFY_NORMALIZE("fa",
                            "می کند و",
                            "می‌کند و"
                            ));
//    QVERIFY(VERIFY_NORMALIZE("fa",
//                            "کشته شده اند",
//                            "کشته شده‌اند"
//                            ));

//    QVERIFY(VERIFY_NORMALIZE("fa",
//                            "اشغال",
//                            "اشغال"
//                            ));

//    QVERIFY(VERIFY_NORMALIZE("fa",
//                            "از تاثیر مثبت این خاطره هم کاری برنیامده است",
//                            "از تاثیر مثبت این خاطره هم کاری برنیامده است"
//                            ));

}
