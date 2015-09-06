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

#ifndef TARGOMAN_CORE_PRIVATE_SEARCHGRAPHBUILDER_CLSLEXICALHYPOTHESIS_H
#define TARGOMAN_CORE_PRIVATE_SEARCHGRAPHBUILDER_CLSLEXICALHYPOTHESIS_H

#include <QList>
#include "clsSearchGraphNode.h"
#include "libTargomanCommon/Types.h"
#include "libTargomanCommon/Configuration/tmplConfigurable.h"
#include <set>

namespace Targoman{
namespace SMT {
namespace Private{
namespace SearchGraphBuilder {

class clsSearchGraphNodeContainerData : public QSharedData {
public:
    clsSearchGraphNodeContainerData() {}
    clsSearchGraphNodeContainerData(const clsSearchGraphNodeContainerData& _other) :
        QSharedData(_other),
        SortedByTotalCost(_other.SortedByTotalCost),
        SortedByState(_other.SortedByState),
        BestNode(_other.BestNode),
        WorstNode(_other.WorstNode),
        KeepRecombined(_other.KeepRecombined)
    { }
    clsSearchGraphNodeContainerData(clsSearchGraphNode _bestNode,
                                    clsSearchGraphNode _worstNode,
                                    bool _keepRecombined) :
        BestNode(_bestNode),
        WorstNode(_worstNode),
        KeepRecombined(_keepRecombined)
    { }
    ~clsSearchGraphNodeContainerData() {}


    QList<clsSearchGraphNode> SortedByTotalCost;
    std::set<clsSearchGraphNode> SortedByState;
    clsSearchGraphNode BestNode;
    clsSearchGraphNode WorstNode;
    bool KeepRecombined;
};

class clsSearchGraphNodeContainer {

public:
    typedef typename QList<clsSearchGraphNode>::iterator iterator;
    typedef typename QList<clsSearchGraphNode>::const_iterator const_iterator;

private:
    void populateSortedByTotalCost() {
        Q_ASSERT(this->Data->SortedByTotalCost.isEmpty());
        for(auto Iter = this->Data->SortedByState.begin();
            Iter != this->Data->SortedByState.end(); ++Iter) {
            const clsSearchGraphNode& Node = *Iter;
            int Pos = findInsertionPos(this->Data->SortedByTotalCost,
                                       Node,
                                       [] (const clsSearchGraphNode& _first, const clsSearchGraphNode& _second) {
                return _second.getTotalCost() - _first.getTotalCost();
            });
//            int Pos = 0;
//            while(Pos < this->Data->SortedByTotalCost.size()) {
//                if(this->Data->SortedByTotalCost.at(Pos).getTotalCost() >= Node.getTotalCost())
//                    break;
//                ++Pos;
//            }
            this->Data->SortedByTotalCost.insert(Pos, Node);
        }
    }

public:
    clsSearchGraphNodeContainer(bool _keepRecombined) :
        Data(new clsSearchGraphNodeContainerData(
                 *pInvalidSearchGraphNode,
                 *pInvalidSearchGraphNode,
                 _keepRecombined
                 )
             )
    { }

    clsSearchGraphNodeContainer(const clsSearchGraphNodeContainer& _other) :
        Data(_other.Data)
    { }

    ~clsSearchGraphNodeContainer() { }

    const clsSearchGraphNode& at(int index) const {
        if(this->Data->SortedByTotalCost.isEmpty()) {
            clsSearchGraphNodeContainer& This = const_cast<clsSearchGraphNodeContainer&>(*this);
            This.populateSortedByTotalCost();
        }
        return this->Data->SortedByTotalCost.at(index);
    }

    bool insert(clsSearchGraphNode& _node) {
        auto SetInsertionResult = this->Data->SortedByState.insert(_node);
        if(SetInsertionResult.second == false) {
            clsSearchGraphNode& Node = const_cast<clsSearchGraphNode&>(*SetInsertionResult.first);
            Q_ASSERT(_node.haveSameFuture(*SetInsertionResult.first));
            if(this->Data->KeepRecombined) {
                if(Node.getTotalCost() > _node.getTotalCost())
                    this->Data->SortedByTotalCost.clear();
                Node.recombine(_node);
            } else if(Node.getTotalCost() > _node.getTotalCost()) {
                this->Data->SortedByTotalCost.clear();
                Node.swap(_node);
            }
            if(Node.getTotalCost() < this->Data->BestNode.getTotalCost())
                this->Data->BestNode = Node;
            if(Node.getTotalCost() > this->Data->WorstNode.getTotalCost())
                this->Data->WorstNode = Node;
            return false;
        }
        this->Data->SortedByTotalCost.clear();
        if(this->Data->BestNode.isInvalid() || _node.getTotalCost() < this->Data->BestNode.getTotalCost())
            this->Data->BestNode = _node;
        if(this->Data->WorstNode.isInvalid() || _node.getTotalCost() > this->Data->WorstNode.getTotalCost())
            this->Data->WorstNode = _node;
        return true;
    }

    iterator begin() {
        if(this->Data->SortedByTotalCost.isEmpty())
            this->populateSortedByTotalCost();
        return this->Data->SortedByTotalCost.begin();
    }

