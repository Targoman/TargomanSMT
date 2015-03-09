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

#include "ReorderingJump.h"

namespace Targoman {
namespace SMT {
namespace Private {
namespace FeatureFunction{

using namespace Common;
using namespace SearchGraph;
using namespace Proxies;
using namespace RuleTable;

TARGOMAN_REGISTER_SINGLETON_MODULE(ReorderingJump);

Common::Configuration::tmplConfigurable<double>  ReorderingJump::ScalingFactor(
        ReorderingJump::baseConfigPath() + "/ScalingFactor",
        "Scaling factor for reordering jump model feature.",
        1.0);

Common::Configuration::tmplConfigurable<quint8>  ReorderingJump::MaximumJumpWidth(
        ReorderingJump::baseConfigPath() + "/MaximumJumpWidth",
        "Maximum jump width.",
        6);


/**
 * @brief The clsReorderingJumpFeatureData class is a derviation of intfFeatureFunctionData class.
 */
class clsReorderingJumpFeatureData : public intfFeatureFunctionData{
public:
    /**
     * @brief constructor of this class sets CostElements to 1 because we have cost for reordering jump feature.
     */
    clsReorderingJumpFeatureData():
        intfFeatureFunctionData(1)
    {}

    intfFeatureFunctionData* copy() const {
        clsReorderingJumpFeatureData* Copy = new clsReorderingJumpFeatureData();
        Copy->CostElements[0] = this->CostElements[0];
        return Copy;
    }
};

/**
 * @brief LexicalReordering::scoreSearchGraphNode   Sets CostElement value and computes reordering jump based on
 * range of source phrase words of this node and previous node.
 * @return Returns score of ReorderingJumpfor this search graph node.
 */
Common::Cost_t ReorderingJump::scoreSearchGraphNode(clsSearchGraphNode &_newHypothesisNode) const
{
    clsReorderingJumpFeatureData* Data = new clsReorderingJumpFeatureData;
    _newHypothesisNode.setFeatureFunctionData(this->DataIndex, Data);

    size_t JumpWidth = qAbs((int)_newHypothesisNode.sourceRangeBegin() -
                            (int)_newHypothesisNode.prevNode().sourceRangeEnd());

    Cost_t Cost = ReorderingJump::getJumpCost(JumpWidth);

    if(gConfigs.WorkingMode.value() != enuWorkingModes::Decode)
        Data->CostElements[0] = Cost;

    return Cost * ReorderingJump::ScalingFactor.value();
}

/**
 * @brief ReorderingJump::getRestCostForPosition
 * @param _coverage
 * @param _beginPos
 * @param endPos
 * @return
 */
Cost_t ReorderingJump::getRestCostForPosition(const Coverage_t& _coverage, size_t _beginPos, size_t endPos) const
{
    Q_UNUSED(_beginPos)
    size_t LastPos = endPos - 1;

    Cost_t SumJumpCost = 0.0;
    bool LastPositionCovered = false;
    bool CurrentPositionCovered = false;
    bool NextPositionCovered = _coverage.at(0);
    size_t InputSentenceSize = _coverage.size();
    size_t JumpWidth = 0;

    size_t Position = 0;
    for(; Position < InputSentenceSize; ++Position)
    {
        LastPositionCovered = CurrentPositionCovered;
        CurrentPositionCovered = NextPositionCovered;
        NextPositionCovered = (Position + 1 == InputSentenceSize || _coverage.at(Position + 1));

        if( (Position == 0 || LastPositionCovered) && CurrentPositionCovered == 0 )
        {
            JumpWidth = std::abs((int)(LastPos - Position));
            SumJumpCost += ReorderingJump::getJumpCost(JumpWidth);
        }

        if(Position > 0 && CurrentPositionCovered == 0 && NextPositionCovered )
            LastPos = Position + 1;
    }
    Q_ASSERT(SumJumpCost >= 0);
    return SumJumpCost * ReorderingJump::ScalingFactor.value();
}


/**
 * @brief ReorderingJump::initRootNode This function will be called in the constructor of searchGraphNode
 * in order to always have a valid previous node data for feature functions in scoreSearchGraphNode function.
 * @param _rootNode
 */
void ReorderingJump::initRootNode(clsSearchGraphNode &_rootNode)
{
    _rootNode.setFeatureFunctionData(this->DataIndex, new clsReorderingJumpFeatureData);
}

}
}
}
}

