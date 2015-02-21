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

#ifndef TARGOMAN_CORE_PRIVATE_SEARCHGRAPHBUILDER_CLSSEARCHGRAPHNODE_H
#define TARGOMAN_CORE_PRIVATE_SEARCHGRAPHBUILDER_CLSSEARCHGRAPHNODE_H

#include "libTargomanCommon/Types.h"
#include "Private/LanguageModel/intfLMSentenceScorer.hpp"
#include "Private/RuleTable/clsTargetRule.h"

namespace Targoman{
namespace Core {
namespace Private{
namespace SearchGraphBuilder {

typedef QBitArray Coverage_t; // TODO why it is redeclared? First Decalration in  clsCardinality.h

class clsSearchGraphNodeData;

class clsSearchGraphNode
{
public:
    clsSearchGraphNode();
    clsSearchGraphNode(const clsSearchGraphNode& _prevNode,
            const RuleTable::clsTargetRule& _targetRule,
            Common::Cost_t _cost,
            Common::Cost_t _reorderingJumpCost,
            Common::Cost_t _restCost,
            Common::Cost_t _lmCost,
            quint8 _startPos,
            quint8 _endPos,
            const Coverage_t &_newCoverage,
            bool _isFinal,
            LanguageModel::intfLMSentenceScorer* _lmscorer);
    ~clsSearchGraphNode(){}

    inline Common::Cost_t getTotalCost() const;
    inline Common::Cost_t getCost() const;

    void recombine(clsSearchGraphNode& _node);

    bool haveSameFuture(const clsSearchGraphNode& _node) const;

    inline const LanguageModel::intfLMSentenceScorer& lmScorer() const;

    inline size_t sourceRangeBegin() const;
    inline size_t sourceRangeEnd() const;
    inline const clsSearchGraphNode&  prevNode() const;
    inline const RuleTable::clsTargetRule& targetRule() const;
    inline const Coverage_t&    coverage() const;
    inline bool isRecombined() const;
    inline bool isInvalid() const;

private:
    QExplicitlySharedDataPointer<clsSearchGraphNodeData>          Data;
};

class clsSearchGraphNodeData : public QSharedData
{
public:
    clsSearchGraphNodeData() :
        IsFinal(false),
        Cost(0),
        RestCost(0),
        TargetRule(*RuleTable::InvalidTargetRule),
        IsRecombined(false),
        Coverage(Coverage_t()),
        SourceRangeBegin(0),
        SourceRangeEnd(0),
        LMScorer(gConfigs.LM.getInstance<LanguageModel::intfLMSentenceScorer>()),
        PrevNode(NULL)
    {}

    clsSearchGraphNodeData(const clsSearchGraphNode& _prevNode,
                           const RuleTable::clsTargetRule &_targetRule,
                           Common::Cost_t _cost,
                           Common::Cost_t _reorderingJumpCost,
                           Common::Cost_t _restCost,
                           Common::Cost_t _lmCost,
                           quint8 _startPos,
                           quint8 _endPos,
                           const Coverage_t &_newCoverage,
                           bool _isFinal,
                           LanguageModel::intfLMSentenceScorer *_lmscorer):
        IsFinal(_isFinal),
        Cost(_cost),
        RestCost(_restCost),
        TargetRule(_targetRule),
        IsRecombined(false),
        Coverage(_newCoverage),
        SourceRangeBegin(_startPos),
        SourceRangeEnd(_endPos),
        LMScorer(_lmscorer),
        PrevNode(&_prevNode)
    {
        Q_UNUSED(_reorderingJumpCost) // used when training
        Q_UNUSED(_lmCost)// used when training
    }

    clsSearchGraphNodeData(clsSearchGraphNodeData& _other) :
        QSharedData(_other),
        IsFinal(_other.IsFinal),
        Cost(_other.Cost),
        RestCost(_other.RestCost),
        TargetRule(_other.TargetRule),
        IsRecombined(_other.IsRecombined),
        Coverage(_other.Coverage),
        SourceRangeBegin(_other.SourceRangeBegin),
        SourceRangeEnd(_other.SourceRangeEnd),
        LMScorer(_other.LMScorer.data()),
        PrevNode(_other.PrevNode),
        CombinedNodes(_other.CombinedNodes)
    {}

    ~clsSearchGraphNodeData();

public:
    bool IsFinal;

    mutable Common::Cost_t Cost;
    mutable Common::Cost_t RestCost;
    const RuleTable::clsTargetRule& TargetRule;
    bool                   IsRecombined;

    Coverage_t             Coverage;
    size_t SourceRangeBegin;
    size_t SourceRangeEnd;

    QScopedPointer<LanguageModel::intfLMSentenceScorer>     LMScorer;
    const clsSearchGraphNode*                               PrevNode;
    QList<clsSearchGraphNode>                               CombinedNodes;
};

inline Common::Cost_t clsSearchGraphNode::getTotalCost() const{
    return this->Data->Cost;// + this->Data->RestCost;
}

inline  Common::Cost_t clsSearchGraphNode::getCost() const{
    return this->Data->Cost;
}

inline const LanguageModel::intfLMSentenceScorer& clsSearchGraphNode::lmScorer() const{
    return *this->Data->LMScorer;
}


extern clsSearchGraphNodeData* InvalidSearchGraphNodeData;

inline size_t clsSearchGraphNode::sourceRangeBegin() const { return this->Data->SourceRangeBegin;}
inline size_t clsSearchGraphNode::sourceRangeEnd() const { return this->Data->SourceRangeEnd;}
inline const clsSearchGraphNode&  clsSearchGraphNode::prevNode() const {return *this->Data->PrevNode;}
inline const RuleTable::clsTargetRule& clsSearchGraphNode::targetRule() const {return this->Data->TargetRule;}
inline const Coverage_t&    clsSearchGraphNode::coverage() const{return this->Data->Coverage;}
inline bool clsSearchGraphNode::isRecombined() const {return this->Data->IsRecombined;}
inline bool clsSearchGraphNode::isInvalid() const {return this->Data == InvalidSearchGraphNodeData;}

}
}
}
}

#endif // TARGOMAN_CORE_PRIVATE_SEARCHGRAPHBUILDER_CLSSEARCHGRAPHNODE_H
