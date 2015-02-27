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

#include "PhraseTable.h"

namespace Targoman {
namespace Core {
namespace Private {
namespace FeatureFunction{

using namespace Common;
using namespace Common::Configuration;
using namespace SearchGraph;
using namespace RuleTable;

TARGOMAN_REGISTER_SINGLETON_MODULE(PhraseTable)

clsFileBasedConfig   PhraseTable::ScalingFactorsConfigSection(
        PhraseTable::baseConfigPath() + "/ScalingFactors/");

QStringList   PhraseTable::ColumnNames;

class clsPhraseTableFeatureData : public intfFeatureFunctionData{
public:
    clsPhraseTableFeatureData(size_t _costElementsSize):
        intfFeatureFunctionData(_costElementsSize)
    {}
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

Cost_t PhraseTable::scoreSearchGraphNode(clsSearchGraphNode &_newHypothesisNode) const
{
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
