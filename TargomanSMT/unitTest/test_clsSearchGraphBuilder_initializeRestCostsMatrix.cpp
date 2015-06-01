#include "UnitTest.h"

using namespace UnitTestNameSpace;
using namespace Targoman::SMT::Private::SearchGraphBuilder;
using namespace RuleTable;
using namespace InputDecomposer;
using namespace Targoman::Common;

class clsDummyScorerProxyForRestCost : public Proxies::intfLMSentenceScorer {
public:
    virtual void init(bool _justVocab) { Q_UNUSED(_justVocab) }
    virtual void initHistory(const intfLMSentenceScorer& _oldScorer) { Q_UNUSED(_oldScorer) }
    virtual void reset(bool _withStartOfSentence = true) { Q_UNUSED(_withStartOfSentence)}
    virtual LogP_t wordProb(const WordIndex_t& _wordIndex) { Q_UNUSED(_wordIndex); return 1;}
    virtual LogP_t endOfSentenceProb() { return 1;}
    virtual WordIndex_t getWordIndex(const QString& _word) { Q_UNUSED(_word); return 1;}
    virtual QString getWordByIndex(WordIndex_t _wordIndex) {Q_UNUSED(_wordIndex); return ""; }
    virtual bool haveSameHistoryAs(const intfLMSentenceScorer& _otherScorer) const {Q_UNUSED(_otherScorer); return true;}

    clsDummyScorerProxyForRestCost(int x) : Proxies::intfLMSentenceScorer(this->moduleName(), x) { }


    TARGOMAN_DEFINE_MODULE("clsDummyScorerProxyForRestCost", clsDummyScorerProxyForRestCost);
};

TARGOMAN_REGISTER_MODULE(clsDummyScorerProxyForRestCost);


class clsDummyFeatureFunctionForRestCost : public FeatureFunction::intfFeatureFunction {
public:
    ~clsDummyFeatureFunctionForRestCost(){}

    void initialize(const QString &){}

    Cost_t scoreSearchGraphNode(SearchGraphBuilder::clsSearchGraphNode&) const { return 0; }

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

    inline QStringList columnNames() const{return QStringList();}

    void initRootNode(SearchGraphBuilder::clsSearchGraphNode&) { }

public:
    clsDummyFeatureFunctionForRestCost():
        intfFeatureFunction(this->moduleName(), false)
    {}

    TARGOMAN_DEFINE_SINGLETONMODULE("clsDummyFeatureFunctionForRestCost", clsDummyFeatureFunctionForRestCost);
};

TARGOMAN_REGISTER_SINGLETON_MODULE(clsDummyFeatureFunctionForRestCost);

