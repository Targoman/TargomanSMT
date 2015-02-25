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
namespace SearchGraphBuilder {

using namespace Common;
using namespace Common::Configuration;

tmplConfigurable<quint8> clsCardinality::ReorderingHistogramSize(
        clsSearchGraphBuilder::moduleBaseconfig() + "/ReorderingHistogramSize",
        "TODO Desc",
        100);

clsCardinality::clsCardinality() :
    Data(new clsCardinalityData)
{}

void clsCardinality::dump(const QString& _prefix)
{
    for(LexicalHypothesisContainer_t::ConstIterator Iter = this->Data->LexicalHypothesisContainer.begin();
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

void clsCardinality::insertNewHypothesis(const Coverage_t &_coverage, clsLexicalHypothesis &_container, clsSearchGraphNode &_node)
{
    size_t OldContainerSize = _container.nodes().size();

    if (_container.insertHypothesis(_node)){
        if (this->Data->WorstLexicalHypothesis == NULL){
            this->Data->WorstLexicalHypothesis = &_container;
            this->Data->WorstCoverage = _coverage;
        }else
            this->pruneAndUpdateWorstNode(_coverage, _container, _node);
    }

    Cost_t LastCost = -INFINITY;
    for(int i=0; i< _container.nodes().size(); ++i){
        if (_container.nodes().at(i).getTotalCost() < LastCost){
            throw exConfiguration("List is not sorted" +
                                  QString::number(_container.nodes().at(i).getTotalCost()) + " vs " + QString::number(LastCost));

        }
        LastCost = _container.nodes().at(i).getTotalCost();
    }

    this->Data->TotalSearchGraphNodeCount += ((qint64)_container.nodes().size() - (qint64)OldContainerSize);
}

bool clsCardinality::mustBePruned(Cost_t _cost) const
{
    if (clsCardinality::ReorderingHistogramSize.value() ||
        this->Data->TotalSearchGraphNodeCount < clsCardinality::ReorderingHistogramSize.value())
        return false;

    if (_cost > this->Data->WorstLexicalHypothesis->nodes().last().getTotalCost())
        return true;

    return false;
}

void clsCardinality::updateWorstNode()
{
    Cost_t  WorstCost = -INFINITY;
    LexicalHypothesisContainer_t::Iterator WorstLexIter = this->Data->LexicalHypothesisContainer.end();
    Coverage_t WorstLexCoverage;
    for(LexicalHypothesisContainer_t::Iterator LexIter = this->Data->LexicalHypothesisContainer.begin();
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

void clsCardinality::pruneAndUpdateWorstNode(const Coverage_t& _coverage, clsLexicalHypothesis &_lexicalHypo, const clsSearchGraphNode &_node)
{
    if (_node.isRecombined())
        return;

    if (clsCardinality::ReorderingHistogramSize.value() &&
        this->Data->TotalSearchGraphNodeCount > clsCardinality::ReorderingHistogramSize.value()){

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


bool operator<(const QBitArray &a, const QBitArray &b)
{
    Q_ASSERT(a.size() == b.size());
    for(int i = 0; i < a.size(); ++i)
        if(a[i] < b[i])
            return false;
        else if(b[i] < a[i])
            return true;
    return false;
}
