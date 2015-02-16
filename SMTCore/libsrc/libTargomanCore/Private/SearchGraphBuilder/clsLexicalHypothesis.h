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
namespace Core {
namespace Private{
namespace SearchGraphBuilder {

class clsLexicalHypothesisData :public QSharedData
{
public:
    clsLexicalHypothesisData(){}
    clsLexicalHypothesisData(clsLexicalHypothesisData &_other):
        QSharedData(_other),
        Nodes(_other.Nodes)
    {}
    ~clsLexicalHypothesisData(){}
    QList<clsSearchGraphNode> Nodes;
};

class clsLexicalHypothesis
{
public:
    clsLexicalHypothesis();

    const clsSearchGraphNode& bestNode() const{
        return this->Data->Nodes.first();
    }


    inline static clsLexicalHypothesis rootLexicalHypothesis(){
        clsLexicalHypothesis LexicalHypothesis;
        LexicalHypothesis.Data->Nodes.append(clsSearchGraphNode());
        return LexicalHypothesis;
    }

    bool mustBePruned(Common::Cost_t _totalCost);

    Common::Cost_t getBestCost() const;

    inline QList<clsSearchGraphNode>& nodes(){
        return this->Data->Nodes;
    }

    bool insertHypothesis(clsSearchGraphNode &_node);

    void finalizeRecombination();

private:
    QExplicitlySharedDataPointer<clsLexicalHypothesisData> Data;

    static Common::Configuration::tmplConfigurable<quint8> LexicalMaxHistogramSize;
    static Targoman::Common::Configuration::tmplConfigurable<bool> KeepRecombined;
};

}
}
}
}

#endif // TARGOMAN_CORE_PRIVATE_SEARCHGRAPHBUILDER_CLSLEXICALHYPOTHESIS_H
