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
#include "Private/Proxies/intfLMSentenceScorer.hpp"
#include "Private/RuleTable/clsTargetRule.h"

namespace Targoman{
namespace SMT {
namespace Private{
namespace SearchGraph {

TARGOMAN_ADD_EXCEPTION_HANDLER(exSearchGraph, exTargomanCore);

class clsSearchGraphNodeData;

/**
 * @brief Every feature function has some data. This base class is an interface to hold function basic data.
 * every feature function oveloads this class to store its feature specific data.
 */
class intfFeatureFunctionData{
public:
    /**
     * @brief constructor of this class resizes #CostElemnts vector with _costElementsSize and initializes those with zero.
     * @param _costElementsSize resize size.
     */
    intfFeatureFunctionData(size_t _costElementsSize) :
        CostElements(_costElementsSize,0)
    {}

    inline const QVector<Common::Cost_t>&     costElements(){return this->CostElements;}

public:
    QVector<Common::Cost_t>     CostElements;           /**< Feature fucntion stores its costs to this vector. */
};

typedef QVector<QVector<Common::Cost_t> > RestCostMatrix_t;

class clsSearchGraphNode
{
public:
    clsSearchGraphNode();
    clsSearchGraphNode(const clsSearchGraphNode& _prevNode,
                       quint16 _startPos,
                       quint16 _endPos,
                       const Coverage_t &_newCoverage,
                       const RuleTable::clsTargetRule& _targetRule,
                       bool _isFinal,
                       Common::Cost_t _restCost);

    ~clsSearchGraphNode(){}

    inline Common::Cost_t getTotalCost() const;
    inline Common::Cost_t getCost() const;

    void recombine(clsSearchGraphNode& _node);

    bool haveSameFuture(const clsSearchGraphNode& _node) const;

    inline size_t sourceRangeBegin() const;
    inline size_t sourceRangeEnd() const;
    inline const clsSearchGraphNode&  prevNode() const;
    inline const RuleTable::clsTargetRule& targetRule() const;
    inline const Coverage_t&    coverage() const;
    inline bool isRecombined() const;
    inline bool isInvalid() const;
    inline void setFeatureFunctionData(size_t _index, intfFeatureFunctionData* _data);
    inline const intfFeatureFunctionData* featureFunctionDataAt(size_t _index) const;

    inline bool isFinal();

#ifdef TARGOMAN_SHOW_DEBUG
public:
    inline QList<Common::Cost_t> costElements() const;
#endif

public:
    static size_t allocateFeatureFunctionData();

private:
    QExplicitlySharedDataPointer<clsSearchGraphNodeData>     Data;
};


/**
 * @brief The clsSearchGraphNodeData class is responsible for storing and managing data member of clsSearchGraphNode class.
 */
class clsSearchGraphNodeData : public QSharedData
{
public:

    /**
     * @brief This is the default constructor of this class.
     */
    clsSearchGraphNodeData() :
        IsFinal(false),
        Cost(0),
        RestCost(0),
        TargetRule(*RuleTable::pInvalidTargetRule),
        IsRecombined(false),
        Coverage(Coverage_t()),
        SourceRangeBegin(0),
        SourceRangeEnd(0),
        PrevNode(NULL),
        FeatureFunctionsData(clsSearchGraphNodeData::RegisteredFeatureFunctionCount)
    {
    }


    /**
     * @brief                   Constructor of this class.
     * @param _prevNode         Previous node of this search graph node.
     * @param _startPos         This node has a translation for a phrase of input sentence, start position of this phrase in input sentence is this variable.
     * @param _endPos           This node has a translation for a phrase of input sentence, end position of this phrase in input sentence is this variable.
     * @param _newCoverage      Updated covered for that is translated is stored in this variable.
     * @param _targetRule       Target language phrase translation.
     * @param _isFinal          Has this node covered translation for all word of input sentence.
     * @param _restCost         approximated cost of rest of translation.
     */
    clsSearchGraphNodeData(const clsSearchGraphNode& _prevNode,
                           quint8 _startPos,
                           quint8 _endPos,
                           const Coverage_t &_newCoverage,
                           const RuleTable::clsTargetRule &_targetRule,
                           bool _isFinal,
                           Common::Cost_t _restCost):
        IsFinal(_isFinal),
        Cost(_prevNode.getTotalCost()),
        RestCost(_restCost),
        TargetRule(_targetRule),
        IsRecombined(false),
        Coverage(_newCoverage),
        SourceRangeBegin(_startPos),
        SourceRangeEnd(_endPos),
        PrevNode(&_prevNode),
        FeatureFunctionsData(clsSearchGraphNodeData::RegisteredFeatureFunctionCount)
    {
    }

