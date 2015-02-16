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

TARGOMAN_REGISTER_SINGLETON_MODULE(PhraseTable)

tmplConfigurable<double>    PhraseTable::ScalingFactors[] = {
    tmplConfigurable<double>(PhraseTable::baseConfigPath() + "/s2t",
    "TODO Desc",
    0),
    tmplConfigurable<double>(PhraseTable::baseConfigPath() + "/t2s",
    "TODO Desc",
    0),
    tmplConfigurable<double>(PhraseTable::baseConfigPath() + "/ibm1s2t",
    "TODO Desc",
    0),
    tmplConfigurable<double>(PhraseTable::baseConfigPath() + "/ibm1t2s",
    "TODO Desc",
    0),
    tmplConfigurable<double>(PhraseTable::baseConfigPath() + "/phrasePenalty",
    "TODO Desc",
    0),
    tmplConfigurable<double>(PhraseTable::baseConfigPath() + "/wordPenalty",
    "TODO Desc",
    0),
    tmplConfigurable<double>(PhraseTable::baseConfigPath() + "/s2tRatio",
    "TODO Desc",
    0),
    tmplConfigurable<double>(PhraseTable::baseConfigPath() + "/t2sRatio",
    "TODO Desc",
    0),
    tmplConfigurable<double>(PhraseTable::baseConfigPath() + "/cnt1",
    "TODO Desc",
    0),
    tmplConfigurable<double>(PhraseTable::baseConfigPath() + "/cnt2",
    "TODO Desc",
    0),
    tmplConfigurable<double>(PhraseTable::baseConfigPath() + "/cnt3",
    "TODO Desc",
    0)
};

void PhraseTable::initialize()
{
    for (int i=0; i< enuPhraseTableFields::getCount(); ++i){
        this->FieldIndexes.append(RuleTable::clsTargetRule::getColumnIndex(
                                      enuPhraseTableFields::toStr(
                                          (enuPhraseTableFields::Type)(i))));
    }
}

PhraseTable::PhraseTable():
    intfFeatureFunction(this->moduleName())
{
}

Common::Cost_t PhraseTable::getApproximateCost(unsigned _sourceStart,
                                               unsigned _sourceEnd,
                                               const RuleTable::clsTargetRule &_targetRule) const
{
    return this->getTargetRuleCost(_sourceStart, _sourceEnd, _targetRule);
}

Cost_t PhraseTable::getTargetRuleCost(unsigned _sourceStart,
                                      unsigned _sourceEnd,
                                      const RuleTable::clsTargetRule &_targetRule) const
{
    Q_UNUSED(_sourceStart)
    Q_UNUSED(_sourceEnd)

    Cost_t Cost = _targetRule.precomputedValue(this->PrecomputedIndex);
    if (Cost == -INFINITY){
        Cost = 0;
        for(int i=0; i< enuPhraseTableFields::getCount(); ++i)
            Cost += _targetRule.field(this->FieldIndexes.at(i)) * this->ScalingFactors[i].value();

        static_cast<RuleTable::clsTargetRule>(_targetRule).setPrecomputedValue(this->PrecomputedIndex, Cost);
    }
    return Cost;
}

}
}
}
}
