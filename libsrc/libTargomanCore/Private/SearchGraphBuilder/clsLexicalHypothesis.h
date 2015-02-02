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

#ifndef CLSLEXICALHYPOTHESIS_H
#define CLSLEXICALHYPOTHESIS_H

#include <QList>
#include "clsSearchGraphNode.h"
#include "libTargomanCommon/Types.h"
#include "libTargomanCommon/Configuration/tmplConfigurable.h"

namespace Targoman{
namespace Core {
namespace Private{
namespace SearchGraphBuilder {

class clsLexicalHypothesis
{
public:
    clsLexicalHypothesis();

    clsSearchGraphNode& bestNode(){
        this->Nodes.first();
    }


    inline static clsLexicalHypothesis rootLexicalHypothesis(){
        clsLexicalHypothesis LexicalHypothesis;
        LexicalHypothesis.Nodes.append(clsSearchGraphNode());
        return LexicalHypothesis;

        //TODO OJO
    }

    bool mustBePruned(Common::Cost_t _totalCost);

    Common::Cost_t getBestCost() const;

    inline const QList<clsSearchGraphNode>& nodes(){
        return this->Nodes;
    }

    inline void insertHypothesis(clsSearchGraphNode* _node);


private:
    QList<clsSearchGraphNode> Nodes;

    static Common::Configuration::tmplConfigurable<quint8> LexicalMaxHistogramSize;

    /* PartialHypothesisSet hypothesisSet_;
    PartialHypothesisStateSet hypothesisStateSet_;

    Cost bestCostsWithRestCosts_;

    // TODO: make this variable static in SourceCardinalitySynchronousSearchDecoder?
    size_t lexicalHistogramSize_;

    //static bool keepRecombined_;    */
};

}
}
}
}

#endif // CLSLEXICALHYPOTHESIS_H
