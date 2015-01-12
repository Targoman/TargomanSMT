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
using namespace Targoman::NLPLibs::TextProcessor_::Private;
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
        Targoman::Common::Logger::instance().init("log.log");

        //Change below in order to silent
        Targoman::Common::TARGOMAN_IO_SETTINGS.Debug.setLevel(8);
        Targoman::Common::TARGOMAN_IO_SETTINGS.Debug.setDetails(true);

        Targoman::NLPLibs::TextProcessor::stuConfigs Configs;
        Configs.NormalizationFile = "../conf/Normalization.conf";
        Configs.AbbreviationsFile = "../conf/Normalization.conf";
        Configs.SpellCorrectorBaseConfigPath = "../conf/SpellCorrectors";
        QVariantHash PersianSpellCorrector;
        PersianSpellCorrector.insert("Active", true);
        Configs.SpellCorrectorLanguageBasedConfigs.insert("fa", PersianSpellCorrector);
        Targoman::NLPLibs::TextProcessor::instance().init(Configs);



//        Targoman::NLPLibs::Private::Normalizer::instance().init("/tmp/Normalization.bin", true);

      //  qDebug()<<Targoman::NLPLibs::TextProcessor::instance().normalizeText(
      //              QStringLiteral("من با دم خود می گفتم که با معرفت ترین ها یشان هم نا رفیق بوده اند"), false, "fa");
        qDebug()<<Targoman::NLPLibs::TextProcessor::instance().text2IXML(
                    QStringLiteral("a -12asd"), "en", 0, true, true);

        while(1);

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
            Line = Targoman::NLPLibs::TextProcessor_::Private::Normalizer::instance().normalize(Line);
            TargomanDebug(1, "[NRM]\n"<<Line);
            Line = Targoman::NLPLibs::TextProcessor_::Private::SpellCorrector::instance().process("fa",Line,false);
            TargomanDebug(1, "[FNL]\n"<<Line);
        }
        /**/
/*

        Targoman::NLPLibs::Private::Normalizer::instance().init("../conf/Normalization.conf");

        Targoman::NLPLibs::Private::Normalizer::instance().updateBinTable("/tmp/Normalization.bin");


        Targoman::NLPLibs::Private::Normalizer::instance().init("/tmp/Normalization.bin",true);

        qDebug()<<QStringLiteral("ｐ = P? :")<<Targoman::NLPLibs::Private::Normalizer::instance().normalize(QStringLiteral("ｐ"));

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
        while(1);
    }catch(Targoman::Common::exTargomanBase &e){
        qDebug()<<e.what();
    }
    return 0;
}


