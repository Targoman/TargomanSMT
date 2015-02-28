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

#include "LanguageModel.h"
#include "Private/Proxies/intfLMSentenceScorer.hpp"

namespace Targoman {
namespace SMT {
namespace Private {
namespace FeatureFunction{

using namespace Common;
using namespace SearchGraph;
using namespace Proxies;
using namespace RuleTable;

TARGOMAN_REGISTER_SINGLETON_MODULE(LanguageModel);

Common::Configuration::tmplConfigurable<double>  LanguageModel::ScalingFactor(
        LanguageModel::baseConfigPath() + "/ScalingFactor",
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

    QScopedPointer<intfLMSentenceScorer> SentenceScorer;        /**< An instance of intfLMSentenceScorer to track previous seen words */
};

/**
 * @brief This function computes cost of language model with the help of previous node LM history.
 * @return Returns score of language model for this search graph node.
 */
Common::Cost_t LanguageModel::scoreSearchGraphNode(clsSearchGraphNode &_newHypothesisNode) const
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

    if(gConfigs.WorkingMode.value() != enuWorkingModes::Decode)
        Data->CostElements[0] = Cost;

    // For compatiblity reasons
    return Cost * log(10) * LanguageModel::ScalingFactor.value();
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
bool LanguageModel::nodesHaveSameState(const clsSearchGraphNode &_first, const clsSearchGraphNode &_second) const
{
    const clsLanguageModelFeatureData* FirstNodeData =
            static_cast<const clsLanguageModelFeatureData*>(_first.featureFunctionDataAt(this->DataIndex));
    const clsLanguageModelFeatureData* SecondNodeData =
            static_cast<const clsLanguageModelFeatureData*>(_second.featureFunctionDataAt(this->DataIndex));
    return FirstNodeData->SentenceScorer->haveSameHistoryAs(*SecondNodeData->SentenceScorer);
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
