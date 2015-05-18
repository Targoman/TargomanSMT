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

#include "UnknownWordPenalty.h"

namespace Targoman {
namespace SMT {
namespace Private {
namespace FeatureFunction{

using namespace Common;
using namespace SearchGraphBuilder;
using namespace Proxies;
using namespace RuleTable;

TARGOMAN_REGISTER_SINGLETON_MODULE(UnknownWordPenalty);

Common::Configuration::tmplConfigurable<double>  UnknownWordPenalty::ScalingFactor(
        UnknownWordPenalty::baseConfigPath() + "/ScalingFactor",
        "Scaling factor for unknown word penalty feature.",
        1.0);

/**
 * @brief The clsReorderingJumpFeatureData class is a derviation of intfFeatureFunctionData class.
 */
class clsUnknownWordPenaltyFeatureData : public intfFeatureFunctionData{
public:
    /**
     * @brief constructor of this class sets CostElements to 1 because we have cost for reordering jump feature.
     */
    clsUnknownWordPenaltyFeatureData():
        intfFeatureFunctionData(1)
    {}

    intfFeatureFunctionData* copy() const {
        clsUnknownWordPenaltyFeatureData* Copy = new clsUnknownWordPenaltyFeatureData();
        Copy->CostElements[0] = this->CostElements[0];
        return Copy;
    }
};


/**
 * @brief UnknownWordPenalty::scoreSearchGraphNode   Sets CostElement value and computes UnknownWordPenalty based on
 * length of target phrase.
 * @return Returns score of ReorderingJumpfor this search graph node.
 */
Common::Cost_t UnknownWordPenalty::scoreSearchGraphNode(clsSearchGraphNode &_newHypothesisNode) const
{
    clsUnknownWordPenaltyFeatureData* Data = new clsUnknownWordPenaltyFeatureData;
    _newHypothesisNode.setFeatureFunctionData(this->DataIndex, Data);

    Cost_t Cost = 0;
//    for(size_t i = 0; i < _newHypothesisNode.targetRule().size(); ++i)
//        if(_newHypothesisNode.targetRule().at(i) == 0)
//            Cost += 100;
    if(_newHypothesisNode.targetRule().hasNoRuleTableRecord())
        Cost = 100;

    if(gConfigs.WorkingMode.value() != enuWorkingModes::Decode)
        Data->CostElements[0] = Cost;

    return Cost * UnknownWordPenalty::ScalingFactor.value();
}


/**
 * @brief UnknownWordPenalty::initRootNode This function will be called in the constructor of searchGraphNode
 * in order to always have a valid previous node data for feature functions in scoreSearchGraphNode function.
 * @param _rootNode
 */
void UnknownWordPenalty::initRootNode(clsSearchGraphNode &_rootNode)
{
    _rootNode.setFeatureFunctionData(this->DataIndex, new clsUnknownWordPenaltyFeatureData);
}

}
}
}
}
