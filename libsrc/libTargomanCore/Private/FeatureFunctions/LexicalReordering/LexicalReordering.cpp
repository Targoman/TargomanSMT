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
        "TODO",
        true);

tmplConfigurable<double>    LexicalReordering::ScalingFactors[] = {
    tmplConfigurable<double>(LexicalReordering::baseConfigPath() + "/LeftMonotone",
    "TODO",
    0),
    tmplConfigurable<double>(LexicalReordering::baseConfigPath() + "/RightMonotone",
    "TODO",
    0),
    tmplConfigurable<double>(LexicalReordering::baseConfigPath() + "/LeftSwap",
    "TODO",
    0),
    tmplConfigurable<double>(LexicalReordering::baseConfigPath() + "/RightSwap",
    "TODO",
    0),
    tmplConfigurable<double>(LexicalReordering::baseConfigPath() + "/LeftDiscontinous",
    "TODO",
    0),
    tmplConfigurable<double>(LexicalReordering::baseConfigPath() + "/RightDiscontinous",
    "TODO",
    0)
};



void LexicalReordering::initialize()
{
    for (int i=0; i< enuLexicalReorderingFields::getCount(); ++i){
        this->FieldIndexes.append(RuleTable::clsTargetRule::getColumnIndex(
                                      enuLexicalReorderingFields::toStr(
                                          (enuLexicalReorderingFields::Type)(i))));
    }
}

void LexicalReordering::newSentence(const InputDecomposer::Sentence_t &inputSentence)
{

}

Common::Cost_t LexicalReordering::scoreSearchGraphNode(SearchGraphBuilder::clsSearchGraphNode &_newHypothesisNode) const
{
    enuLexicalReorderingFields::Type Orientation = this->getLeftOreientation(_newHypothesisNode);
    Cost_t Cost = _newHypothesisNode.targetRule().field(
                this->FieldIndexes.at(Orientation)) * this->ScalingFactors[Orientation].value();

    if (this->IsBidirectional.value()){
        Orientation = getRightOreientation(_newHypothesisNode);
        Cost += _newHypothesisNode.prevNode().targetRule().field(
                    this->FieldIndexes.at(Orientation)) * this->ScalingFactors[Orientation].value();
    }

    return Cost;
}

Common::Cost_t LexicalReordering::getApproximateCost(unsigned _sourceStart, unsigned _sourceEnd, const RuleTable::clsTargetRule &_targetRule)
{
    Cost_t Cost = 0;
    for (int i = enuLexicalReorderingFields::LeftMonotone; i<= enuLexicalReorderingFields::LeftDiscontinous; ++i){
        Cost += _targetRule.field(this->FieldIndexes.at(i)) * this->ScalingFactors[i].value();
    }
    if (this->IsBidirectional.value()){
        for (int i = enuLexicalReorderingFields::RightMonotone; i<= enuLexicalReorderingFields::RightDiscontinous; ++i){
            Cost += _targetRule.field(this->FieldIndexes.at(i)) * this->ScalingFactors[i].value();
        }
    }
    return Cost;
}

LexicalReordering::LexicalReordering():
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