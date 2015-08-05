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
 * @author Saeed Torabzadeh <saeed.torabzadeh@targoman.com>
 */

#include "UnitTest.h"

using namespace Targoman::NLPLibs::TargomanLM;

void UnitTest::testIndexBased()
{

    clsLanguageModel LM;
    Targoman::Common::LogP_t UnkProb = 0;
    Targoman::Common::LogP_t UnkBackoff = 0;
    bool UseIdexBasedModel = true;

    stuLMConfigs languageModelConfig( UnkProb, UnkBackoff, UseIdexBasedModel);

    QDir ApplicationDir(QCoreApplication::applicationDirPath());
    QString AbsoluteFilePath = ApplicationDir.absoluteFilePath("TargomanLM_assets/testLM.arpa");
    quint8 order = LM.init(AbsoluteFilePath, languageModelConfig);
    QVERIFY(order == 4);
    clsLMSentenceScorer SS(LM);
    QString Word;
    quint8 Gram;

    Word = "این";
    Targoman::Common::LogP_t Prob = SS.wordProb(Word, Gram);
    QVERIFY(Gram == 2);
    QVERIFY(qFuzzyCompare(Prob, -1.97194f));

    Word = "استخوان‌ها";
    Prob = SS.wordProb(Word, Gram);
    QVERIFY(Gram == 1);
    QVERIFY(Prob == -FLT_MAX);

    Word = "شامل";
    Prob = SS.wordProb(Word, Gram);
    QVERIFY(Gram == 1);
    QVERIFY(Prob == -FLT_MAX);


    Word = "یک";
    Prob = SS.wordProb(Word, Gram);
    QVERIFY(Gram == 1);
    QVERIFY(qFuzzyCompare(Prob, -2.72819f));

    Word = "استخوان";
    Prob = SS.wordProb(Word, Gram);
    QVERIFY(Gram == 1);
    QVERIFY(Prob == -FLT_MAX);


    Word = "فک";
    Prob = SS.wordProb(Word, Gram);
    QVERIFY(Gram == 1);
    QVERIFY(Prob == -FLT_MAX);

    Word = "بالا";
    Prob = SS.wordProb(Word, Gram);
    QVERIFY(Gram == 1);
    QVERIFY(Prob == -FLT_MAX);


    Word = "و";
    Prob = SS.wordProb(Word, Gram);
    QVERIFY(Gram == 1);
    QVERIFY(qFuzzyCompare(Prob, -1.36646f));

    Word = "دو";
    Prob = SS.wordProb(Word, Gram);
    QVERIFY(Gram == 1);
    QVERIFY(qFuzzyCompare(Prob, -3.41515f));

    Word = "استخوان";
    Prob = SS.wordProb(Word, Gram);
    QVERIFY(Gram == 1);
    QVERIFY(Prob == -FLT_MAX);

    Word = "فک";
    Prob = SS.wordProb(Word, Gram);
    QVERIFY(Gram == 1);
    QVERIFY(Prob == -FLT_MAX);

    Word = "پایین";
    Prob = SS.wordProb(Word, Gram);
    QVERIFY(Gram == 1);
    QVERIFY(qFuzzyCompare(Prob, -3.12613f));


    Word = "با";
    Prob = SS.wordProb(Word, Gram);
    QVERIFY(Gram == 1);
    QVERIFY(qFuzzyCompare(Prob, -1.94521f));


    Word = "دندان‌های";
    Prob = SS.wordProb(Word, Gram);
    QVERIFY(Gram == 1);
    QVERIFY(Prob == -FLT_MAX);

    Word = "سالم";
    Prob = SS.wordProb(Word, Gram);
    QVERIFY(Gram == 1);
    QVERIFY(Prob == -FLT_MAX);

    Word = ",";
    Prob = SS.wordProb(Word, Gram);
    QVERIFY(Gram == 1);
    QVERIFY(qFuzzyCompare(Prob, -1.55793f));

    Word = "بخشی";
    Prob = SS.wordProb(Word, Gram);
    QVERIFY(Gram == 1);
    QVERIFY(qFuzzyCompare(Prob, -3.45461f));

    Word = "از";
    Prob = SS.wordProb(Word, Gram);
    QVERIFY(Gram == 2);
    QVERIFY(qFuzzyCompare(Prob, -1.08925f));

    Word = "استخوان";
    Prob = SS.wordProb(Word, Gram);
    QVERIFY(Gram == 1);
    QVERIFY(Prob == -FLT_MAX);

    Word = "استخوان";
    Prob = SS.wordProb(Word, Gram);
    QVERIFY(Gram == 1);
    QVERIFY(Prob == -FLT_MAX);

    Word = "انگشت";
    Prob = SS.wordProb(Word, Gram);
    QVERIFY(Gram == 1);
    QVERIFY(Prob == -FLT_MAX);

    Word = "پا";
    Prob = SS.wordProb(Word, Gram);
    QVERIFY(Gram == 1);
    QVERIFY(Prob == -FLT_MAX);

    Word = "و";
    Prob = SS.wordProb(Word, Gram);
    QVERIFY(Gram == 1);
    QVERIFY(qFuzzyCompare(Prob, -1.36646f));


    Word = "استخوانهای";
    Prob = SS.wordProb(Word, Gram);
    QVERIFY(Gram == 1);
    QVERIFY(Prob == -FLT_MAX);


    Word = "سالم";
    Prob = SS.wordProb(Word, Gram);
    QVERIFY(Gram == 1);
    QVERIFY(Prob == -FLT_MAX);

    Word = "انگشت";
    Prob = SS.wordProb(Word, Gram);
    QVERIFY(Gram == 1);
    QVERIFY(Prob == -FLT_MAX);

    Word = "دست";
    Prob = SS.wordProb(Word, Gram);
    QVERIFY(Gram == 1);
    QVERIFY(qFuzzyCompare(Prob, -3.42716f));

    Word = "است";
    Prob = SS.wordProb(Word, Gram);
    QVERIFY(Gram == 1);
    QVERIFY(qFuzzyCompare(Prob, -1.94554f));

    Word = ".";
    Prob = SS.wordProb(Word, Gram);
    QVERIFY(Gram == 2);
    QVERIFY(qFuzzyCompare(Prob, -0.425969f));

//    Word =  LM_END_SENTENCE;
//    Prob = SS.wordProb(Word, Gram);
//    QVERIFY(Gram == 3);
//    QVERIFY(qFuzzyCompare(Prob, -0.0347621f));

    SS.reset();

    Word = "بهرام";
    Prob = SS.wordProb(Word, Gram);
    QVERIFY(Gram == 1);
    QVERIFY(Prob == -FLT_MAX);

    Word = "قاسمی";
    Prob = SS.wordProb(Word, Gram);
    QVERIFY(Gram == 1);
    QVERIFY(Prob == -FLT_MAX);

    Word = "افزود";
    Prob = SS.wordProb(Word, Gram);
    QVERIFY(Gram == 1);
    QVERIFY(Prob == -FLT_MAX);

    Word = ":";
    Prob = SS.wordProb(Word, Gram);
    QVERIFY(Gram == 1);
    QVERIFY(qFuzzyCompare(Prob, -1.96476f));

//    Word =  LM_END_SENTENCE;
//    Prob = SS.wordProb(Word, Gram);
//    QVERIFY(Gram == 2);
//    QVERIFY(qFuzzyCompare(Prob, -0.961127f));




//    QCOMPARE(Prob, -1.97194);




//    QString Sentence =
//            QStringLiteral("ارتباط او با سپاه پاسسسداران انقلالللب اسلاملللللی اووووو را در مرکز زنجیققرهای از ترور و جنایت قرار داده که دنیا را در برگرفته است .");

//    quint8 Gram;
//    foreach (const QString& Word, Sentence.split(" ")){
//        Targoman::Common::LogP_t Prob = SS.wordProb(Word, Gram);
//        qDebug()<<"Prob ["<<Word<<"]:Prob = "<<Prob<<" NGram = "<<Gram;
//    }

}

