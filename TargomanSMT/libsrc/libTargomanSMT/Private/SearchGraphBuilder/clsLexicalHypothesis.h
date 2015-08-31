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

inline uint qHash(const clsSearchGraphNode& key, uint seed = 0) Q_DECL_NOTHROW {
    return qHash(key.futureStateHash(), seed);
}

class clsLexicalHypoNodeSet {
public:
    typedef typename QList<clsSearchGraphNode>::iterator iterator;
    typedef typename QList<clsSearchGraphNode>::const_iterator const_iterator;

private:
    QList<clsSearchGraphNode> SortedByTotalCost;
    QSet<clsSearchGraphNode> Nodes;

public:
    int size() const { return this->Nodes.size(); }
    bool isEmpty() const { return this->Nodes.isEmpty(); }

    const clsSearchGraphNode& at(int _index) const { return this->SortedByTotalCost.at(_index); }
    clsSearchGraphNode& operator [] (int _index) { return this->SortedByTotalCost[_index]; }

    void clear() {
        this->Nodes.clear();
        this->SortedByTotalCost.clear();
    }

    iterator begin() { return this->SortedByTotalCost.begin(); }
    iterator end() { return this->SortedByTotalCost.end(); }

    const_iterator begin() const { return this->SortedByTotalCost.constBegin(); }
    const_iterator end() const { return this->SortedByTotalCost.constEnd(); }

    const_iterator constBegin() { return this->SortedByTotalCost.constBegin(); }
    const_iterator constEnd() { return this->SortedByTotalCost.constEnd(); }

    const clsSearchGraphNode& bestNode() const { return SortedByTotalCost.first(); }
    const clsSearchGraphNode& worstNode() const { return SortedByTotalCost.last(); }

    QPair<clsSearchGraphNode, bool> insert(const clsSearchGraphNode& _node) {

        auto NodeIter = this->Nodes.find(_node);
        if(NodeIter != this->Nodes.end())
            return QPair<clsSearchGraphNode, bool>(*NodeIter, false);

        this->Nodes.insert(_node);
        QPair<int, bool> InsertionPosEx = findInsertionPosEx(this->SortedByTotalCost, _node,
            [] (const clsSearchGraphNode& _first, const clsSearchGraphNode& _second) {
                double Delta = _first.getTotalCost() - _second.getTotalCost();
                return Delta < 0 ? -1 : (Delta > 0 ? 1 : 0);
            }
        );
        this->SortedByTotalCost.insert(InsertionPosEx.first, _node);

        return QPair<clsSearchGraphNode, bool>(_node, true);
    }

    iterator erase(iterator _pos) {
        this->Nodes.remove(*_pos);
        return this->SortedByTotalCost.erase(_pos);
    }

    iterator erase(iterator _first, iterator _last) {
        iterator Iter = _first;
        while(Iter != _last && Iter != this->SortedByTotalCost.end())
            Iter = this->erase(Iter);
        return Iter;
    }
};

/**
 * @brief The clsLexicalHypothesisContainerData class holds and manages A list of nodes.
 */
class clsLexicalHypothesisContainerData :public QSharedData
{
public:
    clsLexicalHypothesisContainerData() {}
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
        LexicalHypothesis.Data->Nodes.insert(*pInvalidSearchGraphNode);
        return LexicalHypothesis;
    }

    Common::Cost_t getBestCost() const;
    Common::Cost_t getWorstCost() const;

    inline clsLexicalHypoNodeSet& nodes() const{
        return this->Data->Nodes;
    }

    bool insertHypothesis(clsSearchGraphNode& _node);

    void finalizeRecombination();

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
