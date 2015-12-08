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

#include "LanguageModel.h"
#include "Private/Proxies/LanguageModel/intfLMSentenceScorer.hpp"

namespace Targoman {
namespace SMT {
namespace Private {
namespace FeatureFunction{

using namespace Common;
using namespace SearchGraphBuilder;
using namespace Proxies::LanguageModel;
using namespace Proxies::LanguageModel;
using namespace RuleTable;

TARGOMAN_REGISTER_SINGLETON_MODULE(LanguageModel);

Common::Configuration::tmplConfigurable<double>  LanguageModel::ScalingFactor(
        MAKE_CONFIG_PATH("ScalingFactor"),
        "Scaling factor for language model feature.",
        1.0);

/**
 * @brief The clsLanguageModelFeatureData class is a derviation of intfFeatureFunctionData class to store specific
 * data correspond to language model feature funciton. In addition to CostElements data memeber this overloaded class
 * has an instance of intfLMSentenceScorer class to track its state.
 */

class clsLanguageModelFeatureData : public intfFeatureFunctionData {
public:
    /**
     * @brief constructor of this class resizes costElements to 1 in order to store language model cost
     * of search graph node.
     */
    clsLanguageModelFeatureData():
        intfFeatureFunctionData(1),
        SentenceScorer(gConfigs.LM.getInstance<intfLMSentenceScorer>())
    {}

    ~clsLanguageModelFeatureData(){}

    intfFeatureFunctionData* copy() const {
        clsLanguageModelFeatureData* Copy = new clsLanguageModelFeatureData;
        Copy->CostElements[0] = this->CostElements[0];
        Copy->SentenceScorer->initHistory(*this->SentenceScorer);
        return Copy;
    }

    QScopedPointer<intfLMSentenceScorer> SentenceScorer;        /**< An instance of intfLMSentenceScorer to track previous seen words */
};

/**
 * @brief This function computes cost of language model with the help of previous node LM history.
 * @return Returns score of language model for this search graph node.
 */
Common::Cost_t LanguageModel::scoreSearchGraphNodeAndUpdateFutureHash(clsSearchGraphNode &_newHypothesisNode, QCryptographicHash &_hash) const
{
    const clsLanguageModelFeatureData* PrevNodeData =
            static_cast<const clsLanguageModelFeatureData*>
            (_newHypothesisNode.prevNode().featureFunctionDataAt(this->DataIndex));
    clsLanguageModelFeatureData* Data = new clsLanguageModelFeatureData;
    _newHypothesisNode.setFeatureFunctionData(this->DataIndex, Data);

    Data->SentenceScorer->initHistory(*PrevNodeData->SentenceScorer);
    Cost_t Cost = 0.0;
    const clsTargetRule& TargetRule = _newHypothesisNode.targetRule();
    for(size_t i = 0; i < TargetRule.size(); ++i)
        Cost -= Data->SentenceScorer->wordProb(TargetRule.at(i));

    if (_newHypothesisNode.isFinal())
        Cost -= Data->SentenceScorer->endOfSentenceProb();

    Data->SentenceScorer->updateFutureStateHash(_hash);

    if(gConfigs.WorkingMode.value() != enuWorkingModes::Decode)
        Data->CostElements[0] = Cost;

    // For compatiblity reasons
    return Cost * log(10) * LanguageModel::ScalingFactor.value();
}

Cost_t LanguageModel::getApproximateCost(unsigned _sourceStart, unsigned _sourceEnd, const clsTargetRule &_targetRule) const
{
    Q_UNUSED(_sourceStart)
    Q_UNUSED(_sourceEnd)
    return this->getLanguageModelCost(_targetRule);
}

/**
 * @brief Computes language model score for the given target rule
 */
Cost_t LanguageModel::getLanguageModelCost(const RuleTable::clsTargetRule &_targetRule) const
{
    Cost_t Cost = 0.0;
    QScopedPointer<intfLMSentenceScorer> SentenceScorer(gConfigs.LM.getInstance<intfLMSentenceScorer>());
    SentenceScorer->reset(false);
    for(size_t i = 0; i < _targetRule.size(); ++i)
        Cost -= SentenceScorer->wordProb(_targetRule.at(i));

    // For compatiblity reasons
    return Cost * log(10) * LanguageModel::ScalingFactor.value();
}

/**
 * @brief nodesHaveSameState checks equality of states of two search graph noedes.
 * Checks whether first and second node have same sentence score history or not.
 * @param _first First node.
 * @param _second Second node.
 * @return returns true if both of nodes have same state.
 */
int LanguageModel::compareStates(const clsSearchGraphNode &_first, const clsSearchGraphNode &_second) const
{
    const clsLanguageModelFeatureData* FirstNodeData =
            static_cast<const clsLanguageModelFeatureData*>(_first.featureFunctionDataAt(this->DataIndex));
    const clsLanguageModelFeatureData* SecondNodeData =
            static_cast<const clsLanguageModelFeatureData*>(_second.featureFunctionDataAt(this->DataIndex));
    return FirstNodeData->SentenceScorer->compareHistoryWith(*SecondNodeData->SentenceScorer);
}

/**
 * @brief This function will be called in the constructor of searchGraphNode
 * in order to always have a valid previous node data for feature functions in scoreSearchGraphNode function.
 * @param _rootNode
 */
void LanguageModel::initRootNode(clsSearchGraphNode &_rootNode)
{
    _rootNode.setFeatureFunctionData(this->DataIndex, new clsLanguageModelFeatureData);
}

}
}
}
}
