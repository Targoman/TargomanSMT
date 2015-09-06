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

#include "clsSearchGraphNode.h"
#include "Private/FeatureFunctions/intfFeatureFunction.hpp"

namespace Targoman{
namespace SMT {
namespace Private{
/**
 * @brief Namespace surrounding all classes related to generate, store and manage SearchGraph
 */
namespace SearchGraphBuilder {

using namespace RuleTable;
using namespace Common;

clsSearchGraphNodeData* InvalidSearchGraphNodeData = NULL;
clsSearchGraphNode* pInvalidSearchGraphNode = NULL;
size_t  clsSearchGraphNodeData::RegisteredFeatureFunctionCount;

/**
 * @brief constructor of this class set invalid data to #Data in order to know whether it is used before or not.
 * This funciton also calls initRootNode of every feature function to always have a valid previous node data.
 */
clsSearchGraphNode::clsSearchGraphNode():
    Data(InvalidSearchGraphNodeData)
{
    foreach (FeatureFunction::intfFeatureFunction* FF, gConfigs.ActiveFeatureFunctions)
        FF->initRootNode(*this);
}

/**
 * @brief This constructor of this class set values of #Data and calculates cost of this translation up to now using all feature functions.
 * @param _prevNode         Previous node of this search graph node.
 * @param _startPos         This node has a translation for a phrase of input sentence, start position of this phrase in input sentence is this variable.
 * @param _endPos           This node has a translation for a phrase of input sentence, end position of this phrase in input sentence is this variable.
 * @param _newCoverage      Updated covered for that is translated is stored in this variable.
 * @param _targetRule       Target language phrase translation.
 * @param _isFinal          Has this node covered translation for all word of input sentence.
 * @param _restCost         Approximated cost of rest of translation.
 */
clsSearchGraphNode::clsSearchGraphNode(const clsSearchGraphNode &_prevNode,
                                       quint16 _startPos,
                                       quint16 _endPos,
                                       const Coverage_t &_newCoverage,
                                       const clsTargetRule &_targetRule,
                                       bool _isFinal,
                                       Cost_t _restCost):
    Data(new clsSearchGraphNodeData(
             _prevNode,
             _startPos,
             _endPos,
             _newCoverage,
             _targetRule,
             _isFinal,
             _restCost))
{
    foreach (FeatureFunction::intfFeatureFunction* FF, gConfigs.ActiveFeatureFunctions) {
        Cost_t Cost = FF->scoreSearchGraphNode(*this);
        this->Data->Cost += Cost;
    }
}

clsSearchGraphNode::~clsSearchGraphNode()
{ }

void clsSearchGraphNode::swap(clsSearchGraphNode &_node)
{
    this->Data.swap(_node.Data);
}

template<class Class_t, class Container_t, typename Functor_t>
/**
 * @brief findInsertionPos finds correct place to insert _element in the _container using _comperator to have a sorted list.
 * @return Returns correct place of insertion.
 */
size_t findInsertionPos(const Container_t& _conatiner, const Class_t& _element, Functor_t _comparator){
    for(size_t i = 0; i< _conatiner.size(); ++i){
        if (_comparator(_element, _conatiner.at(i)) > 0)
            return i;
    }
    return _conatiner.size();
}

/**
 * @brief Recombines input node with this node and .
 * @param _node better node for translation that should be replaced with this node.
 */
void clsSearchGraphNode::recombine(clsSearchGraphNode &_node)
{
    Q_ASSERT(this->Data->IsRecombined == false && _node.Data->IsRecombined == false);
    //swaps _node with this instance of node
    if (_node.getTotalCost() < this->getTotalCost()){
        this->swap(_node);
    }
    //now, this node is better node and _node is the worse node.
    auto NodeComparator = [](const clsSearchGraphNode& _firstNode,const clsSearchGraphNode& _secondNode){
        return _secondNode.getTotalCost() - _firstNode.getTotalCost();
    };

    //inserts worse node to CombinedNodes container of this node.
    this->Data->CombinedNodes.insert(
                findInsertionPos(this->Data->CombinedNodes, _node, NodeComparator),
                _node);
    // Now we want to add recombined nodes of worse node to better node.
    // We do not need to find correct places of insertion for recombined nodes because better node is a new node
    // and dosn't have any combined node so far. we just add recombined node of worse node to this new node.

    ///@todo CombinedNodes QList can be swapped instead if appended and cleared because one of the QLists is surely empty
    this->Data->CombinedNodes.append(_node.Data->CombinedNodes);
    _node.Data->CombinedNodes.clear();
    _node.Data->IsRecombined = true;
}

/**
 * @brief determines whether this node and input _node expands same future nodes or not.
 * @param _node input node.
 */
bool clsSearchGraphNode::haveSameFuture(const clsSearchGraphNode &_node) const
{
    if (this->sourceRangeEnd() != _node.sourceRangeEnd())
        return false;
    if (Q_UNLIKELY(_node.coverage() != this->coverage()))
        return false;
    for(auto FeatureFunctionIter = gConfigs.ActiveFeatureFunctions.constBegin();
        FeatureFunctionIter != gConfigs.ActiveFeatureFunctions.constEnd();  ++FeatureFunctionIter)
        if (FeatureFunctionIter.value()->nodesHaveSameState(*this, _node) == false)
            return false;

    return true;
}

bool operator < (const clsSearchGraphNode &_first, const clsSearchGraphNode &_second)
{
    if(_first.sourceRangeEnd() < _second.sourceRangeEnd())
        return true;
    else if(_first.sourceRangeEnd() > _second.sourceRangeEnd())
        return false;
    for(auto FeatureIter = gConfigs.ActiveFeatureFunctions.constBegin();
        FeatureIter != gConfigs.ActiveFeatureFunctions.constEnd(); ++FeatureIter) {
        int ComparisonResult = FeatureIter.value()->compareStates(_first, _second);
        if(ComparisonResult < 0)
            return true;
        else if(ComparisonResult > 0)
            return false;
    }
    return false;
}

/**
 * @brief allocates an index for feature functions.
 * @return Returns allocated index.
 */
size_t clsSearchGraphNode::allocateFeatureFunctionData()
{
    size_t AllocatedIndex = clsSearchGraphNodeData::RegisteredFeatureFunctionCount;
    ++clsSearchGraphNodeData::RegisteredFeatureFunctionCount;
    return AllocatedIndex;

}


}
}
}
}
