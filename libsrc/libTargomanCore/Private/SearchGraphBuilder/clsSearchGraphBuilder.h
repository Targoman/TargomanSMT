/*************************************************************************
 * Copyright © 2012-2015, Targoman.com
 *
 * Published under the terms of TCRL(Targoman Community Research License)
 * You can find a copy of the license file with distributed source or
 * download it from http://targoman.com/License.txt
 *
 *************************************************************************/
/**
 @author S. Mohammad M. Ziabary <smm@ziabary.com>
 @author Behrooz Vedadian <vedadian@gmail.com>
 */

#ifndef TARGOMAN_CORE_PRIVATE_SEARCHGRAPHBUILDER_CLSSEARCHGRAPHBUILDER_H
#define TARGOMAN_CORE_PRIVATE_SEARCHGRAPHBUILDER_CLSSEARCHGRAPHBUILDER_H

#include "Private/RuleTable/intfRuleTable.hpp"
#include "libTargomanCommon/Configuration/tmplConfigurable.h"
#include "Private/InputDecomposer/clsInput.h"
#include "clsHypothesisHolder.h"
#include "clsSearchGraphNode.h"
#include "Private/FeatureFunctions/intfFeatureFunction.hpp"

namespace Targoman{
namespace Core {
namespace Private{
namespace SearchGraphBuilder {

class clsSearchGraphBuilderData : public QSharedData
{
public:
    clsSearchGraphBuilderData(const InputDecomposer::Sentence_t& _sentence):
        Sentence(_sentence)
    {}
    clsSearchGraphBuilderData(const clsSearchGraphBuilderData& _other):
        PhraseMatchTable(_other.PhraseMatchTable),
        RootNode(_other.RootNode),
        GoalNode(_other.GoalNode),
        MaxMatchingSourcePhraseCardinality(_other.MaxMatchingSourcePhraseCardinality),
        HypothesisHolder(_other.HypothesisHolder),
        Sentence(_other.Sentence),
        RestCostMatrix(_other.RestCostMatrix)
    {}
    ~clsSearchGraphBuilderData(){}

public:
    QList<QVector<RuleTable::clsRuleNode>> PhraseMatchTable; //TODO convert to cacheable

    clsSearchGraphNode                  RootNode;
    clsSearchGraphNode                  GoalNode;

    int MaxMatchingSourcePhraseCardinality;

    clsHypothesisHolder                  HypothesisHolder;

    const InputDecomposer::Sentence_t &  Sentence;

    QVector<QVector<Common::Cost_t> > RestCostMatrix;
};


class clsSearchGraphBuilder
{
public:
    clsSearchGraphBuilder(const InputDecomposer::Sentence_t& _sentence);

    static void init();
    void matchPhrase();
    bool parseSentence();

    Common::Cost_t computeReorderingJumpCost(size_t JumpWidth);
private:
    void initializePhraseRestCostsMatrix();
    Common::Cost_t calculateRestCost(Coverage_t _coverage, quint16 _lastPos) const;
    Common::Cost_t computePhraseRestCosts(Coverage_t _coverage) const;
    Common::Cost_t computeReorderingRestCosts(Coverage_t _coverage, quint16 _lastPos) const;
    void initializeRestCostsMatrix();

private:
    QExplicitlySharedDataPointer<clsSearchGraphBuilderData> Data;

    static RuleTable::intfRuleTable*     pRuleTable;
    static FeatureFunction::intfFeatureFunction*           PhraseTable;
    static Common::Configuration::tmplConfigurable<quint8> ReorderingConstraintMaximumRuns;
    static Common::Configuration::tmplConfigurable<bool>   DoComputeReorderingRestCosts;
    static Common::Configuration::tmplConfigurable<bool>   PruneAtStage2;
    static Common::Configuration::tmplConfigurable<bool>   PruneAtStage3;
    static Common::Configuration::tmplConfigurable<bool>   PruneAtStage4;

    static Common::Configuration::tmplConfigurable<bool>   ReorderingHardJumpLimit;
    static Common::Configuration::tmplConfigurable<quint8> ReorderingMaximumJumpWidth;
    static Common::Configuration::tmplConfigurable<quint8> ObservationHistogramSize;

    static Common::Configuration::tmplConfigurable<double> ScalingFactorReorderingJump;

};

}
}
}
}

#endif // TARGOMAN_CORE_PRIVATE_SEARCHGRAPHBUILDER_CLSSEARCHGRAPHBUILDER_H
