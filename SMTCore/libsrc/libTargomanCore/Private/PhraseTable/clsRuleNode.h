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
 */

#ifndef TARGOMAN_CORE_PRIVATE_PHRASETABLE_RULENODE_H
#define TARGOMAN_CORE_PRIVATE_PHRASETABLE_RULENODE_H

#include <QList>
#include "clsTargetRule.h"

namespace Targoman {
namespace Core {
namespace Private {
namespace PhraseTable{

class clsRuleNode
{
public:
    clsRuleNode();

private:
    QList<clsTargetRule> TargetRules;

};

}
}
}
}

#endif // TARGOMAN_CORE_PRIVATE_PHRASETABLE_RULENODE_H
