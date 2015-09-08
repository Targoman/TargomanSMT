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
        Targoman::Common::TARGOMAN_IO_SETTINGS.setSilent();
        Targoman::Common::Logger::instance().setVisible();
        Targoman::Common::Logger::instance().setActive();

        Targoman::Common::LogP_t UnkProb = 0;
        Targoman::Common::LogP_t UnkBackoff = 0;
        bool UseIdexBasedModel = true;

        stuLMConfigs languageModelConfig( UnkProb, UnkBackoff, UseIdexBasedModel);
        languageModelConfig.VerifyBinaryCheckSum = true;

        QStringList EnglishSentences = {
            "Izzet Ibrahim Meets Saudi Trade Official in Baghdad ",
            "Baghdad 1-1 ( AFP - Iraq's official news agency reported that the Deputy Chairman of the Iraqi Revolutionary Command Council , Izzet Ibrahim , today met with Abdul Rahman al-Zamil , Managing Director of the Saudi Center for Export Development . ",
            "The agency said Ibrahim welcomed this occasion for trade exchange and cooperation between Iraq and Saudi Arabia . ",
            "The agency also reported that the Iraqi Minister of Trade , Mohamed Mehdi Salih , took part in the meeting . ",
            "Baghdad and Riyadh , who broke diplomatic relations during the Gulf War in 1991 , began to improve their relations over the course of the Beirut Summit last March . "
        };

        clsLanguageModel* ALM = new clsLanguageModel();

        ALM->init("/Share/local/torabzadeh/Targoman/lm/Final.en.arpa", languageModelConfig);

        for(int i = 0; i < EnglishSentences.size(); ++i) {
            QString& Sentence = EnglishSentences[i];
            clsLMSentenceScorer SS(*ALM);
            qDebug() << Sentence;
            quint8 Gram;
            float SumLM = 0;
            int TokenCount = 1;
            foreach (const QString& Word, Sentence.split(" ")){
                Targoman::Common::LogP_t Prob = SS.wordProb(Word, Gram);
                qDebug()<<"Prob [" << Word << "]:Prob = " << Prob << " NGram = " << Gram;
                SumLM-=Prob;
                ++TokenCount;
            }
            ++TokenCount;
            qDebug()<<"Sum:" << SumLM;
            SumLM -=  SS.endOfSentenceProb(Gram);
            qDebug()<<"Sum after finalize:"<< SumLM;
            qDebug()<<"Perplexity:" << pow(2, SumLM / TokenCount);
        }

        QStringList PersianSentences = {
            "ایزت ابراهیم مامور تجاری عبرستانی را در بغداد ملاقات کرد ",
            "بغداد 1-1 ( AFP اژانس خبری رسمی عراق گزارش داد که امروز نایب رییس هییت فرماندهی انقلابی عراق با عبدالرحمان الزملی مدیر عامل مرکز صعودی برای توسعه ی صادرات ملاقات کرد . ",
            "این اژانس گزارش داد که ابراهیم از این موقعیت برای مبادله ی تجاری و همکاری بین عراق و عربستان صعودی استقبال کرد . ",
            "این اژانس همچنین گزارش داد که مهدی صالح در این جلسه شرکت کرده است ",
            "بغداد و ریاض که هنگام جنگ خلیج در سال 1991 روابط دیپلماتیک خود را با هم قطع کرده بودند , بهبود روابط خود را در جریان جلسه ی بیروت در مارچ گذشته اغاز کردند . "
        };

        delete ALM;
        ALM = new clsLanguageModel();

        ALM->init("/Share/local/torabzadeh/Targoman/lm/Final.fa.arpa", languageModelConfig);

        for(int i = 0; i < PersianSentences.size(); ++i) {
            QString& Sentence = PersianSentences[i];
            clsLMSentenceScorer SS(*ALM);
            qDebug() << Sentence;
            quint8 Gram;
            float SumLM = 0;
            int TokenCount = 1;
            foreach (const QString& Word, Sentence.split(" ")){
                Targoman::Common::LogP_t Prob = SS.wordProb(Word, Gram);
                qDebug()<<"Prob [" << Word << "]:Prob = " << Prob << " NGram = " << Gram;
                SumLM-=Prob;
                ++TokenCount;
            }
            ++TokenCount;
            qDebug()<<"Sum:" << SumLM;
            SumLM -=  SS.endOfSentenceProb(Gram);
            qDebug()<<"Sum after finalize:"<< SumLM;
            qDebug()<<"Perplexity:" << pow(2, SumLM / TokenCount);
        }

        exit(0);
//        Word = "این";
//        Targoman::Common::LogP_t Prob = SS.wordProb(Word, Gram);
//        QVERIFY(Gram == 2);
//        QVERIFY(qFuzzyCompare(Prob, -1.97194f));

//        Word = "استخوان‌ها";
//        Prob = SS.wordProb(Word, Gram);

//        qDebug()<<"Order = "<<LM.init(argc > 2 ? argv[1] : "/Share/local/vedadian/Experiments/Targoman/lm/Fa-En_BaseLine_2014-Mar-5_v0.1.4g.arpa", languageModelConfig);
//        LM.convertBinary("/Share/local/vedadian/Experiments/Targoman/lm/Fa-En_BaseLine_2014-Mar-5_v0.1.4g.bin");
        LM.init("/Share/local/torabzadeh/Targoman/lm/Final.fa.arpa", languageModelConfig);

//        QString Sentence = QStringLiteral("the reactor required for it produces atom plutonium bomb .");
        QString Sentence = QStringLiteral("iiiiiiiiiiiiiiiiiiii King");
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

        exit(0);

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
        return 1;
    }
    return 0;
}


