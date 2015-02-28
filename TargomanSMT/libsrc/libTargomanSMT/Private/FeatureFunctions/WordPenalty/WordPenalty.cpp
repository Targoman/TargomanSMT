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

#include "WordPenalty.h"

namespace Targoman {
namespace SMT {
namespace Private {
namespace FeatureFunction{

using namespace Common;
using namespace SearchGraph;
using namespace Proxies;
using namespace RuleTable;

TARGOMAN_REGISTER_SINGLETON_MODULE(WordPenalty);

Common::Configuration::tmplConfigurable<double>  WordPenalty::ScalingFactor(
        WordPenalty::baseConfigPath() + "/ScalingFactor",
        "Scaling factor for word penalty feature.",
        1.0);

/**
 * @brief The clsReorderingJumpFeatureData class is a derviation of intfFeatureFunctionData class.
 */
class clsWordPenaltyFeatureData : public intfFeatureFunctionData{
public:
    /**
     * @brief constructor of this class sets CostElements to 1 because we have cost for reordering jump feature.
     */
    clsWordPenaltyFeatureData():
        intfFeatureFunctionData(1)
    {}
};


/**
 * @brief WordPenalty::scoreSearchGraphNode   Sets CostElement value and computes WordPenalty based on
 * length of target phrase.
 * @return Returns score of ReorderingJumpfor this search graph node.
 */
Common::Cost_t WordPenalty::scoreSearchGraphNode(clsSearchGraphNode &_newHypothesisNode) const
{
    clsWordPenaltyFeatureData* Data = new clsWordPenaltyFeatureData;
    _newHypothesisNode.setFeatureFunctionData(this->DataIndex, Data);

    Cost_t Cost = (Cost_t)_newHypothesisNode.targetRule().size();

    if(gConfigs.WorkingMode.value() != enuWorkingModes::Decode)
        Data->CostElements[0] = Cost;

    return Cost * WordPenalty::ScalingFactor.value();
}


/**
 * @brief WordPenalty::initRootNode This function will be called in the constructor of searchGraphNode
 * in order to always have a valid previous node data for feature functions in scoreSearchGraphNode function.
 * @param _rootNode
 */
void WordPenalty::initRootNode(clsSearchGraphNode &_rootNode)
{
    _rootNode.setFeatureFunctionData(this->DataIndex, new clsWordPenaltyFeatureData);
}

}
}
}
}
