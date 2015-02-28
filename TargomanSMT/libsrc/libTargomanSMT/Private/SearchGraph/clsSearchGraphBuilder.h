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
#include "clsHypothesisHolder.hpp"
#include "clsSearchGraphNode.h"
#include "Private/FeatureFunctions/intfFeatureFunction.hpp"

namespace Targoman{
namespace SMT {
namespace Private{
namespace SearchGraph {

class clsPhraseCandidateCollectionData : public QSharedData {
public:
    clsPhraseCandidateCollectionData()
    {
        this->UsableTargetRuleCount = 0;
        this->BestApproximateCost = INFINITY;
    }

    clsPhraseCandidateCollectionData(const clsPhraseCandidateCollectionData& _other) :
        QSharedData(_other),
        UsableTargetRuleCount(_other.UsableTargetRuleCount),
        TargetRules(_other.TargetRules),
        BestApproximateCost(_other.BestApproximateCost)
    {}

    clsPhraseCandidateCollectionData(size_t _beginPos, size_t _endPos, const RuleTable::clsRuleNode& _ruleNode);

public:
    int UsableTargetRuleCount;
    QList<RuleTable::clsTargetRule> TargetRules;
    Common::Cost_t BestApproximateCost;

private:
    static Common::Configuration::tmplConfigurable<quint8> ObservationHistogramSize;
};

class clsPhraseCandidateCollection {
public:
    clsPhraseCandidateCollection() :
        Data(new clsPhraseCandidateCollectionData)
    {}
    clsPhraseCandidateCollection(const clsPhraseCandidateCollection& _other) :
        Data(_other.Data)
    {}
    clsPhraseCandidateCollection(size_t _beginPos, size_t _endPos,  const RuleTable::clsRuleNode& _ruleNode) :
        Data(new clsPhraseCandidateCollectionData(_beginPos, _endPos, _ruleNode))
    {}

    const QList<RuleTable::clsTargetRule>& targetRules() const {
        return this->Data->TargetRules;
    }
    Common::Cost_t bestApproximateCost() const {
        return this->Data->BestApproximateCost;
    }

    bool isInvalid() const {
        return this->Data->TargetRules.size() == 0;
    }

    inline int usableTargetRuleCount() const {
        return this->Data->UsableTargetRuleCount;
    }

private:
    QExplicitlySharedDataPointer<clsPhraseCandidateCollectionData> Data;
};

class clsSearchGraphBuilderData : public QSharedData
{
public:
    clsSearchGraphBuilderData(const InputDecomposer::Sentence_t& _sentence):
        HypothesisHolder(_sentence.size()),
        Sentence(_sentence)
    {}
    clsSearchGraphBuilderData(const clsSearchGraphBuilderData& _other):
        QSharedData(_other),
        PhraseCandidateCollections(_other.PhraseCandidateCollections),
        GoalNode(_other.GoalNode),
        MaxMatchingSourcePhraseCardinality(_other.MaxMatchingSourcePhraseCardinality),
        HypothesisHolder(_other.HypothesisHolder),
        Sentence(_other.Sentence),
        RestCostMatrix(_other.RestCostMatrix)
    {}
    ~clsSearchGraphBuilderData(){}

public:
    QList<QVector<clsPhraseCandidateCollection>>      PhraseCandidateCollections;
    const clsSearchGraphNode*                   GoalNode;
    int                                         MaxMatchingSourcePhraseCardinality;
    clsHypothesisHolder                         HypothesisHolder;
    const InputDecomposer::Sentence_t &         Sentence;
    RestCostMatrix_t                            RestCostMatrix;
};


class clsSearchGraphBuilder
{
public:
    clsSearchGraphBuilder(const InputDecomposer::Sentence_t& _sentence);
    clsSearchGraphBuilder(){}
    clsSearchGraphBuilder(const clsSearchGraphBuilder& _other) :
        Data(_other.Data)
    {}

    static void init(const QString &_configFilePath);
    void collectPhraseCandidates();
    bool decode();

    inline const clsSearchGraphNode& goalNode() const{
        return *this->Data->GoalNode;
    }

    inline const QList<clsSearchGraphNode>& getSameCoverageNodes(Coverage_t _coverage) const {
        return this->Data->HypothesisHolder[_coverage.count(true)].lexicalHypotheses().value(_coverage).nodes();
    }

public:
    static inline QString moduleName(){return "SearchGraphBuilder";}
    static inline QString moduleBaseconfig(){return "/" + clsSearchGraphBuilder::moduleName();}

private:
    Common::Cost_t computeReorderingJumpCost(size_t JumpWidth) const;
    void initializePhraseRestCostsMatrix();
    Common::Cost_t calculateRestCost(const Coverage_t& _coverage, quint16 _lastPos) const;
    Common::Cost_t computePhraseRestCosts(const Coverage_t& _coverage) const;
    Common::Cost_t computeReorderingRestCosts(const Coverage_t& _coverage, quint16 _lastPos) const;
    void initializeRestCostsMatrix();
    bool conformsIBM1Constraint(const Coverage_t& _newCoverage);
    Common::Cost_t calculateRestCost(const Coverage_t &_coverage, size_t _beginPos, size_t _endPos) const;

private:
    QExplicitlySharedDataPointer<clsSearchGraphBuilderData> Data;

    static RuleTable::intfRuleTable*                       pRuleTable;
    static FeatureFunction::intfFeatureFunction*           pPhraseTable;
    static RuleTable::clsRuleNode*                         UnknownWordRuleNode;
    static Common::Configuration::tmplConfigurable<quint8> ReorderingConstraintMaximumRuns;
    static Common::Configuration::tmplConfigurable<bool>   DoComputePositionSpecificRestCosts;
    static Common::Configuration::tmplConfigurable<bool>   PruneAtStage2;
    static Common::Configuration::tmplConfigurable<bool>   PruneAtStage3;
    static Common::Configuration::tmplConfigurable<bool>   PruneAtStage4;
};

}
}
}
}

#endif // TARGOMAN_CORE_PRIVATE_SEARCHGRAPHBUILDER_CLSSEARCHGRAPHBUILDER_H
