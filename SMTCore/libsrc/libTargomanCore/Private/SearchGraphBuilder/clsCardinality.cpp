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

namespace Targoman{
namespace Core {
namespace Private{
namespace SearchGraphBuilder {

using namespace Common;
using namespace Common::Configuration;

Coverage_t clsCardinality::EmptyCoverage;
tmplConfigurable<quint8> clsCardinality::ReorderingHistogramSize(
        "TODO",
        "TODO");

clsCardinality::clsCardinality() :
    Data(new clsCardinalityData)
{
}

bool clsCardinality::mustBePruned(Cost_t _cost)
{
    if (clsCardinality::ReorderingHistogramSize.value() ||
        this->Data->TotalSearchGraphNodeCount < clsCardinality::ReorderingHistogramSize.value())
        return false;

    if (_cost > this->Data->WorstNode.Cost)
        return true;

    return false;
}

void clsCardinality::updateWorstNode(const Coverage_t &_coverage, const clsSearchGraphNode &_node)
{
    if (_node.isRecombined())
        return;

    if (clsCardinality::ReorderingHistogramSize.value() &&
        this->Data->TotalSearchGraphNodeCount >= clsCardinality::ReorderingHistogramSize.value()){
        this->Data->WorstNode.ConatinerIter->nodes().erase(this->Data->WorstNode.NodeIter);

        if (this->Data->WorstNode.ConatinerIter->nodes().isEmpty())
            this->Data->LexicalHypothesisContainer.erase(this->Data->WorstNode.ConatinerIter);

        Cost_t  WorstCost = 0;
        LexicalHypothesisContainer_t::Iterator WorstLexIter;
        QList<clsSearchGraphNode>::Iterator    WorstNodeIter;
        for(LexicalHypothesisContainer_t::Iterator LexIter = this->Data->LexicalHypothesisContainer.begin();
            LexIter != this->Data->LexicalHypothesisContainer.end();
            ++LexIter){
            for(QList<clsSearchGraphNode>::Iterator NodeIter = LexIter->nodes().begin();
                NodeIter != LexIter->nodes().end();
                ++NodeIter){
                if (WorstCost < NodeIter->getTotalCost()){
                    WorstCost = NodeIter->getTotalCost();
                    WorstLexIter = LexIter;
                    WorstNodeIter = NodeIter;
                }
            }
        }

        this->Data->WorstNode.ConatinerIter = WorstLexIter;
        this->Data->WorstNode.NodeIter = WorstNodeIter;
        this->Data->WorstNode.Cost = WorstCost;
    }

    if (_node.getTotalCost() > this->Data->WorstNode.Cost){
        this->Data->WorstNode.ConatinerIter = this->Data->LexicalHypothesisContainer.find(_coverage);
        //Worst node is placed at end of list as we are inserting them sorted
        this->Data->WorstNode.NodeIter = this->Data->WorstNode.ConatinerIter->nodes().end() - 1;
        this->Data->WorstNode.Cost = _node.getTotalCost();
    }
}

}
}
}
}
