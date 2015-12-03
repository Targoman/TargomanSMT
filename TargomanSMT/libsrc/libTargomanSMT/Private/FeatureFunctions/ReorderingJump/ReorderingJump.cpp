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

#include "ReorderingJump.h"

namespace Targoman {
namespace SMT {
namespace Private {
namespace FeatureFunction{

using namespace Common;
using namespace SearchGraphBuilder;
using namespace Proxies;
using namespace RuleTable;

TARGOMAN_REGISTER_SINGLETON_MODULE(ReorderingJump);

Common::Configuration::tmplConfigurable<double>  ReorderingJump::ScalingFactor(
        MAKE_CONFIG_PATH("ScalingFactor"),
        "Scaling factor for reordering jump model feature.",
        1.0);

Common::Configuration::tmplConfigurable<quint8>  ReorderingJump::MaximumJumpWidth(
        MAKE_CONFIG_PATH("MaximumJumpWidth"),
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
Common::Cost_t ReorderingJump::scoreSearchGraphNodeAndUpdateFutureHash(
        clsSearchGraphNode &_newHypothesisNode, QCryptographicHash &_hash) const
{
    Q_UNUSED(_hash);
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
 * @brief ReorderingJump::getRestCostForPosition calculates jump width from end of a series of zero
 * to start of another series of zeros.
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
    bool CurrentPositionCovered = false;
    bool NextPositionCovered = _coverage.at(0);
    size_t InputSentenceSize = _coverage.size();
    size_t JumpWidth = 0;

    size_t Position = 0;
    for(; Position < InputSentenceSize; ++Position)
    {
        bool LastPositionCovered = CurrentPositionCovered;
        CurrentPositionCovered = NextPositionCovered;
        NextPositionCovered = (Position + 1 == InputSentenceSize || _coverage.at(Position + 1));

        if( (Position == 0 || LastPositionCovered) && CurrentPositionCovered == 0 ) // finds first zero after a series of ones.
        {
            JumpWidth = std::abs((int)(LastPos - Position));
            SumJumpCost += ReorderingJump::getJumpCost(JumpWidth);
        }

        if(Position > 0 && CurrentPositionCovered == 0 && NextPositionCovered ) // finds first one after a series of zeros.
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

