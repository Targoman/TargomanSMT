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

/**
 * @brief The clsSearchGraphBuilderData class stores data members of clsSearchGraphBuilder class
 * and it is the top most container. #HypothesisHolder stores a list clsCardinalityHypothesisContainer instances
 * for translation process.
 */
class clsSearchGraphBuilderData : public QSharedData
{
public:
    /**
     * @brief Constructor of this class stores input sentence to one of its data members
     * and resizes hypothesis holder with input sentence size.
     * @param _sentence input sentence.
     */
    clsSearchGraphBuilderData(const InputDecomposer::Sentence_t& _sentence):
        HypothesisHolder(_sentence.size()),
        Sentence(_sentence)
    {}
    /**
     * @brief This is a copy constructor for this class
     */
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
    QList<QVector<clsPhraseCandidateCollection>>        PhraseMatchTable;                       /**< Loaded phrase table will be stored in this 2D container. The first dimension is correspond to begin position of sentence and the second dimesion is for end position of sentence.*/
    const clsSearchGraphNode*                           GoalNode;                               /**< Our best founded translation*/
    int                                                 MaxMatchingSourcePhraseCardinality;     /**< Max length of source phrases loaded from phrase table.*/
    clsHypothesisHolder                                 HypothesisHolder;                       /**< A Container to hold clsCardinalityHypothesisContainer */
    const InputDecomposer::Sentence_t&                  Sentence;                               /**< Input sentence.*/
    RestCostMatrix_t                                    RestCostMatrix;                         /**< A 2D container to store approximate rest cost of translation dim one correspond to begin pos of sentence and dim two correspond to end pos of sentence.*/
};

/**
 * @brief The clsSearchGraphBuilder class has the main duty of decoding process and loading phrase table contents.
 */
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

    /**
     * @brief Returns out best founded translation.
     */
    inline const clsSearchGraphNode& goalNode() const{
        return *this->Data->GoalNode;
    }

    /**
     * @brief Returns a list of search graph node that has similar coverage of translation.
     * @param[in] _coverage Covered translated words.
     */
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
