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
namespace SearchGraphBuilder {

/**
 * @brief The clsPhraseCandidateCollectionData class container for the clsPhraseCandidateCollection data
 */
class clsPhraseCandidateCollectionData : public QSharedData {
public:
    /**
     * @brief clsPhraseCandidateCollectionData  invalid phrase collection data constructor
     */
    clsPhraseCandidateCollectionData()
    {
        this->UsableTargetRuleCount = 0;
        this->BestApproximateCost = INFINITY;
    }

    /**
     * @brief clsPhraseCandidateCollectionData  copy constructor
     * @param _other                            the data class from which copy will be done
     */
    clsPhraseCandidateCollectionData(const clsPhraseCandidateCollectionData& _other) :
        QSharedData(_other),
        UsableTargetRuleCount(_other.UsableTargetRuleCount),
        TargetRules(_other.TargetRules),
        BestApproximateCost(_other.BestApproximateCost)
    {}

    /**
     * @brief clsPhraseCandidateCollectionData  main constructor that gets rule node, beginning and ending position for the candidate phrase and builds the usable target rule list
     * @param _beginPos                         beginning position of source phrase
     * @param _endPos                           ending position of source phrase
     * @param _ruleNode                         the rule node from which target rules will be considered
     */
    clsPhraseCandidateCollectionData(size_t _beginPos, size_t _endPos, const RuleTable::clsRuleNode& _ruleNode);

public:
    int UsableTargetRuleCount;
    QList<RuleTable::clsTargetRule> TargetRules;
    Common::Cost_t BestApproximateCost;

private:
    static Common::Configuration::tmplConfigurable<quint8> MaxTargetPhraseCount;

    friend class UnitTestNameSpace::clsUnitTest;
};

/**
 * @brief The clsPhraseCandidateCollection class    placeholder for the possible translations of an input phrase precomputed to speed up the translation process
 */\
class clsPhraseCandidateCollection {
public:
    /**
     * @brief clsPhraseCandidateCollection  invalid phrase collection constructor
     */
    clsPhraseCandidateCollection() :
        Data(new clsPhraseCandidateCollectionData)
    {}

    /**
     * @brief clsPhraseCandidateCollection  copy constructor
     * @param _other                        the collection to be copied
     */
    clsPhraseCandidateCollection(const clsPhraseCandidateCollection& _other) :
        Data(_other.Data)
    {}

    /**
     * @brief clsPhraseCandidateCollection  main constructor that gets rule node, beginning and ending position for the candidate phrase and calls the data constructor to build the usable rule list
     * @param _beginPos                     beginning position of source phrase
     * @param _endPos                       ending position of source phrase
     * @param _ruleNode                     the rule node from which target rules will be considered
     */
    clsPhraseCandidateCollection(size_t _beginPos, size_t _endPos,  const RuleTable::clsRuleNode& _ruleNode) :
        Data(new clsPhraseCandidateCollectionData(_beginPos, _endPos, _ruleNode))
    {}

    /**
     * @brief targetRules   getter function for target rules
     * @return
     */
    const QList<RuleTable::clsTargetRule>& targetRules() const {
        return this->Data->TargetRules;
    }

    /**
     * @brief bestApproximateCost   computed best approximate cost used for filling the rest cost precomputation matrix
     * @return
     */
    Common::Cost_t bestApproximateCost() const {
        return this->Data->BestApproximateCost;
    }

    /**
     * @brief isInvalid             finds out whether any target rules were found for the given source phrase
     * @return                      true if there are any candidate translations for the source phrase and false otherwise
     */
    bool isInvalid() const {
        return this->Data->TargetRules.size() == 0;
    }

    /**
     * @brief usableTargetRuleCount returns number of usable target rules taking into accout histogram pruning (limiting ourselves to n-best translation candidates for any given source phrase)
     * @return                      returns number of usable target rules taking into accout histogram pruning (limiting ourselves to n-best translation candidates for any given source phrase)
     */
    inline int usableTargetRuleCount() const {
        return this->Data->UsableTargetRuleCount;
    }

private:
    QExplicitlySharedDataPointer<clsPhraseCandidateCollectionData> Data;

