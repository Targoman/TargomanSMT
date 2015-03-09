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
namespace SMT {
namespace Private {
namespace RuleTable {

TARGOMAN_ADD_EXCEPTION_HANDLER(exMosesPhraseTable, exRuleTable);

/**
 * @brief The clsMosesPlainRuleTable class is used to load moses phrase and reordering table.
 */
class clsMosesPlainRuleTable : intfRuleTable
{
public:
    clsMosesPlainRuleTable(quint64 _instanceID);
    ~clsMosesPlainRuleTable();

    void initializeSchema();
    void loadTableData();

private:
    void addRule(QList<clsRuleNode>& _ruleNodeList,
                 const QVector<Targoman::Common::WordIndex_t> _sourcePhrase,
                 const QList<Common::WordIndex_t> _targetPhrase,
                 const QList<Targoman::Common::Cost_t> _costs);
    void addRule(QList<clsRuleNode>& _ruleNodeList,
                 const QString& _sourcePhrase,
                 const QString& _targetPhrase,
                 const QStringList &_costs,
                 size_t _ruleNumber);
    void addUnkToUnkRule(QList<clsRuleNode>& _ruleNodeList);

private:
    int PhraseFeatureCount = 0;
    int ReorderingFeatureCount = 0;
    int PrecomputedValueIndex = -1;

private:
    static Targoman::Common::Configuration::tmplConfigurable<QString>   PhraseTableFilePath;            /**< File name of phrase table. */
    static Targoman::Common::Configuration::tmplConfigurable<QString>   ReorderingTableFilePath;        /**< File name of reordering table. */
    static Targoman::Common::Configuration::tmplConfigurable<int>       MaxRuleNodeTargetRuleCount;     /**< Maximum number of target rules kept for each rule node. */

    TARGOMAN_DEFINE_MODULE("MosesPTPlain", clsMosesPlainRuleTable)
};

}
}
}
}
#endif // TARGOMAN_CORE_PRIVATE_RULETABLE_CLSMOSESRULETABLE_H