void clsUnitTest::test_clsSearchGraphBuilder_initializeRestCostsMatrix()
{


    gConfigs.EmptyLMScorer.reset(new clsDummyScorerProxyForRestCost(0));
    clsDummyFeatureFunctionForRestCost FeatureFunction;
    gConfigs.ActiveFeatureFunctions.insert("clsDummyFeatureFunctionForRestCost", &FeatureFunction);

    clsTargetRule::setColumnNames(QStringList() << "C1" << "C2");
    Sentence_t Sentence;
    Sentence << clsToken("word1", 1, "", QVariantMap())
             << clsToken("word2", 2, "", QVariantMap())
             << clsToken("word3", 3, "", QVariantMap())
             << clsToken("word4", 4, "", QVariantMap())
             << clsToken("word5", 5, "", QVariantMap());


    clsSearchGraph Builder(false, Sentence);

    for(int i = 0; i < Sentence.size(); ++i) {
        Builder.Data->PhraseCandidateCollections.append(
                    QVector<clsPhraseCandidateCollection>(Sentence.size() - i)
                    );
        if(true) {
            clsRuleNode RuleNode;
            int Size = rand() % 19 + 1;
            for(int k = 0; k < Size; ++k)
            {
                QList<WordIndex_t> TargetPhrase;
                int TargetPhraseSize = rand() % 5 + 1;
                for(int i = 0; i < TargetPhraseSize; ++i)
                    TargetPhrase << rand() % 5 + 1;
                RuleNode.Data->TargetRules.append(
                            clsTargetRule(TargetPhrase,
                                          QList<Cost_t>() << PredictableRandom() << PredictableRandom()));
            }
            Builder.Data->PhraseCandidateCollections[i][0] = clsPhraseCandidateCollection(i, i + 1, RuleNode);
        }
        for(int j = 1; j < Builder.Data->PhraseCandidateCollections[i].size(); ++j) {
            clsRuleNode RuleNode;
            int Size = (int)(PredictableRandom() * 20);
            Size *= (int)(PredictableRandom() * 2) ;
            for(int k = 0; k < Size; ++k)
            {
                QList<WordIndex_t> TargetPhrase;
                int TargetPhraseSize = (int)(PredictableRandom() * 5) + 1;
                for(int i = 0; i < TargetPhraseSize; ++i)
                    TargetPhrase << (int)(PredictableRandom() * 5) + 1;
                RuleNode.Data->TargetRules.append(
                            clsTargetRule(TargetPhrase,
                                          QList<Cost_t>() << PredictableRandom() << PredictableRandom()));
            }
            Builder.Data->PhraseCandidateCollections[i][j] = clsPhraseCandidateCollection(i, i + j + 1, RuleNode);
        }
    }

    Builder.initializeRestCostsMatrix();

    QVERIFY( qFuzzyCompare(Builder.Data->RestCostMatrix.at(0).at(0), 1.22934794498014) );
    QVERIFY( qFuzzyCompare(Builder.Data->RestCostMatrix.at(0).at(1), 0.957998220686536) );
    QVERIFY( qFuzzyCompare(Builder.Data->RestCostMatrix.at(0).at(2), 0.957998220686536) );
    QVERIFY( qFuzzyCompare(Builder.Data->RestCostMatrix.at(0).at(3), 0.957998220686536) );
    QVERIFY( qFuzzyCompare(Builder.Data->RestCostMatrix.at(0).at(4), 0.957998220686536) );
    QVERIFY( qFuzzyCompare(Builder.Data->RestCostMatrix.at(1).at(0), 0.957998220686536) );
    QVERIFY( qFuzzyCompare(Builder.Data->RestCostMatrix.at(1).at(1), 0.957998220686536) );
    QVERIFY( qFuzzyCompare(Builder.Data->RestCostMatrix.at(1).at(2), 0.957998220686536) );
    QVERIFY( qFuzzyCompare(Builder.Data->RestCostMatrix.at(1).at(3), 0.957998220686536) );
    QVERIFY( qFuzzyCompare(Builder.Data->RestCostMatrix.at(2).at(0), 0.957998220686536) );
    QVERIFY( qFuzzyCompare(Builder.Data->RestCostMatrix.at(2).at(1), 0.662847338678131) );
    QVERIFY( qFuzzyCompare(Builder.Data->RestCostMatrix.at(2).at(2), 0.662847338678131) );
    QVERIFY( qFuzzyCompare(Builder.Data->RestCostMatrix.at(3).at(0), 0.662847338678131) );
    QVERIFY( qFuzzyCompare(Builder.Data->RestCostMatrix.at(3).at(1), 0.662847338678131) );
    QVERIFY( qFuzzyCompare(Builder.Data->RestCostMatrix.at(4).at(0), 0.662847338678131) );

//    for(int i = 0; i < Builder.Data->RestCostMatrix.size(); ++i) {
//        for(int j = 0; j < Builder.Data->RestCostMatrix.at(i).size(); ++j) {
//            PRINT_QVERIFY_DOUBLE_PRECISIONS(
//                        Builder.Data->RestCostMatrix.at(i).at(j),i,j
//                        );
//        }
//    }

}