    friend class UnitTestNameSpace::clsUnitTest;
};

/**
 * @brief The clsSearchGraphBuilderData class stores data members of clsSearchGraphBuilder class
 * and it is the top most container. #HypothesisHolder stores a list clsCardinalityHypothesisContainer instances
 * for translation process.
 */
class clsSearchGraphData : public QSharedData
{
public:
    /**
     * @brief Constructor of this class stores input sentence to one of its data members
     * and resizes hypothesis holder with input sentence size.
     * @param _sentence input sentence.
     */
    clsSearchGraphData(const InputDecomposer::Sentence_t& _sentence):
        HypothesisHolder(_sentence.size()),
        Sentence(_sentence)
    {}
    /**
     * @brief This is a copy constructor for this class
     */
    clsSearchGraphData(const clsSearchGraphData& _other):
        QSharedData(_other),
        PhraseCandidateCollections(_other.PhraseCandidateCollections),
        GoalNode(_other.GoalNode),
        MaxMatchingSourcePhraseCardinality(_other.MaxMatchingSourcePhraseCardinality),
        HypothesisHolder(_other.HypothesisHolder),
        Sentence(_other.Sentence),
        RestCostMatrix(_other.RestCostMatrix)
    {}
    ~clsSearchGraphData(){}

public:
    QList<QVector<clsPhraseCandidateCollection>>        PhraseCandidateCollections;             /**< Loaded phrase table will be stored in this 2D container. The first dimension is correspond to begin position of sentence and the second dimesion is for end position of sentence.*/
    const clsSearchGraphNode*                           GoalNode;                               /**< Our best founded translation*/
    int                                                 MaxMatchingSourcePhraseCardinality;     /**< Max length of source phrases loaded from phrase table.*/
    clsHypothesisHolder                                 HypothesisHolder;                       /**< A Container to hold clsCardinalityHypothesisContainer */
    const InputDecomposer::Sentence_t&                  Sentence;                               /**< Input sentence.*/
    RestCostMatrix_t                                    RestCostMatrix;                         /**< A 2D container to store approximate rest cost of translation dim one correspond to begin pos of sentence and dim two correspond to end pos of sentence.*/

    friend class UnitTestNameSpace::clsUnitTest;
};

/**
 * @brief The clsSearchGraphBuilder class has the main duty of decoding process and loading phrase table contents.
 */
class clsSearchGraph
{
public:
    clsSearchGraph(const InputDecomposer::Sentence_t& _sentence);

    static void init(const QString &_configFilePath);


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

    static inline void saveBinaryRuleTable(const QString& _filePath){
        Q_ASSERT(clsSearchGraph::pRuleTable != NULL);
        clsSearchGraph::pRuleTable->saveBinaryRuleTable(_filePath);
    }

public:
    static inline QString moduleName(){return "SearchGraphBuilder";}
    static inline QString moduleBaseconfig(){return "/" + clsSearchGraph::moduleName();}

private:
    clsSearchGraph(bool, const InputDecomposer::Sentence_t& _sentence) :
        Data(new clsSearchGraphData(_sentence))
    {/*Just used by unitTests*/}

    Q_DISABLE_COPY(clsSearchGraph)
    void collectPhraseCandidates();
    bool decode();
    Common::Cost_t computeReorderingJumpCost(size_t JumpWidth) const;
    Common::Cost_t calculateRestCost(const Coverage_t& _coverage, quint16 _lastPos) const;
    Common::Cost_t computePhraseRestCosts(const Coverage_t& _coverage) const;
    Common::Cost_t computeReorderingRestCosts(const Coverage_t& _coverage, quint16 _lastPos) const;
    void initializeRestCostsMatrix();
    bool conformsIBM1Constraint(const Coverage_t& _newCoverage);
    bool conformsHardReorderingJumpLimit(const Coverage_t &_coverage, size_t _endPos);
    Common::Cost_t calculateRestCost(const Coverage_t &_coverage, size_t _beginPos, size_t _endPos) const;

private:
    QExplicitlySharedDataPointer<clsSearchGraphData> Data;                               /**< A pointer to clsSearchGraphBuilderData class which manages data member of this class*/

    static RuleTable::intfRuleTable*                        pRuleTable;                          /**< Rule table loader e.g. Jane phrase Table loader */
    static FeatureFunction::intfFeatureFunction*            pPhraseTable;
    static RuleTable::clsRuleNode*                          UnknownWordRuleNode;                 /**< An instance of rule node for unknown word.*/
    static Common::Configuration::tmplConfigurable<quint8>  HardReorderingJumpLimit;
    static Common::Configuration::tmplConfigurable<quint8>  ReorderingConstraintMaximumRuns;     /**< A threshold that will be used in IBM1 constrains.*/
    static Common::Configuration::tmplConfigurable<bool>    DoComputePositionSpecificRestCosts;
    static Common::Configuration::tmplConfigurable<bool>    DoPrunePreInsertion;

    friend class UnitTestNameSpace::clsUnitTest;
};

}
}
}
}

#endif // TARGOMAN_CORE_PRIVATE_SEARCHGRAPHBUILDER_CLSSEARCHGRAPHBUILDER_H
