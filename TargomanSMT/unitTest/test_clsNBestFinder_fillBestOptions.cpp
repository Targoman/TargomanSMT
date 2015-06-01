#include <UnitTest.h>
#include "libTargomanSMT/Private/SearchGraphBuilder/clsLexicalHypothesis.h"
#include "libTargomanSMT/Private/SearchGraphBuilder/clsSearchGraphNode.h"
#include "libTargomanSMT/Private/RuleTable/clsTargetRule.h"
#include "libTargomanSMT/Private/N-BestFinder/NBestSuggestions.h"


using namespace UnitTestNameSpace;
using namespace Targoman::SMT::Private::SearchGraphBuilder;
using namespace Targoman::SMT::Private::RuleTable;
using namespace Targoman::SMT::Private::NBestFinder;
using namespace Targoman::SMT;
using namespace Targoman::Common;
using namespace InputDecomposer;

void clsUnitTest::test_clsNBestFinder_fillBestOptions()
{

    gConfigs.ActiveFeatureFunctions.clear();
    Sentence_t Sentence;
    Sentence << clsToken("word1", 1, "", QVariantMap())
             << clsToken("word2", 2, "", QVariantMap())
             << clsToken("word3", 3, "", QVariantMap())
             << clsToken("word4", 4, "", QVariantMap())
             << clsToken("word5", 5, "", QVariantMap());

    clsSearchGraph SearchGraph(false,Sentence);

    InvalidSearchGraphNodeData  = new clsSearchGraphNodeData;
    pInvalidSearchGraphNode     = new clsSearchGraphNode;
    InvalidTargetRuleData       = new clsTargetRuleData;
    pInvalidTargetRule          = new clsTargetRule;


    clsTargetRule::ColumnNames.clear();
    clsTargetRule::setColumnNames(QStringList() << "C1" << "C2");

    clsTargetRule TargetRules[] = {
        clsTargetRule(
            QList<WordIndex_t>() << 1 << 2,
            QList<Cost_t>() << 0.1 << 0.1
        ),
        clsTargetRule(
            QList<WordIndex_t>() << 2 << 3,
            QList<Cost_t>() << 0.2 << 0.2
        ),
        clsTargetRule(
            QList<WordIndex_t>() << 3 << 4,
            QList<Cost_t>() << 0.3 << 0.3
        ),
        clsTargetRule(
            QList<WordIndex_t>() << 4 << 5 ,
            QList<Cost_t>() << 0.4 << 0.4
        ),
        clsTargetRule(
            QList<WordIndex_t>() << 12 << 2,
            QList<Cost_t>() << 0.5 << 0.5
        ),
        clsTargetRule(
            QList<WordIndex_t>() << 3 << 5,
            QList<Cost_t>() << 0.6 << 0.6
        ),
        clsTargetRule(
            QList<WordIndex_t>() << 8 << 8,
            QList<Cost_t>() << 0.7 << 0.7
        ),
        clsTargetRule(
            QList<WordIndex_t>() << 9 << 2,
            QList<Cost_t>() << 0.8 << 0.8
        )
    };

    clsSearchGraphNode RootNode = *pInvalidSearchGraphNode;

    gConfigs.ActiveFeatureFunctions.clear();

    clsSearchGraphNode Best1st(RootNode, 0, 1, makeCoverageByString("10000"),
                                TargetRules[0], false, 4.1 );
    clsSearchGraphNode Best2nd(Best1st, 1, 2, makeCoverageByString("11000"),
                                TargetRules[0], false, 3.2 );
    clsSearchGraphNode Best3rd(Best2nd, 2, 3, makeCoverageByString("11100"),
                                TargetRules[0], false, 2.3 );
    clsSearchGraphNode Best4th(Best3rd, 3, 4, makeCoverageByString("11110"),
                                TargetRules[0], false, 1.7 );
    clsSearchGraphNode Best5th(Best4th, 4, 5, makeCoverageByString("11111"),
                                TargetRules[0], false, 0 );

    clsSearchGraphNode* Bests[] = {
        &Best1st, &Best2nd, &Best3rd, &Best4th, &Best5th
    };

    clsSearchGraphNode Node[] = {
        clsSearchGraphNode(RootNode , 0, 1, makeCoverageByString("10000"), TargetRules[1], false, 8.1 ),
        clsSearchGraphNode(RootNode , 0, 1, makeCoverageByString("10000"), TargetRules[2], false, 10.1 ),
        clsSearchGraphNode(Best1st  , 1, 2, makeCoverageByString("11000"), TargetRules[1], false, 3.1 ),
        clsSearchGraphNode(Best1st  , 1, 2, makeCoverageByString("11000"), TargetRules[2], false, 6.7 ),
        clsSearchGraphNode(Best2nd  , 2, 3, makeCoverageByString("11100"), TargetRules[1], false, 4.2 ),
        clsSearchGraphNode(Best2nd  , 2, 3, makeCoverageByString("11100"), TargetRules[2], false, 4.5 ),
        clsSearchGraphNode(Best2nd  , 2, 3, makeCoverageByString("11100"), TargetRules[3], false, 7.2 ),
        clsSearchGraphNode(Best3rd  , 3, 4, makeCoverageByString("11110"), TargetRules[1], false, 1.2 ),
        clsSearchGraphNode(Best3rd  , 3, 4, makeCoverageByString("11110"), TargetRules[2], false, 2.2 ),
        clsSearchGraphNode(Best3rd  , 3, 4, makeCoverageByString("11110"), TargetRules[3], false, 5.2 )
    };


    SearchGraph.Data->HypothesisHolder.clear();
    SearchGraph.Data->HypothesisHolder.resize(Sentence.size() + 1);

    clsLexicalHypothesisContainer lexicalHypoContainer;

    lexicalHypoContainer.Data->Nodes.push_back(RootNode);
    SearchGraph.Data->HypothesisHolder[0][makeCoverageByString("00000")] = lexicalHypoContainer;

    lexicalHypoContainer = clsLexicalHypothesisContainer();
    lexicalHypoContainer.Data->Nodes.push_back(Best1st);
    lexicalHypoContainer.Data->Nodes.push_back(Node[0]);
    lexicalHypoContainer.Data->Nodes.push_back(Node[1]);
    SearchGraph.Data->HypothesisHolder[1][makeCoverageByString("10000")] = lexicalHypoContainer;

    lexicalHypoContainer = clsLexicalHypothesisContainer();
    lexicalHypoContainer.Data->Nodes.push_back(Best2nd);
    lexicalHypoContainer.Data->Nodes.push_back(Node[2]);
    //lexicalHypoContainer.Data->Nodes.push_back(Node[3]);
    SearchGraph.Data->HypothesisHolder[2][makeCoverageByString("11000")] = lexicalHypoContainer;

    lexicalHypoContainer = clsLexicalHypothesisContainer();
    lexicalHypoContainer.Data->Nodes.push_back(Best3rd);
    lexicalHypoContainer.Data->Nodes.push_back(Node[4]);
    lexicalHypoContainer.Data->Nodes.push_back(Node[5]);
    lexicalHypoContainer.Data->Nodes.push_back(Node[6]);
    SearchGraph.Data->HypothesisHolder[3][makeCoverageByString("11100")] = lexicalHypoContainer;

    lexicalHypoContainer = clsLexicalHypothesisContainer();
    lexicalHypoContainer.Data->Nodes.push_back(Best4th);
    lexicalHypoContainer.Data->Nodes.push_back(Node[7]);
    lexicalHypoContainer.Data->Nodes.push_back(Node[8]);
    lexicalHypoContainer.Data->Nodes.push_back(Node[9]);
    SearchGraph.Data->HypothesisHolder[4][makeCoverageByString("11110")] = lexicalHypoContainer;

    lexicalHypoContainer = clsLexicalHypothesisContainer();
    lexicalHypoContainer.Data->Nodes.push_back(Best5th);
    SearchGraph.Data->HypothesisHolder[5][makeCoverageByString("11111")] = lexicalHypoContainer;

    NBestFinder::NBestSuggestions::MaxSuggestions.setFromVariant(3);

    NBestSuggestions::Container_t NBest;

    NBestSuggestions::fillBestOptions(NBest, SearchGraph, Best5th);

//    NBestFinder::NBestSuggestions::Container_t NBest =
//            NBestFinder::NBestSuggestions::retrieve(SearchGraph);
    QVERIFY(NBest.size() == 5);

    int ExpectedCandidateCount[] = {
      3, 2, 3, 3, 1
    };
    int ExpectedNodeIndices[][2] = {
        { 0, 1 },
        { 2, -1 },
        { 4, 5 },
        { 7, 8 },
        { -1, -1 }
    };

    for(int i = 0; i < 5; ++i)
    {
        QVERIFY(NBest.find(stuPhrasePos(i, i+1)) != NBest.end());
        QVERIFY(NBest.value(stuPhrasePos(i, i+1)).TargetRules.size() == ExpectedCandidateCount[i]);
        QVERIFY(NBest.value(stuPhrasePos(i, i+1)).Pos == stuPhrasePos(2*i, 2*i + 2));
        QVERIFY(NBest.value(stuPhrasePos(i, i+1)).TargetRules.at(0).isSame(Bests[i]->targetRule()));
        for(int j = 1; j < ExpectedCandidateCount[i]; ++j) {
            QVERIFY(NBest.value(stuPhrasePos(i, i+1)).TargetRules.at(j).isSame(Node[
                                                                                      ExpectedNodeIndices[i][j - 1]
                                                                                      ].targetRule()));
        }
    }


}



