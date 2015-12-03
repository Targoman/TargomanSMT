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

#include "WordPenalty.h"

namespace Targoman {
namespace SMT {
namespace Private {
namespace FeatureFunction{

using namespace Common;
using namespace SearchGraphBuilder;
using namespace Proxies;
using namespace RuleTable;

TARGOMAN_REGISTER_SINGLETON_MODULE(WordPenalty);

Common::Configuration::tmplConfigurable<double>  WordPenalty::ScalingFactor(
        MAKE_CONFIG_PATH("ScalingFactor"),
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

    intfFeatureFunctionData* copy() const {
        clsWordPenaltyFeatureData* Copy = new clsWordPenaltyFeatureData();
        Copy->CostElements[0] = this->CostElements[0];
        return Copy;
    }
};


/**
 * @brief WordPenalty::scoreSearchGraphNode   Sets CostElement value and computes WordPenalty based on
 * length of target phrase.
 * @return Returns score of ReorderingJumpfor this search graph node.
 */
Common::Cost_t WordPenalty::scoreSearchGraphNodeAndUpdateFutureHash(clsSearchGraphNode &_newHypothesisNode, QCryptographicHash &_hash) const
{
    Q_UNUSED(_hash);
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
