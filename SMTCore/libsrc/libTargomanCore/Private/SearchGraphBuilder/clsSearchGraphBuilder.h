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
namespace Core {
namespace Private{
namespace SearchGraphBuilder {

TARGOMAN_ADD_EXCEPTION_HANDLER(exSearchGraphBuilder, exTargomanCore);

class clsSearchGraphBuilderData : public QSharedData
{
public:
    clsSearchGraphBuilderData(const InputDecomposer::Sentence_t& _sentence):
        HypothesisHolder(_sentence.size()),
        Sentence(_sentence)
    {}
    clsSearchGraphBuilderData(const clsSearchGraphBuilderData& _other):
        QSharedData(_other),
        PhraseMatchTable(_other.PhraseMatchTable),
        GoalNode(_other.GoalNode),
        MaxMatchingSourcePhraseCardinality(_other.MaxMatchingSourcePhraseCardinality),
        HypothesisHolder(_other.HypothesisHolder),
        Sentence(_other.Sentence),
        RestCostMatrix(_other.RestCostMatrix)
    {}
    ~clsSearchGraphBuilderData(){}

public:
    QList<QVector<RuleTable::clsRuleNode>>      PhraseMatchTable;
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


    static void init(const QString &_configFilePath);
    void matchPhrase();
    bool parseSentence();

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

    static Common::Configuration::tmplConfigurable<quint8> ObservationHistogramSize;
};

}
}
}
}

#endif // TARGOMAN_CORE_PRIVATE_SEARCHGRAPHBUILDER_CLSSEARCHGRAPHBUILDER_H
