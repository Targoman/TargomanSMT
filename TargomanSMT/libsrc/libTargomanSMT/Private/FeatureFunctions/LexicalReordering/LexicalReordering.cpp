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
        MAKE_CONFIG_PATH("IsBidirectional"),
        "Configs lexical reordering beahviour which can be bidirectional (default) or unidirectional",
        true);

tmplConfigurable<double>    LexicalReordering::ScalingFactors[] = {
    tmplConfigurable<double>(
    MAKE_CONFIG_PATH("ForwardMonotone"),
    "Scaling factor for ForwardMonotone reordering score",
    1),
    tmplConfigurable<double>(
    MAKE_CONFIG_PATH("ForwardSwap"),
    "Scaling factor for ForwardSwap reordering score",
    1),
    tmplConfigurable<double>(
    MAKE_CONFIG_PATH("ForwardDiscontinous"),
    "Scaling factor for ForwardDiscontinous reordering score",
    1),
    tmplConfigurable<double>(
    MAKE_CONFIG_PATH("BackwardMonotone"),
    "Scaling factor for BackwardMonotone reordering score",
    1),
    tmplConfigurable<double>(
    MAKE_CONFIG_PATH("BackwardSwap"),
    "Scaling factor for BackwardSwap reordering score",
    1),
    tmplConfigurable<double>(
    MAKE_CONFIG_PATH("BackwardDiscontinous"),
    "Scaling factor for BackwardDiscontinous reordering score",
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
    explicit clsLexicalReorderingFeatureData(size_t _costElementsSize):
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

void LexicalReordering::initialize(QSharedPointer<QSettings>)
{
    // Nothing to be done if there are no lexical reordering data
    // available
    if(clsTargetRule::lexicalReorderingAvailable() == false)
        return;
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

Common::Cost_t LexicalReordering::scoreSearchGraphNodeAndUpdateFutureHash(
        clsSearchGraphNode &_newHypothesisNode, QCryptographicHash &_hash) const
{
    if(clsTargetRule::lexicalReorderingAvailable() == false)
        return 0;

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

    // Too elaborate and unnecesary!
//    QByteArray RawData;
//    if(this->IsBidirectional.value()) {
//        if(_newHypothesisNode.prevNode().isInvalid() == false) {
//            const Coverage_t& PrevCoverage = _newHypothesisNode.prevNode().coverage();
//            RawData.resize(PrevCoverage.size() + 1);
//            for(int i = 0; i < PrevCoverage.size(); ++i)
//                RawData[i / 8] = RawData[i / 8] | (PrevCoverage.testBit(i) << (i % 8));
//        }
//    }
//    _hash.addData(RawData);
    if(this->IsBidirectional.value()) {
        if(_newHypothesisNode.prevNode().isInvalid() == false) {
            const Coverage_t& PrevCoverage = _newHypothesisNode.prevNode().coverage();
            _hash.addData((char*)&PrevCoverage, (
                              1 +  // Extra byte holding number of bits used from the last byte!
                              PrevCoverage.size() / 8 + // Number of bytes actually used by this bit array
                              (PrevCoverage.size() % 8 != 0 ? 1 : 0) // The last byte not all of which is used
                              ));
        }
    }

    const clsTargetRule& TargetRule = _newHypothesisNode.targetRule();
    for(int Orientation = enuLexicalReorderingFields::ForwardMonotone;
        Orientation <= enuLexicalReorderingFields::ForwardDiscontinous;
        ++Orientation) {
        double FieldValue = TargetRule.field(LexicalReordering::FieldIndexes.at(Orientation));
        _hash.addData((char*)&FieldValue, sizeof(double));
        //RawData.append((char*)&FieldValue, sizeof(double));
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

int LexicalReordering::compareStates(const clsSearchGraphNode &_first, const clsSearchGraphNode &_second) const
{
    if(clsTargetRule::lexicalReorderingAvailable() == false)
        return 0;
    if(this->IsBidirectional.value()) {
        if(_first.prevNode().isInvalid() == false || _second.prevNode().isInvalid() == false) {
            if(_first.prevNode().coverage() > _second.prevNode().coverage())
                return 1;
            else if(_first.prevNode().coverage() < _second.prevNode().coverage())
                return -1;
        }
    }

    for(int Orientation = enuLexicalReorderingFields::ForwardMonotone;
        Orientation <= enuLexicalReorderingFields::ForwardDiscontinous;
        ++Orientation) {
        double FirstFieldValue = _first.targetRule().field(LexicalReordering::FieldIndexes.at(Orientation));
        double SecondFieldValue = _second.targetRule().field(LexicalReordering::FieldIndexes.at(Orientation));
        if(FirstFieldValue > SecondFieldValue)
            return 1;
        else if(FirstFieldValue < SecondFieldValue)
            return -1;
    }
    return 0;
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
