#include "UnitTest.h"
using namespace UnitTestNameSpace;
using namespace Targoman::SMT::Private::SearchGraphBuilder;

void clsUnitTest::test_clsSearchGraphBuilder_conformsIBM1Constraint()
{
     Coverage_t  TestConverage;
     clsSearchGraph SearchGraphBuilder(false,InputDecomposer::Sentence_t());

     clsSearchGraph::ReorderingConstraintMaximumRuns.setFromVariant(4);
     TestConverage = makeCoverageByString("110000111000");
     QVERIFY(SearchGraphBuilder.conformsIBM1Constraint(TestConverage) == true);

     clsSearchGraph::ReorderingConstraintMaximumRuns.setFromVariant(3);
     QVERIFY(SearchGraphBuilder.conformsIBM1Constraint(TestConverage) == false);

     clsSearchGraph::ReorderingConstraintMaximumRuns.setFromVariant(4);
     TestConverage = makeCoverageByString("110000");
     QVERIFY(SearchGraphBuilder.conformsIBM1Constraint(TestConverage) == true);

     TestConverage = makeCoverageByString("000000");
     QVERIFY(SearchGraphBuilder.conformsIBM1Constraint(TestConverage) == true);

     TestConverage = makeCoverageByString("111111");
     QVERIFY(SearchGraphBuilder.conformsIBM1Constraint(TestConverage) == true);

}
