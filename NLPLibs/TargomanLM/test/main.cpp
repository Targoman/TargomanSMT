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

#include <QDebug>
#include <QStringList>

#include "libTargomanCommon/CmdIO.h"
#include "libTargomanLM/clsLanguageModel.h"
#include "libTargomanLM/clsLMSentenceScorer.h"
#include "libTargomanCommon/Logger.h"

#include <fstream>
#include <ctime>

using namespace Targoman::NLPLibs::TargomanLM;

int main(int argc, char *argv[])
{
    Q_UNUSED(argc)
    Q_UNUSED(argv)

    clsLanguageModel LM;
    try {
        Targoman::Common::TARGOMAN_IO_SETTINGS.setDefault();
        Targoman::Common::Logger::instance().setVisible();
        Targoman::Common::Logger::instance().setActive();

        Targoman::Common::LogP_t UnkProb = 0;
        Targoman::Common::LogP_t UnkBackoff = 0;
        bool UseIdexBasedModel = true;

        stuLMConfigs languageModelConfig( UnkProb, UnkBackoff, UseIdexBasedModel);


//        Word = "این";
//        Targoman::Common::LogP_t Prob = SS.wordProb(Word, Gram);
//        QVERIFY(Gram == 2);
//        QVERIFY(qFuzzyCompare(Prob, -1.97194f));

//        Word = "استخوان‌ها";
//        Prob = SS.wordProb(Word, Gram);

//        qDebug()<<"Order = "<<LM.init(argc > 1 ? argv[1] : "/home/user/SVN/Targoman/targoman/Trunk/Example/Model-fa2en.full//lm.4g.en", languageModelConfig);
//        LM.convertBinary("/home/user/SVN/Targoman/targoman/Trunk/Example/Model-fa2en.full//lm.4g.en.bin");
        LM.init("/home/user/SVN/Targoman/targoman/Trunk/Example/Model-fa2en.full//lm.4g.en.bin", languageModelConfig);


        QString Sentence = QStringLiteral("the reactor required for it produces atom plutonium bomb .");
        clsLMSentenceScorer SS1(LM);
        qDebug()<<Sentence;
        quint8 Gram;
        float  SumLM = 0;
        foreach (const QString& Word, Sentence.split(" ")){
            Targoman::Common::LogP_t Prob = SS1.wordProb(Word, Gram);
            qDebug()<<"Prob ["<<Word<<"]:Prob = "<<Prob<<" NGram = "<<Gram;
            SumLM-=Prob;
        }
        qDebug()<<"Sum:"<<SumLM;
        qDebug()<<"Sum after finalize:"<<SumLM - SS1.endOfSentenceProb(Gram);
        qDebug()<<"Sum after finalizeScaled:"<<(SumLM - SS1.endOfSentenceProb(Gram)) *0.15442631832099 ;

        Sentence = QStringLiteral("the reactor required for it produces . atom plutonium bomb");
        clsLMSentenceScorer SS2(LM);

        qDebug()<<Sentence;
        SumLM = 0;
        foreach (const QString& Word, Sentence.split(" ")){
            Targoman::Common::LogP_t Prob = SS2.wordProb(Word, Gram);
            qDebug()<<"Prob ["<<Word<<"]:Prob = "<<Prob<<" NGram = "<<Gram;
            SumLM-=Prob;
        }
        qDebug()<<"Sum:"<<SumLM;
        qDebug()<<"Sum after finalize:"<<SumLM - SS2.endOfSentenceProb(Gram);
        qDebug()<<"Sum after finalizeScaled:"<<(SumLM - SS2.endOfSentenceProb(Gram)) *0.15442631832099 ;





exit(0);
        clsLanguageModel LM2;
        LM2.init("/home/user/SVN/Targoman/targoman/Trunk/Example/Model-fa2en//lm.4g.en.bin", languageModelConfig);
        clsLMSentenceScorer SS(LM2);
//        clsLMSentenceScorer SS(LM);

//        QString Word = "این";
//        quint8 Gram;
//        Targoman::Common::LogP_t Prob = SS.wordProb(Word, Gram);
//        QVERIFY(Gram == 2);
//        QVERIFY(qFuzzyCompare(Prob, -1.97194f));

//        Word = "استخوان‌ها";
//        Prob = SS.wordProb(Word, Gram);

      /*  QString Sentence =
                QStringLiteral("این استخوان‌ها شامل یک استخوان فک بالا و دو استخوان فک پایین با دندان‌های سالم , بخشی از استخوان انگشت پا و استخوان‌های سالم انگشت دست است .");
*/
        //quint8 Gram;
        foreach (const QString& Word, Sentence.split(" ")){
            Targoman::Common::LogP_t Prob = SS.wordProb(Word, Gram);
            qDebug()<<"Prob ["<<Word<<"]:Prob = "<<Prob<<" NGram = "<<Gram;
        }
//        while(1);

        return 0;
        std::ifstream File;
        File.open("./test");
        std::string LineString;
        QList<float> Times;
        if (File.is_open()){
            while (std::getline(File, LineString)) {
                quint8 Gram;
                SS.reset();
                foreach (const QString& Word, QString::fromStdString(LineString).split(" ", QString::SkipEmptyParts)){
                    clock_t start = std::clock();
                    Targoman::Common::LogP_t Prob = SS.wordProb(Word, Gram);
                    Times.append((std::clock() - start)/(float)CLOCKS_PER_SEC);

                    qDebug()<<Times.last()<<"Prob ["<<Word<<"]:Prob = "<<Prob<<" NGram = "<<Gram;
                }
                qDebug()<<"###########################33";
            }
            float SumTime = 0;
            foreach (float Time, Times)
                SumTime += Time;
            qDebug()<<"Average = "<<SumTime / (float)Times.size();
        }
    }
    catch(Targoman::Common::exTargomanBase& e) {
        qDebug() << e.what();
    }
}


