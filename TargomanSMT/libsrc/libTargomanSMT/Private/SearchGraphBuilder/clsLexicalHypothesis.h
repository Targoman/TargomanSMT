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

namespace Targoman{
namespace SMT {
namespace Private{
namespace SearchGraphBuilder {

/**
 * @brief The clsLexicalHypothesisContainerData class holds and manages A list of nodes.
 */
class clsLexicalHypothesisContainerData :public QSharedData
{
public:
    clsLexicalHypothesisContainerData(){}
    clsLexicalHypothesisContainerData(clsLexicalHypothesisContainerData &_other):
        QSharedData(_other),
        Nodes(_other.Nodes)
    {}
    ~clsLexicalHypothesisContainerData(){}
    QList<clsSearchGraphNode> Nodes;            /**< list of search graph nodes */

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
        LexicalHypothesis.Data->Nodes.append(*pInvalidSearchGraphNode);
        return LexicalHypothesis;
    }

    Common::Cost_t getBestCost() const;

    inline QList<clsSearchGraphNode>& nodes() const{
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
