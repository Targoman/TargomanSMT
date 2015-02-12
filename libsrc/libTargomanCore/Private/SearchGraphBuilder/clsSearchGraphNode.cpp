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
namespace Core {
namespace Private{
namespace SearchGraphBuilder {

using namespace RuleTable;


clsSearchGraphNode::clsSearchGraphNode(const clsSearchGraphNode _prevNode,
                                       const clsTargetRule &_targetRule,
                                       Common::Cost_t _cost,
                                       Common::Cost_t _reorderingJumpCost,
                                       Common::Cost_t _restCost,
                                       Common::Cost_t _lmCost,
                                       quint8 _startPos,
                                       quint8 _endPos,
                                       const Coverage_t &_newCoverage,
                                       bool _isFinal,
                                       LanguageModel::intfLMSentenceScorer *_lmscorer) :
    Data(new clsSearchGraphNodeData(
             _prevNode,
             _targetRule,
             _cost,
             _reorderingJumpCost,
             _restCost,
             _lmCost,
             _startPos,
             _endPos,
             _newCoverage,
             _isFinal,
             _lmscorer))
{

    //    firstBestHyperEdge_ = 0;//TODO

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
    if (_node.getTotalCost() < this->getTotalCost()){
        this->Data.swap(_node.Data);
    }

    auto NodeComparator = [](const clsSearchGraphNode& _firstNode,const clsSearchGraphNode& _secondNode){
        return _secondNode.getTotalCost() - _firstNode.getTotalCost();
    };

    this->Data->CombinedNodes.insert(findInsertionPos(this->Data->CombinedNodes, _node, NodeComparator),
                                     _node);

    foreach(const clsSearchGraphNode& Node, _node.Data->CombinedNodes){
        int InsertionPos = findInsertionPos(this->Data->CombinedNodes, _node, NodeComparator);
        for (int i = InsertionPos; i>=0; --i){
            if (_node.Data == Node.Data){
                InsertionPos = -1;
                break;
            }
        }
        if (InsertionPos >= 0)
            this->Data->CombinedNodes.insert(InsertionPos, Node);
    }
    _node.Data->CombinedNodes.clear();
    _node.Data->IsRecombined = true;
}

bool clsSearchGraphNode::haveSameFuture(const clsSearchGraphNode &_node) const
{

    if (this->sourceRangeEnd() != _node.sourceRangeEnd() ||
            this->lmScorer().haveSameHistoryAs(_node.lmScorer()) == false)
        return false;

    foreach(FeatureFunction::intfFeatureFunction* FF, gConfigs.ActiveFeatureFunctions.values())
        if (FF->nodesHaveSameState(*this, _node) == false)
            return false;

    if (Q_UNLIKELY(_node.coverage() != this->coverage()))
        return false;

    return true;
}

/*****************************************************************************************************/
clsSearchGraphNodeData::~clsSearchGraphNodeData()
{
    //Just to Suppress compiler error on QScoppedPointer
}

}
}
}
}
