/******************************************************************************
 * Targoman: A robust Statistical Machine Translation framework               *
 *                                                                            *
 * Copyright 2014-2015 by ITRC <http://itrc.ac.ir>                            *
 *                                                                            *
 * This file is part of Targoman.                                             *
 *                                                                            *
 * Targoman is free software: you can redistribute it and/or modify           *
 * it under the terms of the GNU Lesser General Public License as published   *
 * by the Free Software Foundation, either version 3 of the License, or       *
 * (at your option) any later version.                                        *
 *                                                                            *
 * Targoman is distributed in the hope that it will be useful,                *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of             *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              *
 * GNU Lesser General Public License for more details.                        *
 * You should have received a copy of the GNU Lesser General Public License   *
 * along with Targoman. If not, see <http://www.gnu.org/licenses/>.           *
 *                                                                            *
 ******************************************************************************/
/**
 * @author S. Mohammad M. Ziabary <ziabary@targoman.com>
 * @author Behrooz Vedadian <vedadian@targoman.com>
 * @author Saeed Torabzadeh <saeed.torabzadeh@targoman.com>
 */

#include "clsLexicalHypothesis.h"
#include "clsSearchGraph.h"
#include "libTargomanCommon/FastOperations.hpp"
#include <functional>

namespace Targoman{
namespace SMT {
namespace Private{
namespace SearchGraphBuilder {

using namespace Common;
using namespace Common::Configuration;

const Cost_t PBT_LEXICAL_HYPOTHESIS_CONTAINER_EMPTY_BEST = INFINITY;

Targoman::Common::Configuration::tmplConfigurable<bool> clsLexicalHypothesisContainer::KeepRecombined(
        MAKE_CONFIG_PATH("KeepRecombined"),
        "Do recombination(default) or let nodes to be separated",
        true
        );

clsLexicalHypothesisContainer::clsLexicalHypothesisContainer() :
    Data(new clsLexicalHypothesisContainerData())
{}


/**
 * @brief returns cost of best node.
 */
Cost_t clsLexicalHypothesisContainer::getBestCost() const
{
    if (this->Data->Nodes.isEmpty())
        return PBT_LEXICAL_HYPOTHESIS_CONTAINER_EMPTY_BEST;
    return this->Data->Nodes.bestNode().getTotalCost();
}

/**
 * @brief returns cost of worst node.
 */
Cost_t clsLexicalHypothesisContainer::getWorstCost() const
{
    if (this->Data->Nodes.isEmpty())
        return PBT_LEXICAL_HYPOTHESIS_CONTAINER_EMPTY_BEST;
    return this->Data->Nodes.worstNode().getTotalCost();
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
    bool InsertionResult = this->Data->Nodes.insert(_node, clsLexicalHypothesisContainer::KeepRecombined.value());
    return InsertionResult;
//    QPair<clsSearchGraphNode, bool> InsertionResult = this->Data->Nodes.insert(_node);
//    if(InsertionResult.second == false) {
//        if(clsLexicalHypothesisContainer::KeepRecombined.value())
//            InsertionResult.first.recombine(_node);
//        else if(InsertionResult.first.getTotalCost() > _node.getTotalCost())
//            InsertionResult.first.swap(_node);
//        return false;
//    }
//    return true;
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

    clsLexicalHypoNodeSet::iterator BestNodeIter = this->Data->Nodes.begin();
    clsLexicalHypoNodeSet::iterator NodeIter = this->Data->Nodes.begin() + 1;
    while(NodeIter != this->Data->Nodes.end()) {
        BestNodeIter->recombine(*NodeIter);
        NodeIter = this->Data->Nodes.erase(NodeIter);
    }
}

#ifdef TARGOMAN_SHOW_DEBUG
const clsSearchGraphNode &clsLexicalHypothesisContainer::FindNode(const char *_targetRuleStr, const char *_prevTargetRuleStr, const char *_coverage) const
{
//      Q_UNUSED(FindNode)
    for(auto NodeIter = this->Data->Nodes.begin(); NodeIter != this->Data->Nodes.end(); ++NodeIter) {
        if(isDesiredNode(*NodeIter, _targetRuleStr, _prevTargetRuleStr, _coverage))
            return *NodeIter;
    }
    static clsSearchGraphNode InvalidSearchGraphNode;
    return InvalidSearchGraphNode;
}
#endif

QString clsLexicalHypothesisContainer::moduleName()
{
    return clsSearchGraph::moduleName();
}


}
}
}
}