    iterator end() {
        if(this->Data->SortedByTotalCost.isEmpty())
            this->populateSortedByTotalCost();
        return this->Data->SortedByTotalCost.end();
    }

    const_iterator begin() const {
        return this->constBegin();
    }

    const_iterator end() const {
        return this->constEnd();
    }

    const_iterator constBegin() const {
        if(this->Data->SortedByTotalCost.isEmpty()) {
            clsSearchGraphNodeContainer& This = const_cast<clsSearchGraphNodeContainer&>(*this);
            This.populateSortedByTotalCost();
        }
        return this->Data->SortedByTotalCost.constBegin();
    }

    const_iterator constEnd() const {
        if(this->Data->SortedByTotalCost.isEmpty()) {
            clsSearchGraphNodeContainer& This = const_cast<clsSearchGraphNodeContainer&>(*this);
            This.populateSortedByTotalCost();
        }
        return this->Data->SortedByTotalCost.constEnd();
    }

    iterator erase(iterator _begin, iterator _end) {
        Q_ASSERT(this->Data->SortedByTotalCost.isEmpty() == false);
        auto Result = this->Data->SortedByTotalCost.erase(_begin, _end);
        this->Data->SortedByState.clear();
        for(auto Iter = this->Data->SortedByTotalCost.begin();
            Iter != this->Data->SortedByTotalCost.end(); ++Iter)
            this->Data->SortedByState.insert(*Iter);
        Q_ASSERT((int)this->Data->SortedByState.size() == this->Data->SortedByTotalCost.size());
        return Result;
    }

    const clsSearchGraphNode& first() const {
        return this->Data->BestNode;
    }

    const clsSearchGraphNode& last() const {
        return this->Data->WorstNode;
    }

    bool isEmpty() const {
        return this->Data->SortedByState.empty();
    }

    int size() const {
        return (int)this->Data->SortedByState.size();
    }

    void finalizeRecombination() {
//        this->Data->SortedByTotalCost.clear();
//        for(auto Iter = this->Data->SortedByState.begin();
//            Iter != this->Data->SortedByState.end(); ++Iter) {
//            clsSearchGraphNode& Node = const_cast<clsSearchGraphNode&>(*Iter);
//            if(this->Data->BestNode.getTotalCost() < Node.getTotalCost())
//                this->Data->BestNode.recombine(Node);
//        }
//        this->Data->SortedByState.clear();
//        this->Data->WorstNode = *pInvalidSearchGraphNode;
//        this->Data->SortedByState.insert(this->Data->BestNode);

//        this->Data->SortedByTotalCost.append(this->Data->BestNode);
    }

    void clear() {
        this->Data->SortedByTotalCost.clear();
        this->Data->SortedByState.clear();
    }

    bool push_back(clsSearchGraphNode& _node) {
        return this->insert(_node);
    }

private:
    QExplicitlySharedDataPointer<clsSearchGraphNodeContainerData> Data;

};

/**
 * @brief The clsLexicalHypothesisContainerData class holds and manages A list of nodes.
 */
class clsLexicalHypothesisContainerData :public QSharedData
{
public:
    clsLexicalHypothesisContainerData(bool _keepRecombined) :
        Nodes(_keepRecombined)
    {}
    clsLexicalHypothesisContainerData(clsLexicalHypothesisContainerData &_other):
        QSharedData(_other),
        Nodes(_other.Nodes)
    {}
    ~clsLexicalHypothesisContainerData(){}
    clsSearchGraphNodeContainer Nodes; /**< list of search graph nodes */


    friend class UnitTestNameSpace::clsUnitTest;
};

/**
 * @brief The clsLexicalHypothesisContainer class manages same coverage nodes.
 * It stores and manages a list of these nodes.
 */
class clsLexicalHypothesisContainer
{
public:
    clsLexicalHypothesisContainer();
    clsLexicalHypothesisContainer(const clsLexicalHypothesisContainer& _other) :
        Data(_other.Data)
    {}
    ~clsLexicalHypothesisContainer(){}

    inline static clsLexicalHypothesisContainer rootLexicalHypothesis(){
        clsLexicalHypothesisContainer LexicalHypothesis;
        LexicalHypothesis.Data->Nodes.insert(*pInvalidSearchGraphNode);
        return LexicalHypothesis;
    }

    Common::Cost_t getBestCost() const;

    inline clsSearchGraphNodeContainer& nodes() const{
        return this->Data->Nodes;
    }

    bool insertHypothesis(clsSearchGraphNode &_node);

    void finalizeRecombination();

#ifdef TARGOMAN_SHOW_DEBUG
private:
    const clsSearchGraphNode& FindNode(const char* _targetRuleStr, const char* _prevTargetRuleStr, const char* _coverage) const;
#endif

private:
    QExplicitlySharedDataPointer<clsLexicalHypothesisContainerData> Data;           /**< A pointer of clsLexicalHypothesisContainerData class that stores a list of nodes.*/

    static Targoman::Common::Configuration::tmplConfigurable<bool> KeepRecombined;  /**< Store recombined node or node.*/

    friend class UnitTestNameSpace::clsUnitTest;

};

}
}
}
}

#endif // TARGOMAN_CORE_PRIVATE_SEARCHGRAPHBUILDER_CLSLEXICALHYPOTHESIS_H
