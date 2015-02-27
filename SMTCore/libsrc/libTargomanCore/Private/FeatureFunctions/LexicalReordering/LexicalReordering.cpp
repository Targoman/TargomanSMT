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

#include "LexicalReordering.h"
#include "Private/RuleTable/clsRuleNode.h"

namespace Targoman{
namespace Core {
namespace Private{
namespace FeatureFunction {

using namespace Common;
using namespace Common::Configuration;
using namespace SearchGraph;
using namespace InputDecomposer;
using namespace RuleTable;

TARGOMAN_REGISTER_SINGLETON_MODULE(LexicalReordering)

tmplConfigurable<bool>      LexicalReordering::IsBidirectional(
        LexicalReordering::baseConfigPath() + "/IsBidirectional",
        "Configs lexical reordering beahviour which can be bidirectional (default) or unidirectional",
        true);

tmplConfigurable<double>    LexicalReordering::ScalingFactors[] = {
    tmplConfigurable<double>(LexicalReordering::baseConfigPath() + "/ForwardMonotone",
    "Scaling factor for ForwardMonotone reordering score",
    1),
    tmplConfigurable<double>(LexicalReordering::baseConfigPath() + "/BackwardMonotone",
    "Scaling factor for ForwardMonotone reordering score",
    1),
    tmplConfigurable<double>(LexicalReordering::baseConfigPath() + "/ForwardSwap",
    "Scaling factor for ForwardMonotone reordering score",
    1),
    tmplConfigurable<double>(LexicalReordering::baseConfigPath() + "/BackwardSwap",
    "Scaling factor for ForwardMonotone reordering score",
    1),
    tmplConfigurable<double>(LexicalReordering::baseConfigPath() + "/ForwardDiscontinous",
    "Scaling factor for ForwardMonotone reordering score",
    1),
    tmplConfigurable<double>(LexicalReordering::baseConfigPath() + "/BackwardDiscontinous",
    "Scaling factor for ForwardMonotone reordering score",
    1)
};


class clsLexicalReorderingFeatureData : public intfFeatureFunctionData{
public:
    clsLexicalReorderingFeatureData(size_t _costElementsSize):
        intfFeatureFunctionData(_costElementsSize)
    {}
};

void LexicalReordering::initialize(const QString &)
{
    for (int i=0;
         i< (LexicalReordering::IsBidirectional.value() ?
             enuLexicalReorderingFields::getCount() :
             enuLexicalReorderingFields::getCount() / 2
             );
             ++i){
        this->FieldIndexes.append(RuleTable::clsTargetRule::getColumnIndex(
                                      enuLexicalReorderingFields::toStr(
                                          (enuLexicalReorderingFields::Type)(i)),
                                      this->moduleName() + (
                                      LexicalReordering::IsBidirectional.value() ?
                                          ":Bidirectional" :
                                          ":Monodirectional")));
    }
}

void LexicalReordering::newSentence(const Sentence_t &_inputSentence)
{
    Q_UNUSED(_inputSentence)
}

Common::Cost_t LexicalReordering::scoreSearchGraphNode(clsSearchGraphNode &_newHypothesisNode) const
{
    clsLexicalReorderingFeatureData* Data =
            new clsLexicalReorderingFeatureData(this->IsBidirectional.value() ? 6 : 3);
    _newHypothesisNode.setFeatureFunctionData(this->DataIndex, Data);

    enuLexicalReorderingFields::Type Orientation = this->getForwardOreientation(_newHypothesisNode);
    Cost_t Cost =
            _newHypothesisNode.prevNode().targetRule().field(this->FieldIndexes.at(Orientation)) *
            this->ScalingFactors[Orientation].value();

    if(gConfigs.WorkingMode.value() != enuWorkingModes::Decode) {
        Data->CostElements[Orientation] =
                _newHypothesisNode.prevNode().targetRule().field(this->FieldIndexes.at(Orientation));
    }

    if (this->IsBidirectional.value()) {
        Orientation = getBackwardOreientation(_newHypothesisNode);
        Cost +=
                _newHypothesisNode.targetRule().field(this->FieldIndexes.at(Orientation)) *
                this->ScalingFactors[Orientation].value();
        if(gConfigs.WorkingMode.value() != enuWorkingModes::Decode) {
            Data->CostElements[Orientation] =
                    _newHypothesisNode.targetRule().field(this->FieldIndexes.at(Orientation));
        }
    }

    return Cost;
}

Common::Cost_t LexicalReordering::getApproximateCost(unsigned _sourceStart,
                                                     unsigned _sourceEnd,
                                                     const clsTargetRule &_targetRule) const
{
    Q_UNUSED(_sourceStart)
    Q_UNUSED(_sourceEnd)

    Cost_t Cost = 0;
    for (int i = enuLexicalReorderingFields::ForwardMonotone; i<= enuLexicalReorderingFields::ForwardDiscontinous; ++i){
        Cost += _targetRule.field(this->FieldIndexes.at(i)) * this->ScalingFactors[i].value();
    }

    if (this->IsBidirectional.value()){
        for (int i = enuLexicalReorderingFields::BackwardMonotone;
             i<= enuLexicalReorderingFields::BackwardDiscontinous;
             ++i){
            Cost += _targetRule.field(this->FieldIndexes.at(i)) * this->ScalingFactors[i].value();
        }
    }
    return Cost;
}

enuLexicalReorderingFields::Type LexicalReordering::getBackwardOreientation(
        SearchGraph::clsSearchGraphNode &_newHypothesisNode) const
{
    if (_newHypothesisNode.prevNode().isInvalid()) {
        if (_newHypothesisNode.sourceRangeBegin() == 0)
            return enuLexicalReorderingFields::BackwardMonotone;
        else
            return enuLexicalReorderingFields::BackwardDiscontinous;
    }
    if (_newHypothesisNode.sourceRangeBegin() == _newHypothesisNode.prevNode().sourceRangeEnd())
        return enuLexicalReorderingFields::BackwardMonotone;
    else if (_newHypothesisNode.sourceRangeEnd() == _newHypothesisNode.prevNode().sourceRangeBegin())
        return enuLexicalReorderingFields::BackwardSwap;
    else
        return enuLexicalReorderingFields::BackwardDiscontinous;
}

enuLexicalReorderingFields::Type LexicalReordering::getForwardOreientation(
        SearchGraph::clsSearchGraphNode &_newHypothesisNode) const
{
    if (_newHypothesisNode.prevNode().isInvalid()) {
        if (_newHypothesisNode.sourceRangeBegin() == 0)
            return enuLexicalReorderingFields::ForwardMonotone;
        else
            return enuLexicalReorderingFields::ForwardDiscontinous;
    }
    if (_newHypothesisNode.sourceRangeBegin() == _newHypothesisNode.prevNode().sourceRangeEnd())
        return enuLexicalReorderingFields::ForwardMonotone;
    else if (_newHypothesisNode.sourceRangeEnd() == _newHypothesisNode.prevNode().sourceRangeBegin())
        return enuLexicalReorderingFields::ForwardSwap;
    else
        return enuLexicalReorderingFields::ForwardDiscontinous;
}

void LexicalReordering::initRootNode(clsSearchGraphNode &_rootNode)
{
    _rootNode.setFeatureFunctionData(this->DataIndex,
                                     new clsLexicalReorderingFeatureData(
                                         this->IsBidirectional.value() ? 6 : 3));
}

}
}
}
}
