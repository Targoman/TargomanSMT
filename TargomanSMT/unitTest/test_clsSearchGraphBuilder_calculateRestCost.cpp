#include "UnitTest.h"

using namespace UnitTestNameSpace;
using namespace Targoman::SMT::Private::SearchGraph;
using namespace Targoman::Common;
using namespace InputDecomposer;

void clsUnitTest::test_clsSearchGraphBuilder_calculateRestCost()
{
    Sentence_t Sentence;
    Sentence << clsToken("word1", 1, "", QVariantMap())
             << clsToken("word2", 2, "", QVariantMap())
             << clsToken("word3", 3, "", QVariantMap())
             << clsToken("word4", 4, "", QVariantMap())
             << clsToken("word5", 5, "", QVariantMap());

    clsSearchGraphBuilder::DoComputePositionSpecificRestCosts.setFromVariant(false);
    clsSearchGraphBuilder Builder(Sentence);

    Builder.Data->RestCostMatrix.resize(Sentence.size());

    Builder.Data->RestCostMatrix.resize(Sentence.size());
    for (int SentenceStartPos=0; SentenceStartPos<Builder.Data->Sentence.size(); ++SentenceStartPos)
        Builder.Data->RestCostMatrix[SentenceStartPos].fill(0, Sentence.size() - SentenceStartPos);
    for (int SentenceStartPos=0; SentenceStartPos<Builder.Data->Sentence.size(); ++SentenceStartPos)
        for(int length = 0; length < Sentence.size() - SentenceStartPos; ++length)
            Builder.Data->RestCostMatrix[SentenceStartPos][length] = PredictableRandom();

    QVERIFY( qFuzzyCompare(Builder.calculateRestCost(makeCoverageByString("01001"), 1, 2), 1.28471041407369) );
    QVERIFY( qFuzzyCompare(Builder.calculateRestCost(makeCoverageByString("00011"), 3, 5), 0.0917703813655545) );
    QVERIFY( qFuzzyCompare(Builder.calculateRestCost(makeCoverageByString("10011"), 3, 5), 0.196123931831307) );
    QVERIFY( qFuzzyCompare(Builder.calculateRestCost(makeCoverageByString("10001"), 0, 1), 0.35054010570189) );
    QVERIFY( qFuzzyCompare(Builder.calculateRestCost(makeCoverageByString("11000"), 0, 2), 0.755917653166079) );


//    PRINT_QVERIFY_DOUBLE_PRECISIONS(Builder.calculateRestCost(makeCoverageByString("01001"), 1, 2));
//    PRINT_QVERIFY_DOUBLE_PRECISIONS(Builder.calculateRestCost(makeCoverageByString("00011"), 3, 5));
//    PRINT_QVERIFY_DOUBLE_PRECISIONS(Builder.calculateRestCost(makeCoverageByString("10011"), 3, 5));
//    PRINT_QVERIFY_DOUBLE_PRECISIONS(Builder.calculateRestCost(makeCoverageByString("10001"), 0, 1));
//    PRINT_QVERIFY_DOUBLE_PRECISIONS(Builder.calculateRestCost(makeCoverageByString("11000"), 0, 2));


}
