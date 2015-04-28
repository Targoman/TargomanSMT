#include "UnitTest.h"
#include "libTargomanSMT/Private/FeatureFunctions/ReorderingJump/ReorderingJump.h"
using namespace UnitTestNameSpace;
using namespace Targoman::SMT::Private::FeatureFunction;
using namespace Targoman::Common;
using namespace InputDecomposer;

void clsUnitTest::test_ReorderingJump_getRestCostForPosition()
{
    ReorderingJump::ScalingFactor.setFromVariant(0.6);
    ReorderingJump reordeingJump;

    /*
    PRINT_QVERIFY_DOUBLE_PRECISIONS(reordeingJump.getRestCostForPosition(makeCoverageByString("01100101001"), 0, 10));
    PRINT_QVERIFY_DOUBLE_PRECISIONS(reordeingJump.getRestCostForPosition(makeCoverageByString("01101101001"), 0, 10));
    PRINT_QVERIFY_DOUBLE_PRECISIONS(reordeingJump.getRestCostForPosition(makeCoverageByString("11100101001"), 0, 10));
    PRINT_QVERIFY_DOUBLE_PRECISIONS(reordeingJump.getRestCostForPosition(makeCoverageByString("01001101001"), 0, 10));
    PRINT_QVERIFY_DOUBLE_PRECISIONS(reordeingJump.getRestCostForPosition(makeCoverageByString("01100101101"), 0, 10));
    PRINT_QVERIFY_DOUBLE_PRECISIONS(reordeingJump.getRestCostForPosition(makeCoverageByString("11100111001"), 0, 10));
    PRINT_QVERIFY_DOUBLE_PRECISIONS(reordeingJump.getRestCostForPosition(makeCoverageByString("00000000000"), 0, 0));
    PRINT_QVERIFY_DOUBLE_PRECISIONS(reordeingJump.getRestCostForPosition(makeCoverageByString("11111111111"), 0, 10));
    PRINT_QVERIFY_DOUBLE_PRECISIONS(reordeingJump.getRestCostForPosition(makeCoverageByString("11000111100"), 0, 2));
    PRINT_QVERIFY_DOUBLE_PRECISIONS(reordeingJump.getRestCostForPosition(makeCoverageByString("11001111100"), 0, 2));
    */

    QVERIFY( qFuzzyCompare(reordeingJump.getRestCostForPosition(makeCoverageByString("01100101001"), 0, 10), 58.8) );
    QVERIFY( qFuzzyCompare(reordeingJump.getRestCostForPosition(makeCoverageByString("01101101001"), 0, 10), 59.4) );
    QVERIFY( qFuzzyCompare(reordeingJump.getRestCostForPosition(makeCoverageByString("11100101001"), 0, 10), 4.8) );
    QVERIFY( qFuzzyCompare(reordeingJump.getRestCostForPosition(makeCoverageByString("01001101001"), 0, 10), 89.4) );
    QVERIFY( qFuzzyCompare(reordeingJump.getRestCostForPosition(makeCoverageByString("01100101101"), 0, 10), 59.4) );
    QVERIFY( qFuzzyCompare(reordeingJump.getRestCostForPosition(makeCoverageByString("11100111001"), 0, 10), 5.4) );
    QVERIFY( qFuzzyCompare(reordeingJump.getRestCostForPosition(makeCoverageByString("00000000000"), 0, 0), 0.6) );
    QVERIFY( qFuzzyCompare(reordeingJump.getRestCostForPosition(makeCoverageByString("11111111111"), 0, 10), 0) );
    QVERIFY( qFuzzyCompare(reordeingJump.getRestCostForPosition(makeCoverageByString("11000111100"), 0, 2), 3) );
    QVERIFY( qFuzzyCompare(reordeingJump.getRestCostForPosition(makeCoverageByString("11001111100"), 0, 2), 3.6) );

}
