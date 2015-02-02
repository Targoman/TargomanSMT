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

#ifndef CLSSEARCHGRAPHNODE_H
#define CLSSEARCHGRAPHNODE_H

#include "libTargomanCommon/Types.h"
#include "Private/LanguageModel/intfLMSentenceScorer.hpp"
#include "clsCardinality.h"

namespace Targoman{
namespace Core {
namespace Private{
namespace SearchGraphBuilder {

class clsSearchGraphNode
{
public:
    clsSearchGraphNode(clsSearchGraphNode* _prevNode,
            const clsTargetRule& _targetRule,
            Common::Cost_t _cost,
            Common::Cost_t _reorderingJumpCost,
            Common::Cost_t _restCost,
            Common::Cost_t _lmCost,
            quint8 _startPos,
            quint8 _endPos,
            const Coverage_t &_newCoverage,
            bool _isFinal,
            LanguageModel::intfLMSentenceScorer* _lmscorer);

    Common::Cost_t getTotalCost(){
        return this->Cost + this->RestCost;
    }

    Common::Cost_t getCost(){
        return this->Cost;
    }



    inline const LanguageModel::intfLMSentenceScorer& lmScorer() const{
        return *this->LMScorer;
    }

private:
    //bool IsInitial;
    bool IsFinal;

    mutable Common::Cost_t Cost;
    mutable Common::Cost_t RestCost;

    size_t SourceRangeBegin;
    size_t SourceRangeEnd;

    QSharedDataPointer<LanguageModel::intfLMSentenceScorer> LMScorer;
    QSharedDataPointer<clsSearchGraphNode> PrevNode;
};

}
}
}
}

#endif // CLSSEARCHGRAPHNODE_H
