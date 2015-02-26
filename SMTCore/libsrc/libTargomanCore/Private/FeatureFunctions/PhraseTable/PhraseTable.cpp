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
using namespace SearchGraphBuilder;

TARGOMAN_REGISTER_SINGLETON_MODULE(PhraseTable)

clsFileBasedConfig   PhraseTable::ScalingFactorsConfigSection(
        PhraseTable::baseConfigPath() + "/ScalingFactors/");

QStringList   PhraseTable::ColumnNames;

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

PhraseTable::PhraseTable() :
    intfFeatureFunction(this->moduleName())
{

}

Cost_t PhraseTable::getTargetRuleCost(unsigned _sourceStart,
                                      unsigned _sourceEnd,
                                      const RuleTable::clsTargetRule &_targetRule) const
{
    Q_UNUSED(_sourceStart)
    Q_UNUSED(_sourceEnd)

    Cost_t Cost = 0;
    for(int i=0; i< this->FieldIndexes.size(); ++i)
        Cost += _targetRule.field(this->FieldIndexes.at(i)) * this->ScalingFactors.at(i);
    return Cost;
    //TODO remove above using below cached implementation

/*    //Cost_t Cost = _targetRule.precomputedValue(this->PrecomputedIndex);
    if (Cost == -INFINITY){
        Cost = 0;
    for(int i=0; i< this->FieldIndexes.size(); ++i)
        Cost += _targetRule.field(this->FieldIndexes.at(i)) * this->ScalingFactors.at(i);

        static_cast<RuleTable::clsTargetRule>(_targetRule).setPrecomputedValue(this->PrecomputedIndex, Cost);
    }
    return Cost;*/
}

}
}
}
}
