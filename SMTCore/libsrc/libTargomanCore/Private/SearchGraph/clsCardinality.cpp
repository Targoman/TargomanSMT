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
namespace Core {
namespace Private{
namespace SearchGraph {

using namespace Common;
using namespace Common::Configuration;

tmplConfigurable<quint8> clsCardinalityHypothesisContainer::ReorderingHistogramSize(
        clsSearchGraphBuilder::moduleBaseconfig() + "/ReorderingHistogramSize",
        "TODO Desc",
        100);

clsCardinalityHypothesisContainer::clsCardinalityHypothesisContainer() :
    Data(new clsCardinalityHypothesisContainerData)
{}

void clsCardinalityHypothesisContainer::dump(const QString& _prefix)
{
    for(CoverageLexicalHypothesisMap_t::ConstIterator Iter = this->Data->LexicalHypothesisContainer.begin();
        Iter != this->Data->LexicalHypothesisContainer.end();
        ++Iter){
        for(int i=0; i<Iter.value().nodes().size(); ++i){
            clsSearchGraphNode Node(Iter.value().nodes().at(i));
            QString TargetPhrase;
            for(size_t j=0; j <Node.targetRule().size(); ++j)
                TargetPhrase += gConfigs.EmptyLMScorer->getWordByIndex(Node.targetRule().at(j));

            std::cout<<(_prefix +
                        QString("Cov[%1] Node[%2] Cost[%3] RestCost[%4] : ").arg(
                            bitArray2Str(Iter.key())).arg(
                            i).arg(
                            Node.getCost()).arg(
                            Node.getTotalCost() - Iter.value().nodes().at(i).getCost())
                        + TargetPhrase).toUtf8().constData()<<std::endl;
        }
    }

}

bool clsCardinalityHypothesisContainer::insertNewHypothesis(const Coverage_t &_coverage, clsLexicalHypothesisContainer &_container, clsSearchGraphNode &_node)
{

    size_t OldContainerSize = _container.nodes().size();

    bool InsertionDone = _container.insertHypothesis(_node);
    if (InsertionDone){
        if (this->Data->WorstLexicalHypothesis == NULL){
            this->Data->WorstLexicalHypothesis = &_container;
            this->Data->WorstCoverage = _coverage;
        }else
            this->pruneAndUpdateWorstNode(_coverage, _container, _node);
    }

    this->Data->TotalSearchGraphNodeCount += ((qint64)_container.nodes().size() - (qint64)OldContainerSize);

    return InsertionDone;
}

bool clsCardinalityHypothesisContainer::mustBePruned(Cost_t _cost) const
{
    if (clsCardinalityHypothesisContainer::ReorderingHistogramSize.value() ||
        this->Data->TotalSearchGraphNodeCount < clsCardinalityHypothesisContainer::ReorderingHistogramSize.value())
        return false;

    if (_cost > this->Data->WorstLexicalHypothesis->nodes().last().getTotalCost())
        return true;

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

void clsCardinalityHypothesisContainer::pruneAndUpdateWorstNode(const Coverage_t& _coverage, clsLexicalHypothesisContainer &_lexicalHypo, const clsSearchGraphNode &_node)
{
    if (_node.isRecombined())
        return;

    if (clsCardinalityHypothesisContainer::ReorderingHistogramSize.value() &&
        this->Data->TotalSearchGraphNodeCount > clsCardinalityHypothesisContainer::ReorderingHistogramSize.value()){

        Q_ASSERT(this->Data->WorstLexicalHypothesis->nodes().size());
        this->Data->WorstLexicalHypothesis->nodes().removeLast();

        //If these are equal then this will be accounted for in insertNewHypothesis
        if (&_lexicalHypo != this->Data->WorstLexicalHypothesis)
            --this->Data->TotalSearchGraphNodeCount;

        if (this->Data->WorstLexicalHypothesis->nodes().isEmpty())
            this->Data->LexicalHypothesisContainer.remove(this->Data->WorstCoverage);

        this->updateWorstNode();
    }

    if (_node.getTotalCost() > this->Data->WorstLexicalHypothesis->nodes().last().getTotalCost()){
        this->Data->WorstLexicalHypothesis = &_lexicalHypo;
        this->Data->WorstCoverage = _coverage;
    }
}

}
}
}
}

