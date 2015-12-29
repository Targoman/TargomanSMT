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

#include "libTargomanTextProcessor/Private/Normalizer.h"
#include "libTargomanTextProcessor/Private/Unicode.hpp"
#include "libTargomanTextProcessor/Private/SpellCorrector.h"
using namespace Targoman::NLPLibs::TargomanTP::Private;
#include "libTargomanCommon/Logger.h"
using namespace Targoman::Common;

#include <QChar>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QString>
#include <iostream>
#include <QDir>
#include <QDirIterator>

using  namespace Targoman::NLPLibs;

int main(int _argc, char *_argv[])
{
    Q_UNUSED(_argc)
    Q_UNUSED(_argv)

   try{
        qDebug()<<"sdklfdsklfjldsjk";
        Targoman::Common::Logger::instance().init("log.log");

        //Change below in order to silent
//        Targoman::Common::TARGOMAN_IO_SETTINGS.setFull();
        Targoman::Common::TARGOMAN_IO_SETTINGS.setSilent();
        Targoman::Common::TARGOMAN_IO_SETTINGS.Debug.setDetails(true);

        Targoman::NLPLibs::TargomanTextProcessor::stuConfigs Configs;
        Configs.NormalizationFile = "../conf/Normalization.conf";
        Configs.AbbreviationsFile = "../conf/Abbreviations.tbl";
        Configs.SpellCorrectorBaseConfigPath = "../conf/SpellCorrectors";
        QVariantHash PersianSpellCorrector;
        PersianSpellCorrector.insert("Active", true);
        Configs.SpellCorrectorLanguageBasedConfigs.insert("fa", PersianSpellCorrector);
        TargomanTextProcessor::instance().init(Configs);

        QStringList Tests = {"this is just  a test.",
                             "A simple \"Test\" for you.",
                             "A simple 'Test' for you.",
                             "با سویه H1N1رخ داد",
                             "-12.5 -13 17,254.25",
                             " \"asd12\"",
                             "سلام12",
                             "12asd13",
                             "asd-12",
                             "-12asd",
                             "a asd12",
                             "a سلام12",
                             "a 12asd13",
                             "a asd-12",
                             "a -12asd",
                             "17/11/2001",
                             " یا49راتحقق میبخشد",
                             "1st",
                             "the 2nd",
                             "a -20th",
                             "a 1380/2/1 b",
                             "1980 jun 1",
                             "12.5.asd",
                             "A.5.asd",
                             "IV.5.asd",
                             "الف.5.asd",
                             "a 12.5. asd",
                             "b A.5. asd",
                             "Amazon.com",
                             "1.Amazon.com",
                             "آمازون.کام",
                             "Resources and Irrigation Dr.MMahmoud Abu-Zaid.",
                             "I'm it's balls' ",
                             "a U.S. A.B.C.D A.B.C.D. ",
                             "thisسلام",
                             "با سویه H1N1رخ داد",
                             "a S.A. اخبار الیوم، 1380/2/1",
                             "(Is this a vulnerability?)",
                             "(Is<this>a (vulnerability)?)",
                             "* Rawhi al-Mushtaha:Senior&lt;/url&gt; Hamas leader.",
                             "میدهند",
                             "اویزه",
                             "غیبگو تر"};

        for(QString TestLine : Tests)
        {
            bool SpellCorrected;
            QString OutputLine = /*TestLine + QString(" | ") +*/ Targoman::NLPLibs::TargomanTextProcessor::instance().text2IXML(TestLine, SpellCorrected, "fa", 0, false);
            std::cout << OutputLine.toUtf8().constData() << std::endl;
        }

//        QString normalizedLine = Targoman::NLPLibs::TargomanTP::instance().text2IXML(line, "fa");


////        Targoman::NLPLibs::Private::Normalizer::instance().init("/tmp/Normalization.bin", true);
////          qDebug()<<Targoman::NLPLibs::TargomanTP::instance().normalizeText(
////                    QStringLiteral("نا مردترینشان"), false, "fa");
////        qDebug()<<Targoman::NLPLibs::TargomanTP::instance().normalizeText(
////                    QStringLiteral("من با دم خود می گفتم که با معرفت ترین ها یشان هم نا رفیق بوده اند"), false, "fa");


//        qDebug()<<Targoman::NLPLibs::TargomanTP::Private::Normalizer::instance().normalize(
//                    QStringLiteral("آذربایجان‌  شرقی"), false, "fa");
////        qDebug()<<Targoman::NLPLibs::TargomanTP::instance().normalizeText(
////                    QStringLiteral("ﺍﺟـﺘـﻤﺎﻋـﯽ ـ ﺳﻴﺎﺳـﯽ"), false, "fa");
//        qDebug()<<Targoman::NLPLibs::TargomanTP::Private::Normalizer::instance().normalize(
//                    QStringLiteral("بیمعرفتها"), false, "fa");
////        qDebug()<<Targoman::NLPLibs::TargomanTP::instance().normalizeText(
////                    QStringLiteral("میرفته‌‌ام"), false, "fa");
////        qDebug()<<Targoman::NLPLibs::TargomanTP::instance().normalizeText(
////                    QStringLiteral("نا مردهایشان"), false, "fa");

////        qDebug()<<Targoman::NLPLibs::TargomanTP::instance().normalizeText(
////                    QStringLiteral("\"کوری\""), false, "");
////        qDebug() << Targoman::NLPLibs::TargomanTP::instance().text2IXML(QStringLiteral("123. saeed tor234 الف."), "en");
//            qDebug() << Targoman::NLPLibs::TargomanTP::instance().text2IXML(QStringLiteral("می رود..."), "fa");
////        qDebug()<<Targoman::NLPLibs::TargomanTP::instance().normalizeText(
////                    QStringLiteral("میخورم"), false, "fa");


////        qDebug()<<Targoman::NLPLibs::TargomanTP::instance().normalizeText(
////                    QStringLiteral("غیبگو ترین ها یشان"), false, "fa");


//        qDebug()<<Targoman::NLPLibs::TargomanTextProcessor::instance().normalizeText(
//                    QStringLiteral("آیا اصلاح یا تغییر خط کاری است که صرفا\" دولت می تواند انجام دهد؟"), false, "fa");


    }catch(Targoman::Common::exTargomanBase &e){
        qDebug()<<e.what();
        return 1;
    }
    return 0;
}


