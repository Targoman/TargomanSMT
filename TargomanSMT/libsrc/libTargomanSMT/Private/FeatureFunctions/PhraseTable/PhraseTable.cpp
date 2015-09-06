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

#include "PhraseTable.h"

namespace Targoman {
namespace SMT {
namespace Private {
namespace FeatureFunction{

using namespace Common;
using namespace Common::Configuration;
using namespace SearchGraphBuilder;
using namespace RuleTable;

TARGOMAN_REGISTER_SINGLETON_MODULE(PhraseTable);

clsFileBasedConfig   PhraseTable::ScalingFactorsConfigSection(
        PhraseTable::baseConfigPath() + "/ScalingFactors/");

QStringList   PhraseTable::ColumnNames;

/**
 * @brief This constructor just calls the base class constructor with its cost element count.
 * @param _costElementsSize number of cost elements can be infered from size of ColumnNames.
 */
class clsPhraseTableFeatureData : public intfFeatureFunctionData{
public:
    clsPhraseTableFeatureData(size_t _costElementsSize):
        intfFeatureFunctionData(_costElementsSize)
    {}

    intfFeatureFunctionData* copy() const {
        clsPhraseTableFeatureData* Copy = new clsPhraseTableFeatureData(this->CostElements.size());
        for(int i = 0; i < this->CostElements.size(); ++i)
            Copy->CostElements[i] = this->CostElements[i];
        return Copy;
    }
};

/**
 * @brief PhraseTable::initialize Loads and stores scale factor and field indices of phrase table.
 * @param _configFile Address of input config file.
 * @exception throws exception if phrase table loader has not initialized phrase table column names.
 */
void PhraseTable::initialize(const QString& _configFile)
{
    if (this->ColumnNames.isEmpty())
        throw exPhraseTable("Seems that RuleTable loader has not initialized phrase table column names.");

    QSettings ConfigFile(_configFile, QSettings::IniFormat);
    ConfigFile.beginGroup(PhraseTable::ScalingFactorsConfigSection.configPath());
    foreach(const QString& ColumnName, this->ColumnNames){
        if (ConfigFile.value(ColumnName).isValid()){
            bool Ok;
            double ScalingFactor = ConfigFile.value(ColumnName).toDouble(&Ok);
            if (!Ok)
                throw exPhraseTable("Invalid scaling factor defined for column: "+ ColumnName);
            this->ScalingFactors.append(ScalingFactor);
            this->FieldIndexes.append(
                        RuleTable::clsTargetRule::getColumnIndex(ColumnName, this->moduleName()));
        }else
            throw exPhraseTable("No scaling factor found for column: "+ ColumnName);
    }
}

/**
 * @brief PhraseTable::scoreSearchGraphNode   Sets CostElements values and compute phrase cost.
 * @return Returns score of phrase table for this search graph node.
 */
Cost_t PhraseTable::scoreSearchGraphNodeAndUpdateFutureHash(clsSearchGraphNode &_newHypothesisNode, QCryptographicHash &_hash) const
{
    Q_UNUSED(_hash);
    if(gConfigs.WorkingMode.value() != enuWorkingModes::Decode) {
        clsPhraseTableFeatureData* Data = new clsPhraseTableFeatureData(this->ColumnNames.size());
        _newHypothesisNode.setFeatureFunctionData(this->DataIndex, Data);
        for(int i = 0; i < this->ColumnNames.size(); ++i)
            Data->CostElements[i] = _newHypothesisNode.targetRule().field(this->FieldIndexes.at(i));
    }

    return getPhraseCost(_newHypothesisNode.targetRule());
}

/**
 * @brief PhraseTable::getPhraseCost Computes target rule cost.
 * This function computes an inner product of target rule costs, corresponding to this feature function, with scale factors of phrase table.
 * This function also stores Computed cost to precomputedValue data of target rule.
 * @param _targetRule input target rule.
 * @return returns cost of this target rule for this feature (rule table).
 */
Cost_t PhraseTable::getPhraseCost(const clsTargetRule &_targetRule) const
{
    Cost_t Cost = _targetRule.precomputedValue(this->PrecomputedIndex);
    if (Cost == -INFINITY){
        Cost = 0;
        for(int i=0; i< this->FieldIndexes.size(); ++i)
            Cost += _targetRule.field(this->FieldIndexes.at(i)) * this->ScalingFactors.at(i);
        static_cast<RuleTable::clsTargetRule>(_targetRule).setPrecomputedValue(
                this->PrecomputedIndex, Cost);
    }
    return Cost;
}


/**
 * @brief PhraseTable::initRootNode This function will be called in the constructor of searchGraphNode
 * in order to always have a valid previous node data for feature functions in scoreSearchGraphNode function.
 * @param _rootNode
 */
void PhraseTable::initRootNode(clsSearchGraphNode &_rootNode)
{
    _rootNode.setFeatureFunctionData(this->DataIndex,
                                     new clsPhraseTableFeatureData(
                                         this->ColumnNames.size()));
}

}
}
}
}
