#include "UnitTest.h"
using namespace UnitTestNameSpace;
using namespace Targoman::SMT::Private::SearchGraph;

void clsUnitTest::test_clsSearchGraphBuilder_conformsIBM1Constraint()
{
     Coverage_t  TestConverage;
     clsSearchGraphBuilder SearchGraphBuilder;

     clsSearchGraphBuilder::ReorderingConstraintMaximumRuns.setFromVariant(4);
     TestConverage = makeCoverageByString("110000111000");
     QVERIFY(SearchGraphBuilder.conformsIBM1Constraint(TestConverage) == true);

     clsSearchGraphBuilder::ReorderingConstraintMaximumRuns.setFromVariant(3);
     QVERIFY(SearchGraphBuilder.conformsIBM1Constraint(TestConverage) == false);

     clsSearchGraphBuilder::ReorderingConstraintMaximumRuns.setFromVariant(4);
     TestConverage = makeCoverageByString("110000");
     QVERIFY(SearchGraphBuilder.conformsIBM1Constraint(TestConverage) == true);

     TestConverage = makeCoverageByString("000000");
     QVERIFY(SearchGraphBuilder.conformsIBM1Constraint(TestConverage) == true);

     TestConverage = makeCoverageByString("111111");
     QVERIFY(SearchGraphBuilder.conformsIBM1Constraint(TestConverage) == true);

}
