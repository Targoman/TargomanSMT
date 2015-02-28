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
namespace SMT {
namespace Private{
namespace SearchGraph {

using namespace Common;
using namespace Common::Configuration;

const Cost_t PBT_LEXICAL_HYPOTHESIS_CONTAINER_EMPTY_BEST = 1e10;
tmplConfigurable<quint8> clsLexicalHypothesisContainer::LexicalMaxHistogramSize(
        clsSearchGraphBuilder::moduleBaseconfig() + "/LexicalMaxHistogramSize",
        "TODO Desc",
        100
        );

Targoman::Common::Configuration::tmplConfigurable<bool> clsLexicalHypothesisContainer::KeepRecombined(
        clsSearchGraphBuilder::moduleBaseconfig() + "/KeepRecombined",
        "Do recombination(default) or let nodes to be separated",
        true
        );

clsLexicalHypothesisContainer::clsLexicalHypothesisContainer() :
    Data(new clsLexicalHypothesisContainerData)
{}

/**
 * @brief Checks whether new hypothesis with cost of _totalCost should be pruned or not.
 *
 * If list is not empty and max size of list (#LexicalMaxHistogramSize) is not zero and If size of list of nodes is greater than #LexicalMaxHistogramSize and cost of new hypothesis
 * (input cost) is greater than worse node it should pruned.
 * @param _totalCost
 * @return
 */
bool clsLexicalHypothesisContainer::mustBePruned(Cost_t _totalCost)
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

/**
 * @brief returns cost of best node.
 */
Cost_t clsLexicalHypothesisContainer::getBestCost() const
{
    if (this->Data->Nodes.isEmpty())
        return PBT_LEXICAL_HYPOTHESIS_CONTAINER_EMPTY_BEST;

    return this->bestNode().getCost();
}


/**
 * @brief first checks whether input node should be recombined with another existing node,
 * if not it inserts new node in list of nodes in a sorted order.
 * So our list is always sorted by node costs from less cost to highest cost.
 *
 * @param _node
 * @return
 */
bool clsLexicalHypothesisContainer::insertHypothesis(clsSearchGraphNode& _node)
{
    size_t InsertionPos = this->Data->Nodes.size();
    for (size_t i=0; i<(size_t)this->Data->Nodes.size(); ++i) {
        clsSearchGraphNode& HypoNode = this->Data->Nodes[i];
        if (HypoNode.haveSameFuture(_node)){
            if (clsLexicalHypothesisContainer::KeepRecombined.value()){
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

/**
 * @brief it will be used in the last cardinality.
 * Because every node has same coverage (full coverage) and no other word is remained,
 * so all of them has same future and should be recombined with best node.
 */
void clsLexicalHypothesisContainer::finalizeRecombination()
{
    if(clsLexicalHypothesisContainer::KeepRecombined.value() == false)
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
