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

#include "clsLexicalHypothesis.h"
#include "clsSearchGraphBuilder.h"

namespace Targoman{
namespace Core {
namespace Private{
namespace SearchGraphBuilder {

using namespace Common;
using namespace Common::Configuration;

const Cost_t PBT_LEXICAL_HYPOTHESIS_CONTAINER_EMPTY_BEST = 1e10;
tmplConfigurable<quint8> clsLexicalHypothesis::LexicalMaxHistogramSize(
        clsSearchGraphBuilder::moduleBaseconfig() + "/LexicalMaxHistogramSize",
        "TODO Desc",
        100
        );

Targoman::Common::Configuration::tmplConfigurable<bool> clsLexicalHypothesis::KeepRecombined(
        clsSearchGraphBuilder::moduleBaseconfig() + "/KeepRecombined",
        "Do recombination(default) or let nodes to be separated",
        true
        );

clsLexicalHypothesis::clsLexicalHypothesis() :
    Data(new clsLexicalHypothesisData)
{}

bool clsLexicalHypothesis::mustBePruned(Cost_t _totalCost)
{
    if (this->Data->Nodes.isEmpty())
        return false;
    if(this->LexicalMaxHistogramSize.value() == 0)
        return false;
    if(this->Data->Nodes.size() < this->LexicalMaxHistogramSize.value())
        return false;
    if (_totalCost < this->Data->Nodes.last().getTotalCost())
        return false;

    return true;
}

Cost_t clsLexicalHypothesis::getBestCost() const
{
    if (this->Data->Nodes.isEmpty())
        return PBT_LEXICAL_HYPOTHESIS_CONTAINER_EMPTY_BEST;

    return this->bestNode().getCost();
}

bool clsLexicalHypothesis::insertHypothesis(clsSearchGraphNode& _node)
{
    size_t InsertionPos = this->Data->Nodes.size();
    for (size_t i=0; i<(size_t)this->Data->Nodes.size(); ++i) {
        clsSearchGraphNode& HypoNode = this->Data->Nodes[i];
        if (HypoNode.haveSameFuture(_node)){
            if (clsLexicalHypothesis::KeepRecombined.value()){
                HypoNode.recombine(_node);
                return true;
            }else{
                if(HypoNode.getTotalCost() > _node.getTotalCost()){
                    this->Data->Nodes.removeAt(i);
                    InsertionPos = i;
                    break;
                }else
                    return false;
            }
        }else if(HypoNode.getTotalCost() > _node.getTotalCost()){
            InsertionPos = i;
            break;
        }
    }

    if (this->LexicalMaxHistogramSize.value() > 0 &&
            this->Data->Nodes.size() > this->LexicalMaxHistogramSize.value()){
        if (InsertionPos >= (size_t)this->Data->Nodes.size())
            return false;

        this->Data->Nodes.takeLast();
    }

    this->Data->Nodes.insert(InsertionPos,_node);
    return true;
}

void clsLexicalHypothesis::finalizeRecombination()
{
    if(clsLexicalHypothesis::KeepRecombined.value() == false)
        return;
    while (this->nodes().size() > 1) {
        this->Data->Nodes[0].recombine(this->Data->Nodes[1]);
        this->Data->Nodes.removeAt(1);
    }
}

}
}
}
}
