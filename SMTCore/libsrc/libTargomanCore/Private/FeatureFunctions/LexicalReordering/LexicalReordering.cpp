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

TARGOMAN_REGISTER_SINGLETON_MODULE(LexicalReordering)

tmplConfigurable<bool>      LexicalReordering::IsBidirectional(
        LexicalReordering::baseConfigPath() + "/IsBidirectional",
        "Configs lexical reordering beahviour which can be bidirectional (default) or unidirectional",
        true);

tmplConfigurable<double>    LexicalReordering::ScalingFactors[] = {
    tmplConfigurable<double>(LexicalReordering::baseConfigPath() + "/LeftMonotone",
    "Scaling factor for LeftMonotone reordering score",
    1),
    tmplConfigurable<double>(LexicalReordering::baseConfigPath() + "/RightMonotone",
    "Scaling factor for LeftMonotone reordering score",
    1),
    tmplConfigurable<double>(LexicalReordering::baseConfigPath() + "/LeftSwap",
    "Scaling factor for LeftMonotone reordering score",
    1),
    tmplConfigurable<double>(LexicalReordering::baseConfigPath() + "/RightSwap",
    "Scaling factor for LeftMonotone reordering score",
    1),
    tmplConfigurable<double>(LexicalReordering::baseConfigPath() + "/LeftDiscontinous",
    "Scaling factor for LeftMonotone reordering score",
    1),
    tmplConfigurable<double>(LexicalReordering::baseConfigPath() + "/RightDiscontinous",
    "Scaling factor for LeftMonotone reordering score",
    1)
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

void LexicalReordering::newSentence(const InputDecomposer::Sentence_t &_inputSentence)
{
    Q_UNUSED(_inputSentence)
}

Common::Cost_t LexicalReordering::scoreSearchGraphNode(SearchGraphBuilder::clsSearchGraphNode &_newHypothesisNode) const
{
    enuLexicalReorderingFields::Type Orientation = this->getLeftOreientation(_newHypothesisNode);
    Cost_t Cost = _newHypothesisNode.targetRule().field(
                this->FieldIndexes.at(Orientation)) * this->ScalingFactors[Orientation].value();

    //TODO remove this
    return Cost;

    if (this->IsBidirectional.value()){
        Orientation = getRightOreientation(_newHypothesisNode);
        Cost += _newHypothesisNode.prevNode().targetRule().field(
                    this->FieldIndexes.at(Orientation)) * this->ScalingFactors[Orientation].value();
    }

    return Cost;
}

Common::Cost_t LexicalReordering::getApproximateCost(unsigned _sourceStart, unsigned _sourceEnd, const RuleTable::clsTargetRule &_targetRule)
{
    Q_UNUSED(_sourceStart)
    Q_UNUSED(_sourceEnd)

    Cost_t Cost = 0;
    for (int i = enuLexicalReorderingFields::LeftMonotone; i<= enuLexicalReorderingFields::LeftDiscontinous; ++i){
        Cost += _targetRule.field(this->FieldIndexes.at(i)) * this->ScalingFactors[i].value();
    }

    //TODO remove this
    return Cost;

    if (this->IsBidirectional.value()){
        for (int i = enuLexicalReorderingFields::RightMonotone; i<= enuLexicalReorderingFields::RightDiscontinous; ++i){
            Cost += _targetRule.field(this->FieldIndexes.at(i)) * this->ScalingFactors[i].value();
        }
    }
    return Cost;
}

LexicalReordering::LexicalReordering() :
    intfFeatureFunction(this->moduleName())
{

}

enuLexicalReorderingFields::Type LexicalReordering::getRightOreientation(SearchGraphBuilder::clsSearchGraphNode &_newHypothesisNode) const
{
    if (_newHypothesisNode.sourceRangeBegin() == _newHypothesisNode.prevNode().sourceRangeEnd())
        return enuLexicalReorderingFields::RightMonotone;
    else if (_newHypothesisNode.sourceRangeEnd() == _newHypothesisNode.prevNode().sourceRangeBegin())
        return enuLexicalReorderingFields::RightSwap;
    else
        return enuLexicalReorderingFields::RightDiscontinous;
}

enuLexicalReorderingFields::Type LexicalReordering::getLeftOreientation(SearchGraphBuilder::clsSearchGraphNode &_newHypothesisNode) const
{
    if (_newHypothesisNode.sourceRangeBegin() == _newHypothesisNode.prevNode().sourceRangeEnd())
        return enuLexicalReorderingFields::LeftSwap;
    else if (_newHypothesisNode.sourceRangeEnd() == _newHypothesisNode.prevNode().sourceRangeBegin())
        return enuLexicalReorderingFields::LeftMonotone;
    else
        return enuLexicalReorderingFields::LeftDiscontinous;
}

}
}
}
}