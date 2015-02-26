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
namespace Core {
namespace Private {
namespace FeatureFunction{

using namespace Common;
using namespace SearchGraphBuilder;
using namespace Proxies;
using namespace RuleTable;

Common::Configuration::tmplConfigurable<double>  ReorderingJump::ScalingFactor(
        ReorderingJump::baseConfigPath() + "/ScalingFactor",
        "Scaling factor for reordering jump model feature.",
        1.0);

Common::Configuration::tmplConfigurable<quint8>  ReorderingJump::MaximumJumpWidth(
        ReorderingJump::baseConfigPath() + "/MaximumJumpWidth",
        "Maximum jump width.",
        6);

class clsReorderingJumpFeatureData : public intfFeatureFunctionData{
public:
    clsReorderingJumpFeatureData():
        intfFeatureFunctionData(1)
    {}
};

Common::Cost_t ReorderingJump::scoreSearchGraphNode(clsSearchGraphNode &_newHypothesisNode) const
{
    clsReorderingJumpFeatureData* Data = new clsReorderingJumpFeatureData;
    _newHypothesisNode.setFeatureFunctionData(this->DataIndex, Data);

    size_t JumpWidth = qAbs((int)_newHypothesisNode.sourceRangeBegin() -
                            (int)_newHypothesisNode.prevNode().sourceRangeEnd());

    Cost_t Cost = JumpWidth;

    if (JumpWidth > ReorderingJump::MaximumJumpWidth.value())
        Cost = JumpWidth + (JumpWidth * JumpWidth);

    if(gConfigs.WorkingMode.value() != enuWorkingModes::Decode)
        Data->CostElements[0] = Cost;

    return Cost * ReorderingJump::ScalingFactor.value();
}

}
}
}
}

