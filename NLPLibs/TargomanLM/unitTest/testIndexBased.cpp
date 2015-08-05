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
    quint8 Gram;
    QString Sentence = QStringLiteral("این استخوان‌ها شامل یک استخوان فک بالا و دو استخوان فک پایین با دندان‌های سالم , بخشی از استخوان استخوان انگشت پا و استخوانهای سالم انگشت دست است . بهرام قاسمی افزود : ");
    QStringList Words = Sentence.split(" ",QString::SkipEmptyParts);

    QList<int> FoundGrams = {
        2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,1,1,1,1,1,1,1,1,1,1,2,1,1,1,1
    };

    QList<float> FoundProbs = {
        -1.97194,-2.26817,-2.25566,-2.76081,-2.35332,-2.25566,-2.25566,-1.39908,-3.41515,-2.2596,-2.25566,-3.15875,-1.94521,-2.27029,-2.25566,-1.59055,-3.45461,-1.08925,-2.30768,-2.25566,-2.25566,-2.25566,-1.39908,-2.21103,-2.25566,-2.25566,-3.45978,-1.94554,-0.425969,-3.32043,-2.25566,-2.25566,-1.99738
    };

    for(int i = 0; i < Words.size(); ++i) {
        Targoman::Common::LogP_t Prob = SS.wordProb(Words[i], Gram);
        QVERIFY(Gram == FoundGrams[i]);
        QVERIFY(qFuzzyCompare(Prob, FoundProbs[i]));
    }

    SS.reset();

    for(int i = 0; i < Words.size(); ++i) {
        Targoman::Common::LogP_t Prob = SS.wordProb(Words[i], Gram);
        QVERIFY(Gram == FoundGrams[i]);
        QVERIFY(qFuzzyCompare(Prob, FoundProbs[i]));
    }


}

