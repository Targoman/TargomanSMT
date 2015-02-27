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
    QVERIFY(VERIFY_TXT2IXML("en","-12.5 -13 17,254.25", "<num>-12.5</num> <num>-13</num> <num>17,254.25</num>"));
    QVERIFY(VERIFY_TXT2IXML("en","asd12", "asd12"));
    QVERIFY(VERIFY_TXT2IXML("en","سلام12", "سلام <num>12</num>"));
    QVERIFY(VERIFY_TXT2IXML("en","12asd13", "<num>12</num> asd13"));
    QVERIFY(VERIFY_TXT2IXML("en","asd-12", "asd - <num>12</num>"));
    QVERIFY(VERIFY_TXT2IXML("en","-12asd", "<oli>-</oli> <num>12</num> asd"));

    QVERIFY(VERIFY_TXT2IXML("en","a asd12", "a asd12"));
    QVERIFY(VERIFY_TXT2IXML("en","a سلام12", "a سلام <num>12</num>"));
    QVERIFY(VERIFY_TXT2IXML("en","a 12asd13", "a <num>12</num> asd13"));
    QVERIFY(VERIFY_TXT2IXML("en","a asd-12", "a asd - <num>12</num>"));
    QVERIFY(VERIFY_TXT2IXML("en","a -12asd", "a - <num>12</num> asd"));
    QVERIFY(VERIFY_TXT2IXML("en","17/11/20001", "<num>17</num> / <num>11</num> / <num>20001</num>"));
//    QVERIFY(VERIFY_TXT2IXML("fa"," یا49راتحقق می‌بخشد", "یا <num>49</num> راتحقق می‌بخشد"));
    QVERIFY(VERIFY_TXT2IXML("fa","و 1.6155فرانک سوییس در مقابل 1.5960", "و <num>1.6155</num> فرانک سوییس در مقابل <num>1.5960</num>"));
    QVERIFY(VERIFY_TXT2IXML("en"," to_1967_lines", "to _ <num>1967</num> _ lines"));
