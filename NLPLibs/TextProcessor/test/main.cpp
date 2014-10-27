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

//#include "libLIB_NAME/???.h"

#include "libTargomanTextProcessor/Private/Normalizer.h"
#include "libTargomanTextProcessor/Private/Unicode.hpp"
#include "libTargomanTextProcessor/Private/SpellCorrector.h"
using namespace Targoman::NLPLibs::Private;
#include "libTargomanCommon/Logger.h"
using namespace Targoman::Common;

#include <QChar>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QString>
#include <iostream>



int main(int _argc, char *_argv[])
{
    Q_UNUSED(_argc)
    Q_UNUSED(_argv)

   try{
        QString ActorUUID;

        TARGOMAN_REGISTER_ACTOR("testLibCommon");
        OUTPUT_SETTINGS_DEBUG.set(10,true);
        OUTPUT_SETTINGS_ERROR.set(10,true);
        OUTPUT_SETTINGS_WARNING.set(10,true);
        OUTPUT_SETTINGS_INFO.set(10,true);
        OUTPUT_SETTINGS_HAPPY.set(10,true);
        Targoman::Common::Logger::instance().init("log.log");
        //Targoman::Common::OUTPUT_SETTINGS_SHOWCOLORED = true;

/*        QRegExp RxHa                  = QRegExp(QString::fromUtf8("ها(ی|یم|یت|یش|یمان|یتان|یشان)?"));
        QRegExp RxEndWithHa           = QRegExp(".*" + RxHa.pattern() + "$");

        TargomanDebug(1,"\n"<<RxHa.pattern()<<"\n"<<RxEndWithHa.pattern());
        QString Str = QString::fromUtf8("ها");
        TargomanDebug(1,"\n"<<Str<<" "<<RxEndWithHa.exactMatch(Str));
        Str = QString::fromUtf8("های");
        TargomanDebug(1,"\n"<<Str<<" "<<RxEndWithHa.exactMatch(Str));
        Str = QString::fromUtf8("پاهایشان");
        TargomanDebug(1,"\n"<<Str<<" "<<RxEndWithHa.exactMatch(Str));
        Str = QString::fromUtf8("هان");
        TargomanDebug(1,"\n"<<Str<<" "<<RxEndWithHa.exactMatch(Str));
        exit(1);
*/
        Targoman::NLPLibs::Private::Normalizer::instance().init("/tmp/Normalization.bin", true);

         QHash<QString, QVariantHash> SpellConfig;
         QVariantHash PersianSpellCorrector;
         //PersianSpellCorrector.insert("Active", false);
         SpellConfig.insert("fa", PersianSpellCorrector);
         Targoman::NLPLibs::Private::SpellCorrector::instance().init("../conf/SpellCorrectors/",SpellConfig);


        QFile SampleFile("/tmp/persian.txt");
        QTextStream Stream(&SampleFile);
        Stream.setCodec("UTF-8");
        SampleFile.open(QFile::ReadOnly);

        if (SampleFile.isReadable() == false)
            throw exSpellCorrector("Unable to open" + SampleFile.fileName());

        quint32 Line=0;
        while (!Stream.atEnd()){
            Line++;
            TargomanDebug(1,"*******************************************************************************")
            QString Line = Stream.readLine();
            TargomanDebug(1, "[ORG]\n"<<Line);
            Line = Targoman::NLPLibs::Private::Normalizer::instance().normalize(Line);
           // TargomanDebug(1, "[NRM]\n"<<Line);
            Line = Targoman::NLPLibs::Private::SpellCorrector::instance().process("fa",Line,false);
            TargomanDebug(1, "[FNL]\n"<<Line);
        }
        /**/
/*

        Targoman::NLPLibs::Private::Normalizer::instance().init("../conf/Normalization.conf");

        Targoman::NLPLibs::Private::Normalizer::instance().updateBinTable("/tmp/Normalization.bin");


        Targoman::NLPLibs::Private::Normalizer::instance().init("/tmp/Normalization.bin",true);

        qDebug()<<QString::fromUtf8("ｐ = P? :")<<Targoman::NLPLibs::Private::Normalizer::instance().normalize(QString::fromUtf8("ｐ"));

/**/
/*        for (int i=0x27B1; i<=0xFFFF; i++){
            QChar C = QChar(i);
            QString Normalized = Targoman::NLPLibs::Private::Normalizer::instance().normalize(C,QChar(),true,0,"",0);

            std::cerr<<QString("<0x%1>\t{%2}\t[%3]\t[%4]\t==>\t%6").arg(
                           QString::number(C.unicode(),16).toAscii().toUpper().constData()).arg(
                           ((C.isLetterOrNumber() || C.toAscii() == C)) ?
                               QString(C) : "N/A").arg(
                           QCHAR_UNICOE_CATEGORIES[C.category()]).arg(
                        enuUnicodeCharScripts::toStr(
                            (enuUnicodeCharScripts::Type)
                            QUnicodeTables::script(C.unicode()))).arg(
                        Normalized.isEmpty() ?
                            "REMOVED" :
                            ((Normalized == C) ?
                                 "INTACT" :
                                 QString("{%1}").arg(Normalized))).toUtf8().constData()<<std::endl;
        }/**/
    }catch(Targoman::Common::exTargomanBase &e){
        qDebug()<<e.what();
    }
    return 0;
}


