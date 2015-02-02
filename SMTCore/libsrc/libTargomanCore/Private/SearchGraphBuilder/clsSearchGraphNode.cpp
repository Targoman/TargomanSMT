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

namespace Targoman{
namespace Core {
namespace Private{
namespace SearchGraphBuilder {

clsSearchGraphNode::clsSearchGraphNode(clsSearchGraphNode *_prevNode, LanguageModel::intfLMSentenceScorer *_lmscorer):

{
}

clsSearchGraphNode::clsSearchGraphNode(clsSearchGraphNode *_prevNode,
                                       const clsTargetRule &_targetRule,
                                       Common::Cost_t _cost,
                                       Common::Cost_t _reorderingJumpCost,
                                       Common::Cost_t _restCost,
                                       Common::Cost_t _lmCost,
                                       quint8 _startPos,
                                       quint8 _endPos,
                                       const Coverage_t& _newCoverage,
                                       bool _isFinal,
                                       LanguageModel::intfLMSentenceScorer *_lmscorer) :
    PrevNode(_prevNode),
    Cost(_prevCost),
    RestCost(_restCost),
    SourceRangeBegin(_startPos),
    SourceRangeEnd(_endPos),
    IsFinal(_isFinal),
    LMScorer(_lmscorer)
{
    Q_UNUSED(_reorderingJumpCost) // used when training
    Q_UNUSED(_lmCost)// used when training

}

}
}
}
}
