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
namespace SMT {
namespace Private{
namespace FeatureFunction {

using namespace Common;
using namespace Common::Configuration;
using namespace SearchGraphBuilder;
using namespace InputDecomposer;
using namespace RuleTable;

TARGOMAN_REGISTER_SINGLETON_MODULE(LexicalReordering);

tmplConfigurable<bool>      LexicalReordering::IsBidirectional(
        LexicalReordering::baseConfigPath() + "/IsBidirectional",
        "Configs lexical reordering beahviour which can be bidirectional (default) or unidirectional",
        true);

tmplConfigurable<double>    LexicalReordering::ScalingFactors[] = {
    tmplConfigurable<double>(LexicalReordering::baseConfigPath() + "/ForwardMonotone",
    "Scaling factor for ForwardMonotone reordering score",
    1),
    tmplConfigurable<double>(LexicalReordering::baseConfigPath() + "/ForwardSwap",
    "Scaling factor for ForwardMonotone reordering score",
    1),
    tmplConfigurable<double>(LexicalReordering::baseConfigPath() + "/ForwardDiscontinous",
    "Scaling factor for ForwardMonotone reordering score",
    1),
    tmplConfigurable<double>(LexicalReordering::baseConfigPath() + "/BackwardMonotone",
    "Scaling factor for ForwardMonotone reordering score",
    1),
    tmplConfigurable<double>(LexicalReordering::baseConfigPath() + "/BackwardSwap",
    "Scaling factor for ForwardMonotone reordering score",
    1),
    tmplConfigurable<double>(LexicalReordering::baseConfigPath() + "/BackwardDiscontinous",
    "Scaling factor for ForwardMonotone reordering score",
    1)
};


/**
 * @brief The clsLexicalReorderingFeatureData class is a derviation of intfFeatureFunctionData class.
 */
class clsLexicalReorderingFeatureData : public intfFeatureFunctionData{
public:
    /**
     * @brief This constructor just calls the base class constructor with its cost element count.
     * @param _costElementsSize number of cost elements depends on whether
     * it is bidirectional or not.
     */
    clsLexicalReorderingFeatureData(size_t _costElementsSize):
        intfFeatureFunctionData(_costElementsSize)
    {}

    intfFeatureFunctionData* copy() const {
        clsLexicalReorderingFeatureData* Copy = new clsLexicalReorderingFeatureData(this->CostElements.size());
        for(int i = 0; i < this->CostElements.size(); ++i)
            Copy->CostElements[i] = this->CostElements[i];
        return Copy;
    }
};

/**
 * @brief LexicalReordering::initialize Stores field indices of lexical reordering table to #FieldIndexes.
 */

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

/**
 * @brief LexicalReordering::scoreSearchGraphNode   computes score of search graph node based on its orientation and wether it is bidirectional or not.
 * @param _newHypothesisNode input search graph node.
 * @return Returns score of lexical reordering search graph node.
 */

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

/**
 * @brief LexicalReordering::getApproximateCost Computes approximate cost of target rule using cost of all orientations.
 * @return Returns approximate cost.
 */

Common::Cost_t LexicalReordering::getApproximateCost(unsigned _sourceStart,
                                                     unsigned _sourceEnd,
                                                     const clsTargetRule &_targetRule) const
{
    Q_UNUSED(_sourceStart)
    Q_UNUSED(_sourceEnd)

    /*
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
    */
    Q_UNUSED(_targetRule)
    return 0;
}

bool LexicalReordering::nodesHaveSameState(const clsSearchGraphNode &_first, const clsSearchGraphNode &_second) const
{
    if(this->IsBidirectional.value()) {
        if(_first.prevNode().isInvalid() == false || _second.prevNode().isInvalid() == false)
            if(_first.prevNode().coverage() != _second.prevNode().coverage())
                return false;
    }

    for(int Orientation = enuLexicalReorderingFields::ForwardMonotone;
        Orientation <= enuLexicalReorderingFields::ForwardDiscontinous;
        ++Orientation) {
        if(_first.targetRule().field(LexicalReordering::FieldIndexes.at(Orientation)) !=
                _second.targetRule().field(LexicalReordering::FieldIndexes.at(Orientation)))
            return false;
    }
    return true;
}

/**
 * @brief LexicalReordering::getBackwardOreientation Checks whether orientation of input search graph node is "Backward Monotone", "Backward Swap" or "Backward Discontinous"
 * @param _newHypothesisNode input search graph node.
 * @return Return orientation of backward reordering.
 */

enuLexicalReorderingFields::Type LexicalReordering::getBackwardOreientation(
        SearchGraphBuilder::clsSearchGraphNode &_newHypothesisNode) const
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

/**
 * @brief LexicalReordering::getForwardOreientation Checks whether orientation of input search graph node is "Forward Monotone", "forward_as_tupleSwap" or "Forward Discontinous"
 * @param _newHypothesisNode   input search graph node.
 * @return  Return orientation of Forward reordering.
 */

enuLexicalReorderingFields::Type LexicalReordering::getForwardOreientation(
        SearchGraphBuilder::clsSearchGraphNode &_newHypothesisNode) const
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

/**
 * @brief LexicalReordering::initRootNode This function will be called in the constructor of searchGraphNode
 * in order to always have a valid previous node data for feature functions in scoreSearchGraphNode function.
 * @param _rootNode
 */

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
