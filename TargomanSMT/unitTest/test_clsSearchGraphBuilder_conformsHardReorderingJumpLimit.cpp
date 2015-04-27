#include "UnitTest.h"
using namespace UnitTestNameSpace;
using namespace Targoman::SMT::Private::SearchGraph;

void clsUnitTest::test_clsSearchGraphBuilder_conformsHardReorderingJumpLimit()
{
    Coverage_t Coverage;
    size_t EndPosition;
    clsSearchGraphBuilder SearchGraphBuilder;

    clsSearchGraphBuilder::HardReorderingJumpLimit.setFromVariant(2);
    Coverage = makeCoverageByString("1100100101");
    EndPosition = 7;
    QVERIFY(SearchGraphBuilder.conformsHardReorderingJumpLimit(Coverage, EndPosition)
            == false );

    clsSearchGraphBuilder::HardReorderingJumpLimit.setFromVariant(4);
    Coverage = makeCoverageByString("1100100101");
    EndPosition = 4;
    QVERIFY(SearchGraphBuilder.conformsHardReorderingJumpLimit(Coverage, EndPosition)
            == true );

}