    /**
     * @brief Copy constructor of this class.
     */
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
        PrevNode(_other.PrevNode),
        CombinedNodes(_other.CombinedNodes)
    {}

public:
    bool                                IsFinal;                        /**< Has this node covered translation for all word of input sentence.*/
    Common::Cost_t                      Cost;                           /**< Cost of translation up to now.*/
    Common::Cost_t                      RestCost;                       /**< Approximated cost of rest of translation.*/
    const RuleTable::clsTargetRule&     TargetRule;                     /**< Target language phrase translation is stored in this variable.*/
    bool                                IsRecombined;                   /**< Has this node been combined with another node or not.*/
    Coverage_t                          Coverage;                       /**< Covered words for translation.*/
    size_t                              SourceRangeBegin;               /**< This node has a translation for a phrase of input sentence, start position of this phrase in input sentence is this variable.*/
    size_t                              SourceRangeEnd;                 /**< This node has a translation for a phrase of input sentence, end position of this phrase in input sentence is this variable.*/
    const clsSearchGraphNode*           PrevNode;                       /**< Previous node of this search graph node.*/
    QList<clsSearchGraphNode>           CombinedNodes;                  /**< List of nodes that are combined with this node.*/
    QVector<intfFeatureFunctionData*>   FeatureFunctionsData;           /**< Every feature function has a special data. Each index of this list stores data for one the feature function. Each feature function knows his own index in this list.  */
    static  size_t                      RegisteredFeatureFunctionCount; /**< Number of active feature functions.*/
};


/**
 * @brief Sum of cost and rest cost of this node.
 * @return Returns total cost for this node.
 */
inline Common::Cost_t clsSearchGraphNode::getTotalCost() const{
    return this->Data->Cost + this->Data->RestCost;
}

/**
 * @brief Returns Cost of this node up to now.
 */
inline  Common::Cost_t clsSearchGraphNode::getCost() const{
    return this->Data->Cost;
}

extern clsSearchGraphNodeData* InvalidSearchGraphNodeData;
extern clsSearchGraphNode* pInvalidSearchGraphNode;

inline size_t clsSearchGraphNode::sourceRangeBegin() const { return this->Data->SourceRangeBegin;}
inline size_t clsSearchGraphNode::sourceRangeEnd() const { return this->Data->SourceRangeEnd;}
inline const clsSearchGraphNode&  clsSearchGraphNode::prevNode() const {
    return Q_LIKELY(this->Data->PrevNode) ? *this->Data->PrevNode : *pInvalidSearchGraphNode;
}
inline const RuleTable::clsTargetRule& clsSearchGraphNode::targetRule() const {return this->Data->TargetRule;}
inline const Coverage_t&    clsSearchGraphNode::coverage() const{return this->Data->Coverage;}
inline bool clsSearchGraphNode::isRecombined() const {return this->Data->IsRecombined;}
inline bool clsSearchGraphNode::isInvalid() const {return this->Data == InvalidSearchGraphNodeData;}
inline bool clsSearchGraphNode::isFinal(){return this->Data->IsFinal;}

/**
 * @brief Returns a list of cost of all feature funcitons.
 */
QList<Common::Cost_t> clsSearchGraphNode::costElements() const
{
    QList<Common::Cost_t> result;
    for(size_t i = 0; i < this->Data->RegisteredFeatureFunctionCount; ++i)
        if(this->Data->FeatureFunctionsData.at(i) != NULL)
            result.append(this->Data->FeatureFunctionsData.at(i)->costElements().toList());
    return result;
}
/**
 * @brief sets data of a feature function in FeatureFunctionsData member of #Data.
 * @param _index    index of feature function.
 * @param _data     data of that feature function.
 */
inline void clsSearchGraphNode::setFeatureFunctionData(size_t _index, intfFeatureFunctionData* _data){
    this->Data->FeatureFunctionsData[_index] = _data;
}
/**
 * @brief Return data of a feature function.
 * @param _index index of feature funciton.
 */
const intfFeatureFunctionData *clsSearchGraphNode::featureFunctionDataAt(size_t _index) const {
    return this->Data->FeatureFunctionsData.at(_index);
}

}
}
}
}

#endif // TARGOMAN_CORE_PRIVATE_SEARCHGRAPHBUILDER_CLSSEARCHGRAPHNODE_H
