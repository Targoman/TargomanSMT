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

class clsLexicalHypothesisContainerData :public QSharedData
{
public:
    clsLexicalHypothesisContainerData(){}
    clsLexicalHypothesisContainerData(clsLexicalHypothesisContainerData &_other):
        QSharedData(_other),
        Nodes(_other.Nodes)
    {}
    ~clsLexicalHypothesisContainerData(){}
    QList<clsSearchGraphNode> Nodes;
};

class clsLexicalHypothesisContainer
{
public:
    clsLexicalHypothesisContainer();
    ~clsLexicalHypothesisContainer(){}

    const clsSearchGraphNode& bestNode() const{
        return this->Data->Nodes.first();
    }


    inline static clsLexicalHypothesisContainer rootLexicalHypothesis(){
        clsLexicalHypothesisContainer LexicalHypothesis;
        LexicalHypothesis.Data->Nodes.append(*pInvalidSearchGraphNode);
        return LexicalHypothesis;
    }

    bool mustBePruned(Common::Cost_t _totalCost);

    Common::Cost_t getBestCost() const;

    inline QList<clsSearchGraphNode>& nodes() const{
        return this->Data->Nodes;
    }

    bool insertHypothesis(clsSearchGraphNode &_node);

    void finalizeRecombination();

//TODO uncomment
//private:
public:
    QExplicitlySharedDataPointer<clsLexicalHypothesisContainerData> Data;

    static Common::Configuration::tmplConfigurable<quint8> LexicalMaxHistogramSize;
    static Targoman::Common::Configuration::tmplConfigurable<bool> KeepRecombined;
};

}
}
}
}

#endif // TARGOMAN_CORE_PRIVATE_SEARCHGRAPHBUILDER_CLSLEXICALHYPOTHESIS_H
