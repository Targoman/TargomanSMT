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

#include "Private/PhraseTable/intfPhraseTable.hpp"
#include "libTargomanCommon/Configuration/tmplConfigurable.h"
#include "Private/InputDecomposer/clsInput.h"
#include "clsHypothesisHolder.h"
#include "clsSearchGraphNode.h"

namespace Targoman{
namespace Core {
namespace Private{
namespace SearchGraphBuilder {

class clsSearchGraphBuilder
{
public:
    clsSearchGraphBuilder(const Sentence_t& _sentence);

    static void init();
    void matchPhrase();
    void parseSentence();

    Common::Cost_t computeReorderingJumpCost(size_t JumpWidth);
private:
    void initializePhraseRestCostsMatrix();
    Common::Cost_t calculateRestCost(Coverage_t _coverage, quint16 _lastPos) const;
    Common::Cost_t computePhraseRestCosts(Coverage_t _coverage) const;
    Common::Cost_t computeReorderingRestCosts(Coverage_t _coverage, quint16 _lastPos) const;


private:
    //TODO maybe QsharedDataArray
    QList<QVector<const PhraseTable::clsRuleNode*>> PhraseMatchTable; //TODO convert to cacheable

    clsSearchGraphNode                  RootNode;

    int MaxMatchingSourcePhraseCardinality;

    clsHypothesisHolder                  HypothesisHolder;
    static PhraseTable::intfPhraseTable& PhraseTable;
    const Sentence_t & Sentence;

    QVector<QVector<Commmon::Cost_t> > RestCostMatrix;

    static Common::Configuration::tmplConfigurable<quint8> ReorderingConstraintMaximumRuns;
    static Common::Configuration::tmplConfigurable<bool> DoComputeReorderingRestCosts;
    static Common::Configuration::tmplConfigurable<bool> PruneAtStage2;
    static Common::Configuration::tmplConfigurable<bool> PruneAtStage3;
    static Common::Configuration::tmplConfigurable<bool> PruneAtStage4;

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
