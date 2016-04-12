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
#include <iostream>

namespace Targoman{
namespace SMT {
namespace Private{
namespace SearchGraphBuilder {

class clsLexicalHypoNodeSetData : public QSharedData {
public:
    clsLexicalHypoNodeSetData() {}
    clsLexicalHypoNodeSetData(const clsLexicalHypoNodeSetData& _other) :
        QSharedData(_other),
        SortedByTotalCost(_other.SortedByTotalCost),
        SortedByState(_other.SortedByState),
        BestNode(_other.BestNode),
        WorstNode(_other.WorstNode)
    { }
    clsLexicalHypoNodeSetData(clsSearchGraphNode _bestNode,
                                    clsSearchGraphNode _worstNode) :
        BestNode(_bestNode),
        WorstNode(_worstNode)
    { }
    ~clsLexicalHypoNodeSetData() {}


    QList<clsSearchGraphNode> SortedByTotalCost;
    std::set<clsSearchGraphNode> SortedByState;
    clsSearchGraphNode BestNode;
    clsSearchGraphNode WorstNode;
};

class clsLexicalHypoNodeSet {
public:
    typedef typename QList<clsSearchGraphNode>::iterator iterator;
    typedef typename QList<clsSearchGraphNode>::const_iterator const_iterator;

    clsLexicalHypoNodeSet() :
           Data(new clsLexicalHypoNodeSetData(
                    *pInvalidSearchGraphNode,
                    *pInvalidSearchGraphNode
                    )
                )
       { }

       clsLexicalHypoNodeSet(const clsLexicalHypoNodeSet& _other) :
           Data(_other.Data)
       { }

       ~clsLexicalHypoNodeSet() { }

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
            this->Data->SortedByTotalCost.insert(Pos, Node);
        }
    }

public:
    int size() const { return this->Data->SortedByState.size(); }
    bool isEmpty() const { return this->Data->SortedByState.empty(); }

    const clsSearchGraphNode& at(int index) const {
            if(this->Data->SortedByTotalCost.isEmpty()) {
                clsLexicalHypoNodeSet& This = const_cast<clsLexicalHypoNodeSet&>(*this);
                This.populateSortedByTotalCost();
            }
            return this->Data->SortedByTotalCost.at(index);
    }

    clsSearchGraphNode& operator [] (int _index) {
        if(this->Data->SortedByTotalCost.isEmpty())
               this->populateSortedByTotalCost();
        return this->Data->SortedByTotalCost[_index];
    }

    void clear() {
        this->Data->SortedByState.clear();
        this->Data->SortedByTotalCost.clear();
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
                   clsLexicalHypoNodeSet& This = const_cast<clsLexicalHypoNodeSet&>(*this);
                   This.populateSortedByTotalCost();
               }
               return this->Data->SortedByTotalCost.constBegin();
           }

           const_iterator constEnd() const {
               if(this->Data->SortedByTotalCost.isEmpty()) {
                   clsLexicalHypoNodeSet& This = const_cast<clsLexicalHypoNodeSet&>(*this);
                   This.populateSortedByTotalCost();
               }
               return this->Data->SortedByTotalCost.constEnd();
           }

    const clsSearchGraphNode& bestNode() {
        return this->Data->BestNode;
    }
    const clsSearchGraphNode& worstNode() {
        return this->Data->WorstNode;
    }

    bool insert(clsSearchGraphNode& _node, bool _keepRecombined) {

           auto SetInsertionResult = this->Data->SortedByState.insert(_node);
           if(SetInsertionResult.second == false) {
               clsSearchGraphNode& Node = const_cast<clsSearchGraphNode&>(*SetInsertionResult.first);
//               Q_ASSERT(_node.haveSameFuture(*SetInsertionResult.first));
               if(_keepRecombined) {
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
//           this->Data->SortedByTotalCost.clear();
           if(this->Data->SortedByTotalCost.size() > 0){
                 int Pos = findInsertionPos(this->Data->SortedByTotalCost,
                                      _node,
                                      [] (const clsSearchGraphNode& _first, const clsSearchGraphNode& _second) {
                        return _second.getTotalCost() - _first.getTotalCost();
                 });
                 this->Data->SortedByTotalCost.insert(Pos, _node);
           }
           if(this->Data->BestNode.isInvalid() || _node.getTotalCost() < this->Data->BestNode.getTotalCost())
               this->Data->BestNode = _node;
           if(this->Data->WorstNode.isInvalid() || _node.getTotalCost() > this->Data->WorstNode.getTotalCost())
               this->Data->WorstNode = _node;
           return true;
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

    iterator erase(iterator _pos) {
        this->Data->SortedByState.erase(*_pos);
        return this->Data->SortedByTotalCost.erase(_pos);
    }

private:
    QExplicitlySharedDataPointer<clsLexicalHypoNodeSetData> Data;

};

/**
 * @brief The clsLexicalHypothesisContainerData class holds and manages A list of nodes.
 */
class clsLexicalHypothesisContainerData :public QSharedData
{
public:
    clsLexicalHypothesisContainerData() :
           Nodes()
       {}
       clsLexicalHypothesisContainerData(clsLexicalHypothesisContainerData &_other):
           QSharedData(_other),
           Nodes(_other.Nodes)
       {}
       ~clsLexicalHypothesisContainerData(){}

    //QList<clsSearchGraphNode> Nodes;            /**< list of search graph nodes */
    clsLexicalHypoNodeSet Nodes;

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
        LexicalHypothesis.Data->Nodes.insert(*pInvalidSearchGraphNode, clsLexicalHypothesisContainer::KeepRecombined.value());
        return LexicalHypothesis;
    }

    Common::Cost_t getBestCost() const;
    Common::Cost_t getWorstCost() const;

    inline clsLexicalHypoNodeSet& nodes() const{
        return this->Data->Nodes;
    }

    bool insertHypothesis(clsSearchGraphNode& _node);

    void finalizeRecombination();
    static QString moduleName();

#ifdef TARGOMAN_SHOW_DEBUG
private:
    const clsSearchGraphNode& FindNode(const char* _targetRuleStr, const char* _prevTargetRuleStr, const char* _coverage) const;
#endif

private:
    QExplicitlySharedDataPointer<clsLexicalHypothesisContainerData> Data;           /**< A pointer of clsLexicalHypothesisContainerData class that stores a list of nodes.*/

    static Targoman::Common::Configuration::tmplConfigurable<bool> KeepRecombined;  /**< Store recombined node or node.*/

    friend class clsLexicalHypoNodeSet;
    friend class UnitTestNameSpace::clsUnitTest;

};

}
}
}
}

#endif // TARGOMAN_CORE_PRIVATE_SEARCHGRAPHBUILDER_CLSLEXICALHYPOTHESIS_H
