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

namespace Targoman {
namespace Core {
namespace Private {
namespace RuleTable{

using namespace Common;

QStringList  clsTargetRule::ColumnNames;
size_t clsTargetRule::DefaultPrecomputedValuesSize = 0;

clsTargetRule::clsTargetRule() :
    Data(new clsTargetRuleData(clsTargetRule::DefaultPrecomputedValuesSize))
{
}

}
}
}
}
