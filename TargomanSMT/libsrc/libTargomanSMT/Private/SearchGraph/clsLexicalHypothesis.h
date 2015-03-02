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

#ifndef TARGOMAN_CORE_PRIVATE_SEARCHGRAPHBUILDER_CLSLEXICALHYPOTHESIS_H
#define TARGOMAN_CORE_PRIVATE_SEARCHGRAPHBUILDER_CLSLEXICALHYPOTHESIS_H

#include <QList>
#include "clsSearchGraphNode.h"
#include "libTargomanCommon/Types.h"
#include "libTargomanCommon/Configuration/tmplConfigurable.h"

namespace Targoman{
namespace SMT {
namespace Private{
namespace SearchGraph {

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
};

/**
 * @brief The clsLexicalHypothesisContainer class manages same coverage nodes.
 * It stores and manages a list of these nodes.
 */
class clsLexicalHypothesisContainer
{
public:
    clsLexicalHypothesisContainer();
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

//TODO uncomment
//private:
public:
    QExplicitlySharedDataPointer<clsLexicalHypothesisContainerData> Data;           /**< A pointer of clsLexicalHypothesisContainerData class that stores a list of nodes.*/

    static Targoman::Common::Configuration::tmplConfigurable<bool> KeepRecombined;  /**< Store recombined node or node.*/
};

}
}
}
}

#endif // TARGOMAN_CORE_PRIVATE_SEARCHGRAPHBUILDER_CLSLEXICALHYPOTHESIS_H
