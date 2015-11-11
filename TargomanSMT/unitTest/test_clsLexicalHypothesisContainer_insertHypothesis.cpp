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

#include <UnitTest.h>
#include "libTargomanSMT/Private/SearchGraphBuilder/clsLexicalHypothesis.h"
#include "libTargomanSMT/Private/SearchGraphBuilder/clsSearchGraphNode.h"
#include "libTargomanSMT/Private/RuleTable/clsTargetRule.h"
using namespace UnitTestNameSpace;
using namespace Targoman::SMT::Private::SearchGraphBuilder;
using namespace Targoman::SMT::Private::RuleTable;
using namespace Targoman::Common;
//extern InvalidSearchGraphNodeData;

class clsDummyFeatureFunctionForInsertion : public FeatureFunction::intfFeatureFunction {
public:
    ~clsDummyFeatureFunctionForInsertion(){}

    void initialize(QSharedPointer<QSettings> ){}

    Cost_t scoreSearchGraphNodeAndUpdateFutureHash(SearchGraphBuilder::clsSearchGraphNode&, QCryptographicHash&) const { return 0; }

    Cost_t getRestCostForPosition(const Coverage_t& _coverage, size_t _beginPos, size_t endPos) const {
        Q_UNUSED(_coverage)
        Q_UNUSED(_beginPos)
        Q_UNUSED(endPos)
        return 0;
    }

    inline Cost_t getApproximateCost(unsigned _sourceStart,
                                             unsigned _sourceEnd,
                                             const RuleTable::clsTargetRule& _targetRule) const {
        Q_UNUSED(_sourceStart)
        Q_UNUSED(_sourceEnd)
        return (2 *_targetRule.field(0) + 3 * _targetRule.field(1));
    }

    int compareStates(const SearchGraphBuilder::clsSearchGraphNode& _first,
                                        const SearchGraphBuilder::clsSearchGraphNode& _second) const {
        Q_UNUSED(_second);
        if(_first.targetRule().at(0)  == 1)
            return 1;
        return 0;
    }

    inline QStringList columnNames() const{return QStringList();}

    void initRootNode(SearchGraphBuilder::clsSearchGraphNode&) { }

public:
    clsDummyFeatureFunctionForInsertion():
        intfFeatureFunction(this->moduleName(), false)
    {}

    TARGOMAN_DEFINE_SINGLETONMODULE("clsDummyFeatureFunctionForInsertion", clsDummyFeatureFunctionForInsertion);
};

TARGOMAN_REGISTER_SINGLETON_MODULE(clsDummyFeatureFunctionForInsertion);


