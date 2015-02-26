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

#include "clsTargetRule.h"

#ifdef TARGOMAN_SHOW_DEBUG
#include "Private/GlobalConfigs.h"
#include "Private/LanguageModel/intfLMSentenceScorer.hpp"
#endif

namespace Targoman {
namespace Core {
namespace Private {
namespace RuleTable{

using namespace Common;

QStringList  clsTargetRule::ColumnNames;
size_t clsTargetRule::PrecomputedValuesSize = 0;

clsTargetRule* InvalidTargetRule = NULL;
clsTargetRuleData* InvalidTargetRuleData = NULL;

clsTargetRule::clsTargetRule():
    Data(InvalidTargetRuleData)
{}

clsTargetRule::clsTargetRule(const QList<WordIndex_t> &_targetPhrase, const QList<Cost_t> &_fields):
    Data(new clsTargetRuleData(_targetPhrase, _fields, clsTargetRule::PrecomputedValuesSize))
{

}

#ifdef TARGOMAN_SHOW_DEBUG

QString clsTargetRule::toStr() const
{
    QString result = gConfigs.EmptyLMScorer->getWordByIndex(this->Data->TargetPhrase.at(0));
    for(int i = 1; i < this->Data->TargetPhrase.size(); ++i)
        result += " " + gConfigs.EmptyLMScorer->getWordByIndex(this->Data->TargetPhrase.at(0));
    return result;
}

#endif

#ifdef TARGOMAN_SHOW_DEBUG

std::ostream& operator << (std::ostream& _outputStream, const clsTargetRule& _targetRule)
{
    if(_targetRule.fieldCount() == 0)
        return _outputStream;
    _outputStream << _targetRule.field(0);
    for(size_t i = 1; i < _targetRule.fieldCount(); ++i)
        _outputStream << " " << _targetRule.field(i);
    return _outputStream;
}

#endif

}
}
}
}