//    QVERIFY(VERIFY_TXT2IXML("گفت.مریم", "گفت . مریم"));


    QVERIFY(VERIFY_TXT2IXML("en","1st", "<ord>1st</ord>"));
    QVERIFY(VERIFY_TXT2IXML("en","the 2nd", "the <ord>2nd</ord>"));
    QVERIFY(VERIFY_TXT2IXML("en","a -20th", "a - <ord>20th</ord>"));
    QVERIFY(VERIFY_TXT2IXML("en","a 1380/2/1 b", "a <num>1380</num> / <num>2</num> / <num>1</num> b"));
    QVERIFY(VERIFY_TXT2IXML("en","1380/2/1", "<num>1380</num> / <num>2</num> / <num>1</num>"));

    //Ordered lists
    QVERIFY(VERIFY_TXT2IXML("en","12.5.asd", "<oli>12.5.</oli> asd"));
    QVERIFY(VERIFY_TXT2IXML("en","A.5.asd", "<oli>A.5.</oli> asd"));
    QVERIFY(VERIFY_TXT2IXML("en","IV.5.asd", "<oli>IV.5.</oli> asd"));
    QVERIFY(VERIFY_TXT2IXML("en","الف.5.asd", "<oli>الف.5.</oli> asd"));

    //sum
    QVERIFY(VERIFY_TXT2IXML("en","a 12.5. asd", "a <snum>12.5.</snum> asd"));
    QVERIFY(VERIFY_TXT2IXML("en","b A.5. asd",  "b <abbr>A.5.</abbr> asd"));

    //url
    QVERIFY(VERIFY_TXT2IXML("en","Amazon.com", "<url>Amazon.com</url>"));
    QVERIFY(VERIFY_TXT2IXML("en","1.Amazon.com", "<oli>1.</oli> <url>Amazon.com</url>"));
    QVERIFY(VERIFY_TXT2IXML("fa","آمازون.کام", "<url>آمازون.کام</url>"));

    QVERIFY(VERIFY_TXT2IXML("en","Resources and Irrigation Dr.MMahmoud Abu-Zaid.", "Resources and Irrigation <abbr>Dr.</abbr> MMahmoud Abu - Zaid ."));

    //suffix
    QVERIFY(VERIFY_TXT2IXML("en","I'm it's balls' ", "I 'm it 's balls '"));

    //abbr
    QVERIFY(VERIFY_TXT2IXML("en","a U.S. A.B.C.D A.B.C.D. ", "a <abbr>U.S.</abbr> <abbr>A.B.C.D</abbr> <abbr>A.B.C.D.</abbr>"));
    //  /QVERIFY(E4SMT_VERIFY("آ.ام.پی", "<abbr>آ.ام.پی</abbr>"));

    //separator
    QVERIFY(VERIFY_TXT2IXML("en","thisسلام", "this سلام"));
    QVERIFY(VERIFY_TXT2IXML("en","thisسلام", "this سلام"));
    QVERIFY(VERIFY_TXT2IXML("fa","با سویه H1N1رخ داد","با سویه H1N1 رخ داد"));
    //QVERIFY(E4SMT_VERIFY("fa","دکتر محمد الغمراوی رئیس GAIFZ‌می‌گوید","دکتر محمد الغمراوی رئیس GAIFZ‌ می‌گوید"));
    QVERIFY(VERIFY_TXT2IXML("fa"," پس تفاوت ایجادشده به حدود LE1159میلیون رسید","پس تفاوت ایجادشده به حدود LE1159 میلیون رسید"));
    //QVERIFY(E4SMT_VERIFY("fa","اروپایی UNITA‌برعهده خواهد داشت","اروپایی UNITA‌ برعهده خواهد داشت"));
    //QVERIFY(E4SMT_VERIFY("fa","افزود که او به تمامی مقامات SABIC‌اطلاع داده","افزود که او به تمامی مقامات SABIC‌ اطلاع داده"));

    //complex
    QVERIFY(VERIFY_TXT2IXML("fa","a S.A. اخبار الیوم، 1380/2/1","a <abbr>S.A.</abbr> اخبار الیوم , <num>1380</num> / <num>2</num> / <num>1</num>"));
    QVERIFY(VERIFY_TXT2IXML("en","(Is this a vulnerability?)", "( Is this a vulnerability ? )"));
    QVERIFY(VERIFY_TXT2IXML("en","(Is<this>a (vulnerability)?)", "( Is &lt; this &gt; a ( vulnerability ) ? )"));
    QVERIFY(VERIFY_TXT2IXML("en","* Rawhi al-Mushtaha:Senior&lt;/url&gt; Hamas leader.","* Rawhi al - Mushtaha : Senior &lt; / url &gt; Hamas leader ."));


    //virastyar

    QVERIFY(VERIFY_TXT2IXML("fa",
                            "",
                            ""
                            ));

    QVERIFY(VERIFY_TXT2IXML("fa",
                            "-"
                            "استفاده از"
                            "Wi-Fi."
                            "هر روز",
                            "<oli>-</oli>"
                            "استفاده از"
                            "Wi-Fi . "
                            "هر روز"));

    QVERIFY(VERIFY_TXT2IXML("fa",
                            "* ایران",
                            "<oli>*</oli> ایران"));
    QVERIFY(VERIFY_TXT2IXML("en",
                            "__kook@bbc.co.uk",
                            "_ _ <email>kook@bbc.co.uk</email>"));
    QVERIFY(VERIFY_TXT2IXML("en",
                            "__http://bit.ly/BBCKookFB",
                            "_ _ <url>http://bit.ly/BBCKookFB</url>"));
    QVERIFY(VERIFY_TXT2IXML("fa",
                            "o صفحات_ویژه",
                            "<oli>o</oli> صفحات _ ویژه"));

    QVERIFY(VERIFY_TXT2IXML("fa",
                            "می گفت \"پاتک\"",
                            "می‌گفت \" پاتک \""));

    QVERIFY(VERIFY_TXT2IXML("fa",
                            "کرد.مشهورترین",
                            "کرد . مشهورترین"));
}

