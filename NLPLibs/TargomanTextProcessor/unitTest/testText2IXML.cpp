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
#include <QTest>
#define VERIFY_TXT2IXML(_lang, _check, _desired) \
    Targoman::NLPLibs::TargomanTextProcessor::instance().text2IXML(QStringLiteral(_check), _lang, 0, false) == \
        QStringLiteral(_desired)

void UnitTest::text2IXML()
{
    QVERIFY(VERIFY_TXT2IXML("en","this is just  a test.", "this is just a test ."));
    QVERIFY(VERIFY_TXT2IXML("en","A simple \"Test\" for you.", "A simple \" Test \" for you ."));
    QVERIFY(VERIFY_TXT2IXML("en","A simple 'Test' for you.", "A simple ' Test ' for you ."));
    QVERIFY(VERIFY_TXT2IXML("fa","با سویه H1N1رخ داد","با سویه H1N1 رخ داد"));

    //Numbers
    QVERIFY(VERIFY_TXT2IXML("en","-12.5 -13 17,254.25", "<Number>-12.5</Number> <Number>-13</Number> <Number>17,254.25</Number>"));
    QVERIFY(VERIFY_TXT2IXML("en","asd12", "asd12"));
    QVERIFY(VERIFY_TXT2IXML("en","سلام12", "سلام <Number>12</Number>"));
    QVERIFY(VERIFY_TXT2IXML("en","12asd13", "<Number>12</Number> asd13"));
    QVERIFY(VERIFY_TXT2IXML("en","asd-12", "asd - <Number>12</Number>"));
    QVERIFY(VERIFY_TXT2IXML("en","-12asd", "<OrderedListItem>-</OrderedListItem> <Number>12</Number> asd"));

    QVERIFY(VERIFY_TXT2IXML("en","a asd12", "a asd12"));
    QVERIFY(VERIFY_TXT2IXML("en","a سلام12", "a سلام <Number>12</Number>"));
    QVERIFY(VERIFY_TXT2IXML("en","a 12asd13", "a <Number>12</Number> asd13"));
    QVERIFY(VERIFY_TXT2IXML("en","a asd-12", "a asd - <Number>12</Number>"));
    QVERIFY(VERIFY_TXT2IXML("en","a -12asd", "a - <Number>12</Number> asd"));
    QVERIFY(VERIFY_TXT2IXML("en","17/11/20001", "<Number>17</Number> / <Number>11</Number> / <Number>20001</Number>"));
    QVERIFY(VERIFY_TXT2IXML("fa"," یا49راتحقق می‌بخشد", "یا <Number>49</Number> راتحقق می‌بخشد"));
    QVERIFY(VERIFY_TXT2IXML("fa","و 1.6155فرانک سوییس در مقابل 1.5960", "و <Number>1.6155</Number> فرانک سوییس در مقابل <Number>1.5960</Number>"));
    QVERIFY(VERIFY_TXT2IXML("en"," to_1967_lines", "to _ <Number>1967</Number> _ lines"));
//    QVERIFY(VERIFY_TXT2IXML("گفت.مریم", "گفت . مریم"));


    QVERIFY(VERIFY_TXT2IXML("en","1st", "<Ordinals>1st</Ordinals>"));
    QVERIFY(VERIFY_TXT2IXML("en","a -20th", "a - <Ordinals>20th</Ordinals>"));
    QVERIFY(VERIFY_TXT2IXML("en","a 1380/2/1 b", "a <Number>1380</Number> / <Number>2</Number> / <Number>1</Number> b"));
    QVERIFY(VERIFY_TXT2IXML("en","1380/2/1", "<Number>1380</Number> / <Number>2</Number> / <Number>1</Number>"));

    //Ordered lists
    QVERIFY(VERIFY_TXT2IXML("en","12.5.asd", "<OrderedListItem>12.5.</OrderedListItem> asd"));
    QVERIFY(VERIFY_TXT2IXML("en","A.5.asd", "<OrderedListItem>A.5.</OrderedListItem> asd"));
    QVERIFY(VERIFY_TXT2IXML("en","IV.5.asd", "<OrderedListItem>IV.5.</OrderedListItem> asd"));
    QVERIFY(VERIFY_TXT2IXML("en","الف.5.asd", "<OrderedListItem>الف.5.</OrderedListItem> asd"));

    //sum
    QVERIFY(VERIFY_TXT2IXML("en","a 12.5. asd", "a <SpecialNumber>12.5.</SpecialNumber> asd"));
    QVERIFY(VERIFY_TXT2IXML("en","b A.5. asd",  "b <Abbreviation>A.5.</Abbreviation> asd"));

    //url
    QVERIFY(VERIFY_TXT2IXML("en","Amazon.com", "<URL>Amazon.com</URL>"));
    QVERIFY(VERIFY_TXT2IXML("en","1.Amazon.com", "<OrderedListItem>1.</OrderedListItem> <URL>Amazon.com</URL>"));
    QVERIFY(VERIFY_TXT2IXML("fa","آمازون.کام", "<URL>آمازون.کام</URL>"));

    QVERIFY(VERIFY_TXT2IXML("en","Resources and Irrigation Dr.MMahmoud Abu-Zaid.", "Resources and Irrigation <Abbreviation>Dr.</Abbreviation> MMahmoud Abu - Zaid ."));

    //suffix
    QVERIFY(VERIFY_TXT2IXML("en","I'm it's balls' ", "I 'm it 's balls '"));

    //abbr
    QVERIFY(VERIFY_TXT2IXML("en","a U.S. A.B.C.D A.B.C.D. ", "a <Abbreviation>U.S.</Abbreviation> <Abbreviation>A.B.C.D</Abbreviation> <Abbreviation>A.B.C.D.</Abbreviation>"));
    //  /QVERIFY(E4SMT_VERIFY("آ.ام.پی", "<Abbreviation>آ.ام.پی</Abbreviation>"));

    //separator
    QVERIFY(VERIFY_TXT2IXML("en","thisسلام", "this سلام"));
    QVERIFY(VERIFY_TXT2IXML("en","thisسلام", "this سلام"));
    QVERIFY(VERIFY_TXT2IXML("fa","با سویه H1N1رخ داد","با سویه H1N1 رخ داد"));
    //QVERIFY(E4SMT_VERIFY("fa","دکتر محمد الغمراوی رئیس GAIFZ‌می‌گوید","دکتر محمد الغمراوی رئیس GAIFZ‌ می‌گوید"));
    QVERIFY(VERIFY_TXT2IXML("fa"," پس تفاوت ایجادشده به حدود LE1159میلیون رسید","پس تفاوت ایجادشده به حدود LE1159 میلیون رسید"));
    //QVERIFY(E4SMT_VERIFY("fa","اروپایی UNITA‌برعهده خواهد داشت","اروپایی UNITA‌ برعهده خواهد داشت"));
    //QVERIFY(E4SMT_VERIFY("fa","افزود که او به تمامی مقامات SABIC‌اطلاع داده","افزود که او به تمامی مقامات SABIC‌ اطلاع داده"));

    //complex
    QVERIFY(VERIFY_TXT2IXML("fa","a S.A. اخبار الیوم، 1380/2/1","a <Abbreviation>S.A.</Abbreviation> اخبار الیوم , <Number>1380</Number> / <Number>2</Number> / <Number>1</Number>"));
    QVERIFY(VERIFY_TXT2IXML("en","(Is this a vulnerability?)", "( Is this a vulnerability ? )"));
    QVERIFY(VERIFY_TXT2IXML("en","(Is<this>a (vulnerability)?)", "( Is &lt; this &gt; a ( vulnerability ) ? )"));
    QVERIFY(VERIFY_TXT2IXML("en","* Rawhi al-Mushtaha:Senior&lt;/url&gt; Hamas leader.","* Rawhi al - Mushtaha : Senior &lt; / url &gt; Hamas leader ."));


    //virastyar

    QVERIFY(VERIFY_TXT2IXML("fa",
                            "",
                            ""
                            ));

//    QVERIFY(VERIFY_TXT2IXML("fa",
//                            "-"
//                            "استفاده از"
//                            "Wi-Fi."
//                            "هر روز",
//                            "<OrderedListItem>-</OrderedListItem>"
//                            "استفاده از"
//                            "Wi-Fi . "
//                            "هر روز"));

//    QVERIFY(VERIFY_TXT2IXML("fa",
//                            "* ایران",
//                            "<OrderedListItem>*</OrderedListItem> ایران"));
//    QVERIFY(VERIFY_TXT2IXML("en",
//                            "__kook@bbc.co.uk",
//                            "_ _ <Email>kook@bbc.co.uk</Email>"));
//    QVERIFY(VERIFY_TXT2IXML("en",
//                            "__http://bit.ly/BBCKookFB",
//                            "_ _ <URL>http://bit.ly/BBCKookFB</URL>"));
//    QVERIFY(VERIFY_TXT2IXML("fa",
//                            "o صفحات_ویژه",
//                            "<OrderedListItem>o</OrderedListItem> صفحات _ ویژه"));

    QVERIFY(VERIFY_TXT2IXML("fa",
                            "می گفت \"پاتک\"",
                            "می‌گفت \" پاتک \""));

//    QVERIFY(VERIFY_TXT2IXML("fa",
//                            "کرد.مشهورترین",
//                            "کرد . مشهورترین"));
}

