/**
 *@author Saeed Torabzadeh <saeed.torabzadeh@gmail.com>
 */

#include "UnitTest.h"

using namespace Targoman::NLPLibs;

void UnitTest::testStringBased()
{
    clsLanguageModel LM;
    Targoman::Common::LogP_t UnkProb = 0;
    Targoman::Common::LogP_t UnkBackoff = 0;
    bool UseIdexBasedModel = false;

    stuLMConfigs languageModelConfig( UnkProb, UnkBackoff, UseIdexBasedModel);

    quint8 order = LM.init("./test.arpa", languageModelConfig);
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

    Word =  LM_END_SENTENCE;
    Prob = SS.wordProb(Word, Gram);
    QVERIFY(Gram == 3);
    QVERIFY(qFuzzyCompare(Prob, -0.0347621f));

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

    Word =  LM_END_SENTENCE;
    Prob = SS.wordProb(Word, Gram);
    QVERIFY(Gram == 2);
    QVERIFY(qFuzzyCompare(Prob, -0.961127f));



}

