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

#include "clsSearchGraphNode.h"
#include "Private/FeatureFunctions/intfFeatureFunction.hpp"

namespace Targoman{
namespace SMT {
namespace Private{
/**
 * @brief Namespace surrounding all classes related to generate, store and manage SearchGraph
 */
namespace SearchGraph {

using namespace RuleTable;
using namespace Common;

clsSearchGraphNodeData* InvalidSearchGraphNodeData = NULL;
clsSearchGraphNode* pInvalidSearchGraphNode = NULL;
size_t  clsSearchGraphNodeData::RegisteredFeatureFunctionCount;

clsSearchGraphNode::clsSearchGraphNode():
    Data(InvalidSearchGraphNodeData)
{
    foreach (FeatureFunction::intfFeatureFunction* FF, gConfigs.ActiveFeatureFunctions)
        FF->initRootNode(*this);
}

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
    foreach (FeatureFunction::intfFeatureFunction* FF, gConfigs.ActiveFeatureFunctions)
        this->Data->Cost += FF->scoreSearchGraphNode(*this);
}

template<class Class_t, class Container_t, typename Functor_t>
size_t findInsertionPos(const Container_t& _conatiner, const Class_t& _element, Functor_t _comparator){
    for(size_t i = 0; i< _conatiner.size(); ++i){
        if (_comparator(_element, _conatiner.at(i)) > 0)
            return i;
    }
    return _conatiner.size();
}


void clsSearchGraphNode::recombine(clsSearchGraphNode &_node)
{
    Q_ASSERT(this->Data->IsRecombined == false && _node.Data->IsRecombined == false);

    if (_node.getTotalCost() < this->getTotalCost()){
        this->Data.swap(_node.Data);
    }

    auto NodeComparator = [](const clsSearchGraphNode& _firstNode,const clsSearchGraphNode& _secondNode){
        return _secondNode.getTotalCost() - _firstNode.getTotalCost();
    };

    this->Data->CombinedNodes.insert(
                findInsertionPos(this->Data->CombinedNodes, _node, NodeComparator),
                _node);
    // We do not need to find ...
    /*
    foreach(const clsSearchGraphNode& Node, _node.Data->CombinedNodes)
        this->Data->CombinedNodes.insert(
                findInsertionPos(this->Data->CombinedNodes, Node, NodeComparator),
                Node);
    */
    // TODO CombinedNodes QList can be swapped instead if appended and cleared because one of the QLists is surely empty
    this->Data->CombinedNodes.append(_node.Data->CombinedNodes);
    _node.Data->CombinedNodes.clear();
    _node.Data->IsRecombined = true;
}

bool clsSearchGraphNode::haveSameFuture(const clsSearchGraphNode &_node) const
{
    if (this->sourceRangeEnd() != _node.sourceRangeEnd())
        return false;

    foreach(FeatureFunction::intfFeatureFunction* FF, gConfigs.ActiveFeatureFunctions.values())
        if (FF->nodesHaveSameState(*this, _node) == false)
            return false;

    if (Q_UNLIKELY(_node.coverage() != this->coverage()))
        return false;

    return true;
}

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
