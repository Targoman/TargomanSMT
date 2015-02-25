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

#ifndef TARGOMAN_CORE_PRIVATE_RULETABLE_CLSMOSESRULETABLE_H
#define TARGOMAN_CORE_PRIVATE_RULETABLE_CLSMOSESRULETABLE_H

#include "libTargomanCommon/Configuration/tmplConfigurable.h"
#include "libTargomanCommon/PrefixTree/tmplFullVectorFilePrefixTree.hpp"
#include "intfRuleTable.hpp"
#include "clsRuleNode.h"

namespace Targoman {
namespace Core {
namespace Private {
namespace RuleTable {

TARGOMAN_ADD_EXCEPTION_HANDLER(exMosesPhraseTable, exRuleTable);

class clsMosesPlainRuleTable : intfRuleTable
{
public:
    clsMosesPlainRuleTable(quint64 _instanceID);
    ~clsMosesPlainRuleTable();

    virtual void initializeSchema();
    virtual void loadTableData();

private:
    void addToRuleNodeSorted(clsRuleNode &_ruleNode, clsTargetRule &_targetRule);
    void addRule(const QString& _sourcePhrase,
                 const QString& _targetPhrase,
                 const QStringList &_costs,
                 size_t _ruleNumber);
private:
    int PhraseFeatureCount = 0;
    int ReorderingFeatureCount = 0;

private:
    static Targoman::Common::Configuration::tmplConfigurable<QString> PhraseTableFileName;
    static Targoman::Common::Configuration::tmplConfigurable<QString> ReorderingTableFileName;

    TARGOMAN_DEFINE_MODULE("MosesPTPlain", clsMosesPlainRuleTable)
};

}
}
}
}
#endif // TARGOMAN_CORE_PRIVATE_RULETABLE_CLSMOSESRULETABLE_H
