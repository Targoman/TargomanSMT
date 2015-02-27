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
namespace Core {
namespace Private {
namespace FeatureFunction{

using namespace Common;
using namespace SearchGraph;
using namespace Proxies;
using namespace RuleTable;

TARGOMAN_REGISTER_SINGLETON_MODULE(LanguageModel)

Common::Configuration::tmplConfigurable<double>  LanguageModel::ScalingFactor(
        LanguageModel::baseConfigPath() + "/ScalingFactor",
        "Scaling factor for language model feature.",
        1.0);

class clsLanguageModelFeatureData : public intfFeatureFunctionData {
public:
    clsLanguageModelFeatureData():
        intfFeatureFunctionData(1),
        SentenceScorer(gConfigs.LM.getInstance<intfLMSentenceScorer>())
    {}

    ~clsLanguageModelFeatureData(){}

    QScopedPointer<intfLMSentenceScorer> SentenceScorer;
};

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

    return Cost * LanguageModel::ScalingFactor.value();
}

Cost_t LanguageModel::getApproximateCost(unsigned _sourceStart,
                                         unsigned _sourceEnd,
                                         const RuleTable::clsTargetRule &_targetRule) const
{
    Q_UNUSED(_sourceStart)
    Q_UNUSED(_sourceEnd)

    Cost_t Cost = 0.0;
    QScopedPointer<intfLMSentenceScorer> SentenceScorer(gConfigs.LM.getInstance<intfLMSentenceScorer>());
    SentenceScorer->reset(false);
    for(size_t i = 0; i < _targetRule.size(); ++i)
        Cost -= SentenceScorer->wordProb(_targetRule.at(i));
    return Cost;
}

bool LanguageModel::nodesHaveSameState(const clsSearchGraphNode &_first, const clsSearchGraphNode &_second) const
{
    const clsLanguageModelFeatureData* FirstNodeData =
            static_cast<const clsLanguageModelFeatureData*>(_first.featureFunctionDataAt(this->DataIndex));
    const clsLanguageModelFeatureData* SecondNodeData =
            static_cast<const clsLanguageModelFeatureData*>(_second.featureFunctionDataAt(this->DataIndex));
    return FirstNodeData->SentenceScorer->haveSameHistoryAs(*SecondNodeData->SentenceScorer);
}

void LanguageModel::initRootNode(clsSearchGraphNode &_rootNode)
{
    _rootNode.setFeatureFunctionData(this->DataIndex, new clsLanguageModelFeatureData);
}

}
}
}
}
