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

const Cost_t PBT_LEXICAL_HYPOTHESIS_CONTAINER_EMPTY_BEST = INFINITY;

Targoman::Common::Configuration::tmplConfigurable<bool> clsLexicalHypothesisContainer::KeepRecombined(
        clsSearchGraphBuilder::moduleBaseconfig() + "/KeepRecombined",
        "Do recombination(default) or let nodes to be separated",
        true
        );

clsLexicalHypothesisContainer::clsLexicalHypothesisContainer() :
    Data(new clsLexicalHypothesisContainerData)
{}

/**
 * @brief returns cost of best node.
 */
Cost_t clsLexicalHypothesisContainer::getBestCost() const
{
    if (this->Data->Nodes.isEmpty())
        return PBT_LEXICAL_HYPOTHESIS_CONTAINER_EMPTY_BEST;

    return this->nodes().first().getCost();
}


/**
 * @brief first checks whether input node should be recombined with another existing node,
 * if not it inserts new node in list of nodes in a sorted order.
 * So our list is always sorted by node costs from less cost to highest cost.
 *
 * @param _node
 * @return  returns true if the new node occupies a new place in this lexical hypothesis container and false otherwise
 */
bool clsLexicalHypothesisContainer::insertHypothesis(clsSearchGraphNode& _node)
{
    size_t InsertionPos = this->Data->Nodes.size();
    for (size_t i=0; i<(size_t)this->Data->Nodes.size(); ++i) {
        // NOTE: Do not change this to get by reference, it will break the algorithm
        clsSearchGraphNode HypoNode = this->Data->Nodes[i];
        if (HypoNode.haveSameFuture(_node)){
            if (clsLexicalHypothesisContainer::KeepRecombined.value()){
                HypoNode.recombine(_node);
                // Find the new position for possibly changed HypoNode
                int NewHypoNodePosition = i;
                while(NewHypoNodePosition > 0 && HypoNode.getTotalCost() <
                      this->Data->Nodes.at(NewHypoNodePosition - 1).getTotalCost()) {
                    this->Data->Nodes[NewHypoNodePosition] = this->Data->Nodes[NewHypoNodePosition - 1];
                    --NewHypoNodePosition;
                }
                this->Data->Nodes[NewHypoNodePosition] = HypoNode;
                return false;
            }else{
                if(HypoNode.getTotalCost() > _node.getTotalCost()){
                    this->Data->Nodes.removeAt(i);
                    InsertionPos = i;
                    while(InsertionPos > 0 && _node.getTotalCost() < this->Data->Nodes.at(InsertionPos - 1).getTotalCost())
                        InsertionPos--;
                    break;
                }else
                    return false;
            }
        } else if ((int)InsertionPos == this->Data->Nodes.size() && HypoNode.getTotalCost() > _node.getTotalCost()){
            InsertionPos = i;
        }
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

#ifdef TARGOMAN_SHOW_DEBUG
const clsSearchGraphNode &clsLexicalHypothesisContainer::FindNode(const char *_targetRuleStr, const char *_prevTargetRuleStr, const char *_coverage) const
{
    for(int i = 0; i < this->Data->Nodes.size(); ++i) {
        if(isDesiredNode(this->Data->Nodes.at(i), _targetRuleStr, _prevTargetRuleStr, _coverage))
            return this->Data->Nodes.at(i);
    }
    static clsSearchGraphNode InvalidSearchGraphNode;
    return InvalidSearchGraphNode;
}
#endif

}
}
}
}
