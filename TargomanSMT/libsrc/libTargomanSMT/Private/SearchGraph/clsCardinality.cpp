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

#include "clsCardinality.h"
#include "clsSearchGraphBuilder.h"
#include <iostream>

namespace Targoman{
namespace SMT {
namespace Private{
namespace SearchGraph {

using namespace Common;
using namespace Common::Configuration;


tmplConfigurable<quint8> clsCardinalityHypothesisContainer::MaxCardinalityContainerSize(
        clsSearchGraphBuilder::moduleBaseconfig() + "/MaxCardinalityContainerSize",
        "TODO Desc",
        100);
tmplConfigurable<quint8> clsCardinalityHypothesisContainer::PrimaryCoverageShare(
        clsSearchGraphBuilder::moduleBaseconfig() + "/PrimaryCoverageShare",
        "TODO Desc",
        0
        );

tmplConfigurable<double> clsCardinalityHypothesisContainer::SearchBeamWidth(
        clsSearchGraphBuilder::moduleBaseconfig() + "/SearchBeamWidth",
        "TODO Desc",
        5
        );

size_t clsCardinalityHypothesisContainer::MaxCardinalitySizeLazyPruning = 0;

clsCardinalityHypothesisContainer::clsCardinalityHypothesisContainer() :
    Data(new clsCardinalityHypothesisContainerData)
{ }

bool clsCardinalityHypothesisContainer::insertNewHypothesis(clsSearchGraphNode &_node)
{
    if(_node.getTotalCost() +
            clsCardinalityHypothesisContainer::SearchBeamWidth.value() <
            this->Data->WorstCostLimit) {
        this->Data->WorstCostLimit = _node.getTotalCost() +
                clsCardinalityHypothesisContainer::SearchBeamWidth.value();
    }

    const Coverage_t& Coverage = _node.coverage();
    clsLexicalHypothesisContainer& Container = this->Data->LexicalHypothesisContainer[Coverage];

    size_t OldContainerSize = Container.nodes().size();

    bool InsertionDone = Container.insertHypothesis(_node);
    if (InsertionDone){
        if (this->Data->BestLexicalHypothesis == NULL || this->Data->WorstLexicalHypothesis == NULL){
            this->Data->BestLexicalHypothesis = &Container;
            this->Data->BestCoverage = Coverage;
            this->Data->WorstLexicalHypothesis = &Container;
            this->Data->WorstCoverage = Coverage;
        } else
            this->pruneAndUpdateBestAndWorstNode(Coverage, Container, _node);
    }

    this->Data->TotalSearchGraphNodeCount += ((qint64)Container.nodes().size() - (qint64)OldContainerSize);

    if(this->Data->TotalSearchGraphNodeCount >
            clsCardinalityHypothesisContainer::MaxCardinalitySizeLazyPruning)
        this->prune();

    return InsertionDone;
}

bool clsCardinalityHypothesisContainer::mustBePruned(Cost_t _cost) const
{
    if (this->Data->BestLexicalHypothesis != NULL) {
        if(_cost > this->Data->WorstCostLimit)
            return true;
    }
    return false;
}

void clsCardinalityHypothesisContainer::updateWorstNode()
{
    Cost_t  WorstCost = -INFINITY;
    CoverageLexicalHypothesisMap_t::Iterator WorstLexIter = this->Data->LexicalHypothesisContainer.end();
    Coverage_t WorstLexCoverage;
    for(CoverageLexicalHypothesisMap_t::Iterator LexIter = this->Data->LexicalHypothesisContainer.begin();
        LexIter != this->Data->LexicalHypothesisContainer.end();
        ++LexIter){
        //TODO Why we have empty container
        Q_ASSERT(LexIter->nodes().size());

        if (LexIter->nodes().size()  && WorstCost < LexIter->nodes().last().getTotalCost()){
            WorstCost = LexIter->nodes().last().getTotalCost();
            WorstLexIter = LexIter;
            WorstLexCoverage = LexIter.key();
        }
    }

    Q_ASSERT(WorstLexIter != this->Data->LexicalHypothesisContainer.end());

    this->Data->WorstLexicalHypothesis = &(*WorstLexIter);
    this->Data->WorstCoverage = WorstLexCoverage;
}

void clsCardinalityHypothesisContainer::pruneAndUpdateBestAndWorstNode(const Coverage_t& _coverage, clsLexicalHypothesisContainer &_container, const clsSearchGraphNode &_node)
{
    if (/* this->Data->BestLexicalHypothesis != NULL && */
            _node.getTotalCost() <
                this->Data->BestLexicalHypothesis->nodes().first().getTotalCost()) {
        this->Data->BestLexicalHypothesis = &_container;
        this->Data->BestCoverage = _coverage;
    }

    if (_node.isRecombined())
        return;

    if (this->isPruningNecessary()){
        Q_ASSERT(this->Data->WorstLexicalHypothesis->nodes().size());
        this->Data->WorstLexicalHypothesis->nodes().removeLast();

        //If these are equal then this will be accounted for in insertNewHypothesis
        if (&_container != this->Data->WorstLexicalHypothesis)
            --this->Data->TotalSearchGraphNodeCount;

        if (this->Data->WorstLexicalHypothesis->nodes().isEmpty())
            this->Data->LexicalHypothesisContainer.remove(this->Data->WorstCoverage);

        this->updateWorstNode();
    }

    if (_node.getTotalCost() > this->Data->WorstLexicalHypothesis->nodes().last().getTotalCost()){
        this->Data->WorstLexicalHypothesis = &_container;
        this->Data->WorstCoverage = _coverage;
    }
}

void clsCardinalityHypothesisContainer::prune()
{
    QHash<Coverage_t, int> PickedHypothesisCount;
    int TotalSearchGraphNodeCount = 0;
    // First perform pruning respecting the primal share of each coverage if needed
    if(clsCardinalityHypothesisContainer::PrimaryCoverageShare.value() != 0) {
        for(auto LexHypoContainerIter = this->Data->LexicalHypothesisContainer.begin();
            LexHypoContainerIter != this->Data->LexicalHypothesisContainer.end();
            ++LexHypoContainerIter) {
            int PickedFromThisCoverage = qMin(
                        (int)clsCardinalityHypothesisContainer::PrimaryCoverageShare.value(),
                        LexHypoContainerIter->nodes().size()
                        );
            PickedHypothesisCount[LexHypoContainerIter.key()] = PickedFromThisCoverage;
            TotalSearchGraphNodeCount += PickedFromThisCoverage;
        }
    } else {
        // Without primal share, nodes of the lexical hypothesis containers will be
        // chosen only based on their costs, so initially we do not choose any nodes
        // from any of these containers
        for(auto LexHypoContainerIter = this->Data->LexicalHypothesisContainer.begin();
            LexHypoContainerIter != this->Data->LexicalHypothesisContainer.end();
            ++LexHypoContainerIter)
            PickedHypothesisCount[LexHypoContainerIter.key()] = 0;
    }
    // Fill up vacant places if there are any
    while(TotalSearchGraphNodeCount <
            clsCardinalityHypothesisContainer::MaxCardinalityContainerSize.value()) {
        Coverage_t ChosenCoverage;
        Cost_t ChosenNodeTotalCost =
                this->Data->BestLexicalHypothesis->nodes().first().getTotalCost() +
                clsCardinalityHypothesisContainer::SearchBeamWidth.value();
        for(auto HypoContainerIter = this->Data->LexicalHypothesisContainer.begin();
            HypoContainerIter != this->Data->LexicalHypothesisContainer.end();
            ++HypoContainerIter) {
            int NextCoverageNodeIndex = PickedHypothesisCount[HypoContainerIter.key()];
            if(NextCoverageNodeIndex >= HypoContainerIter->nodes().size())
                continue;
            const clsSearchGraphNode& Node = HypoContainerIter->nodes().at(NextCoverageNodeIndex);
            if(Node.getTotalCost() < ChosenNodeTotalCost) {
                ChosenCoverage = HypoContainerIter.key();
                ChosenNodeTotalCost = Node.getTotalCost();
            }
        }
        if(ChosenCoverage.count(true) == 0)
            break;
        PickedHypothesisCount[ChosenCoverage]++;
        TotalSearchGraphNodeCount++;
        if(TotalSearchGraphNodeCount ==
                clsCardinalityHypothesisContainer::MaxCardinalityContainerSize.value())
            this->Data->WorstCostLimit = ChosenNodeTotalCost;
    }
    // Now apply the actual pruning
    for(auto LexHypoContainerIter = this->Data->LexicalHypothesisContainer.begin();
        LexHypoContainerIter != this->Data->LexicalHypothesisContainer.end();
        ++LexHypoContainerIter) {
        int PickedFromThisCoverage = PickedHypothesisCount[LexHypoContainerIter.key()];
        QList<clsSearchGraphNode>& Nodes = LexHypoContainerIter->nodes();
        Nodes.erase(Nodes.begin() + PickedFromThisCoverage, Nodes.end());
    }
    // Update best and worst placeholders and the total node count
    this->Data->TotalSearchGraphNodeCount = TotalSearchGraphNodeCount;
    for(auto LexHypoContainerIter = this->Data->LexicalHypothesisContainer.begin();
        LexHypoContainerIter != this->Data->LexicalHypothesisContainer.end();
        ++LexHypoContainerIter) {
        if(LexHypoContainerIter->nodes().first().getTotalCost() <
                this->Data->BestLexicalHypothesis->nodes().first().getTotalCost()) {
            this->Data->BestCoverage = LexHypoContainerIter.key();
            this->Data->BestLexicalHypothesis = &(*LexHypoContainerIter);
        }
        if(LexHypoContainerIter->nodes().last().getTotalCost() >
                this->Data->WorstLexicalHypothesis->nodes().last().getTotalCost()) {
            this->Data->WorstCoverage = LexHypoContainerIter.key();
            this->Data->WorstLexicalHypothesis = &(*LexHypoContainerIter);
        }
    }
}

}
}
}
}

