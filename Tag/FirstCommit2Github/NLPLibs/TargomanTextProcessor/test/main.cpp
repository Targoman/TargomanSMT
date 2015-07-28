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
        QString ActorUUID;

        qDebug()<<"sdklfdsklfjldsjk";
        TARGOMAN_REGISTER_ACTOR("testLibCommon");
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

/*
        QString inputDir = "/home/saeed/Desktop/temp";
//        QString inputDir = "/home/saeed/Desktop/FLexicon/categoriesOfEntries";

        QDirIterator it(inputDir, QStringList() << "*.txt",QDir::Files);
        while (it.hasNext()) {
            QString filePath = it.next();
//            QString filePath = "/home/saeed/Desktop/FLexicon/categoriesOfEntries/N1.one";
            QFile inputFile(filePath);
            qDebug() << "file: " + filePath;

            if (inputFile.open(QIODevice::ReadOnly))
            {
                QTextStream in(&inputFile);
                in.setCodec("UTF-8");
                QFile outputFile(filePath + ".ixml");
                QTextStream out(&outputFile);
                out.setCodec("UTF-8");
                outputFile.open(QIODevice::WriteOnly);

                while ( !in.atEnd() )
                {
                    QString line = in.readLine();
                    QString normalizedLine = Targoman::NLPLibs::TargomanTP::instance().text2IXML(line, "fa");
//                            normalizeText(line, false, "fa");
//                            text2IXML(line, "fa");
                    out << normalizedLine << "\n";
                }
                inputFile.close();
                outputFile.close();
            }
        }
        */


////        Targoman::NLPLibs::Private::Normalizer::instance().init("/tmp/Normalization.bin", true);
////          qDebug()<<Targoman::NLPLibs::TargomanTP::instance().normalizeText(
////                    QStringLiteral("نا مردترینشان"), false, "fa");
////        qDebug()<<Targoman::NLPLibs::TargomanTP::instance().normalizeText(
////                    QStringLiteral("من با دم خود می گفتم که با معرفت ترین ها یشان هم نا رفیق بوده اند"), false, "fa");


        qDebug()<<Targoman::NLPLibs::TargomanTP::Private::Normalizer::instance().normalize(
                    QStringLiteral("آذربایجان‌  شرقی"), false, "fa");
////        qDebug()<<Targoman::NLPLibs::TargomanTP::instance().normalizeText(
////                    QStringLiteral("ﺍﺟـﺘـﻤﺎﻋـﯽ ـ ﺳﻴﺎﺳـﯽ"), false, "fa");
        qDebug()<<Targoman::NLPLibs::TargomanTP::Private::Normalizer::instance().normalize(
                    QStringLiteral("بیمعرفتها"), false, "fa");
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


        qDebug()<<Targoman::NLPLibs::TargomanTextProcessor::instance().normalizeText(
                    QStringLiteral("آیا اصلاح یا تغییر خط کاری است که صرفا\" دولت می تواند انجام دهد؟"), false, "fa");

//        qDebug() << "می گفت \"پاتک\"";
//        qDebug()<<Targoman::NLPLibs::TargomanTextProcessor::instance().normalizeText(
//                    QStringLiteral("می ‌برد و"), false, "fa");

//        qDebug() << Targoman::NLPLibs::TargomanTP::instance().text2IXML(QStringLiteral("ماههای"), "fa", 0, false, true);
////        qDebug() << Targoman::NLPLibs::TargomanTP::instance().text2IXML(QStringLiteral("Resources and Irrigation Dr.MMahmoud Abu-Zaid."), "en");
////        qDebug() << Targoman::NLPLibs::TargomanTP::instance().text2IXML(QStringLiteral("Resources and Irrigation Dr.MMahmoud Abu-Zaid."), "en", 0, false);


////        qDebug()<<Targoman::NLPLibs::TargomanTP::instance().text2IXML(

////                    QStringLiteral(" "), "en", 0, true, true);

////                    QStringLiteral("a -12asd"), "en", 0, true, true);/**/


//        /*
//        while(1);

//        QFile SampleFile("/tmp/persian.txt");
//        QTextStream Stream(&SampleFile);
//        Stream.setCodec("UTF-8");
//        SampleFile.open(QFile::ReadOnly);

//        if (SampleFile.isReadable() == false)
//            throw exSpellCorrector("Unable to open" + SampleFile.fileName());

//        quint32 Line=0;
//        while (!Stream.atEnd()){
//            Line++;
//            TargomanDebug(1,"*******************************************************************************")
//            QString Line = Stream.readLine();
//            TargomanDebug(1, "[ORG]\n"<<Line);
//            Line = Targoman::NLPLibs::TargomanTP::Private::Normalizer::instance().normalize(Line);
//            TargomanDebug(1, "[NRM]\n"<<Line);
//            Line = Targoman::NLPLibs::TargomanTP::Private::SpellCorrector::instance().process("fa",Line,false);
//            TargomanDebug(1, "[FNL]\n"<<Line);
//        }
//        */

//        /**/
///*

//        Targoman::NLPLibs::Private::Normalizer::instance().init("../conf/Normalization.conf");

//        Targoman::NLPLibs::Private::Normalizer::instance().updateBinTable("/tmp/Normalization.bin");


//        Targoman::NLPLibs::Private::Normalizer::instance().init("/tmp/Normalization.bin",true);

//        qDebug()<<QStringLiteral("ｐ = P? :")<<Targoman::NLPLibs::Private::Normalizer::instance().normalize(QStringLiteral("ｐ"));

///**/
///*        for (int i=0x27B1; i<=0xFFFF; i++){
//            QChar C = QChar(i);
//            QString Normalized = Targoman::NLPLibs::Private::Normalizer::instance().normalize(C,QChar(),true,0,"",0);

//            std::cerr<<QString("<0x%1>\t{%2}\t[%3]\t[%4]\t==>\t%6").arg(
//                           QString::number(C.unicode(),16).toAscii().toUpper().constData()).arg(
//                           ((C.isLetterOrNumber() || C.toAscii() == C)) ?
//                               QString(C) : "N/A").arg(
//                           QCHAR_UNICOE_CATEGORIES[C.category()]).arg(
//                        enuUnicodeCharScripts::toStr(
//                            (enuUnicodeCharScripts::Type)
//                            QUnicodeTables::script(C.unicode()))).arg(
//                        Normalized.isEmpty() ?
//                            "REMOVED" :
//                            ((Normalized == C) ?
//                                 "INTACT" :
//                                 QString("{%1}").arg(Normalized))).toUtf8().constData()<<std::endl;
//        }/**/

    }catch(Targoman::Common::exTargomanBase &e){
        qDebug()<<e.what();
    }
    return 0;
}