void clsUnitTest::test_clsLexicalHypothesisContainer_insertHypothesis()
{
    clsLexicalHypothesisContainer::KeepRecombined.setFromVariant(false);
    clsLexicalHypothesisContainer HypoContainer;

    gConfigs.ActiveFeatureFunctions.clear();

    InvalidSearchGraphNodeData  = new clsSearchGraphNodeData;
    pInvalidSearchGraphNode     = new clsSearchGraphNode;
    clsTargetRule::ColumnNames.clear();
    clsTargetRule::setColumnNames(QStringList() << "C1" << "C2");

    Coverage_t Coverage = makeCoverageByString("1001001011");
    clsTargetRule TargetRules[] = {
        clsTargetRule(
            QList<WordIndex_t>() << 1 << 2,
            QList<Cost_t>() << 0.5 << 0.75
        ),
        clsTargetRule(
            QList<WordIndex_t>() << 2,
            QList<Cost_t>() << 1.5 << 0.75
        ),
        clsTargetRule(
            QList<WordIndex_t>() << 3,
            QList<Cost_t>() << 0.5 << 1.75
        ),
        clsTargetRule(
            QList<WordIndex_t>() << 4 << 5 << 6,
            QList<Cost_t>() << 1.5 << 1.75
        ),
        clsTargetRule(
            QList<WordIndex_t>() << 12,
            QList<Cost_t>() << 0.0 << 3.75
        )
    };


    clsDummyFeatureFunctionForInsertion dummyFeatureFunction;
    gConfigs.ActiveFeatureFunctions.insert("clsDummyFeatureFunctionForInsertion", &dummyFeatureFunction);
    clsSearchGraphNode node[] = {
        clsSearchGraphNode(*pInvalidSearchGraphNode, 2, 5, Coverage, TargetRules[0], false, 10.1 ),
        clsSearchGraphNode(*pInvalidSearchGraphNode, 2, 5, Coverage, TargetRules[1], false, 20.1),
        clsSearchGraphNode(*pInvalidSearchGraphNode, 2, 5, Coverage, TargetRules[2], false, 5.1),
        clsSearchGraphNode(*pInvalidSearchGraphNode, 2, 5, Coverage, TargetRules[3], false, 30.1),
        clsSearchGraphNode(*pInvalidSearchGraphNode, 2, 5, Coverage, TargetRules[4], false, 1.1),
        clsSearchGraphNode(*pInvalidSearchGraphNode, 2, 5, Coverage, TargetRules[0], false, 50.1),
        clsSearchGraphNode(*pInvalidSearchGraphNode, 2, 5, Coverage, TargetRules[1], false, 90.1),
        clsSearchGraphNode(*pInvalidSearchGraphNode, 2, 5, Coverage, TargetRules[2], false, 70.1)
    };


    QVERIFY( HypoContainer.insertHypothesis(node[0]) == 1 );
    QVERIFY( HypoContainer.insertHypothesis(node[1]) == 1 );
    QVERIFY( HypoContainer.insertHypothesis(node[2]) == 1 );
    QVERIFY( HypoContainer.insertHypothesis(node[3]) == 0 );
    QVERIFY( HypoContainer.insertHypothesis(node[4]) == 1 );
    QVERIFY( HypoContainer.insertHypothesis(node[5]) == 0 );
    QVERIFY( HypoContainer.insertHypothesis(node[6]) == 0 );
    QVERIFY( HypoContainer.insertHypothesis(node[7]) == 0 );


    /*
    PRINT_QVERIFY(HypoContainer.insertHypothesis(node[0]));
    PRINT_QVERIFY(HypoContainer.insertHypothesis(node[1]));
    PRINT_QVERIFY(HypoContainer.insertHypothesis(node[2]));
    PRINT_QVERIFY(HypoContainer.insertHypothesis(node[3]));
    PRINT_QVERIFY(HypoContainer.insertHypothesis(node[4]));
    PRINT_QVERIFY(HypoContainer.insertHypothesis(node[5]));
    PRINT_QVERIFY(HypoContainer.insertHypothesis(node[6]));
    PRINT_QVERIFY(HypoContainer.insertHypothesis(node[7]));
    */

    clsLexicalHypothesisContainer::KeepRecombined.setFromVariant(true);

    HypoContainer.Data->Nodes.clear();


    /*
    PRINT_QVERIFY(HypoContainer.insertHypothesis(node[0]));
    PRINT_QVERIFY(HypoContainer.insertHypothesis(node[1]));
    PRINT_QVERIFY(HypoContainer.insertHypothesis(node[2]));
    PRINT_QVERIFY(HypoContainer.insertHypothesis(node[3]));
    PRINT_QVERIFY(HypoContainer.insertHypothesis(node[4]));
    PRINT_QVERIFY(HypoContainer.insertHypothesis(node[5]));
    PRINT_QVERIFY(HypoContainer.insertHypothesis(node[6]));
    PRINT_QVERIFY(HypoContainer.insertHypothesis(node[7]));
    */

    QVERIFY( HypoContainer.insertHypothesis(node[0]) == 1 );
    QVERIFY( HypoContainer.insertHypothesis(node[1]) == 1 );
    QVERIFY( HypoContainer.insertHypothesis(node[2]) == 0 );
    QVERIFY( HypoContainer.insertHypothesis(node[3]) == 0 );
    QVERIFY( HypoContainer.insertHypothesis(node[4]) == 0 );
    QVERIFY( HypoContainer.insertHypothesis(node[5]) == 0 );
    QVERIFY( HypoContainer.insertHypothesis(node[6]) == 0 );
    QVERIFY( HypoContainer.insertHypothesis(node[7]) == 0 );



}
