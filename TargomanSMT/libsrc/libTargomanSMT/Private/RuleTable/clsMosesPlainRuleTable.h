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

#ifndef TARGOMAN_CORE_PRIVATE_RULETABLE_CLSMOSESRULETABLE_H
#define TARGOMAN_CORE_PRIVATE_RULETABLE_CLSMOSESRULETABLE_H

#include "libTargomanCommon/Configuration/tmplConfigurable.h"
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
    explicit clsMosesPlainRuleTable();
    ~clsMosesPlainRuleTable();

    void initializeSchema();
    void loadTableData();

private:
    void addRule(QList<clsRuleNode>& _ruleNodeList,
                 const QList<Common::WordIndex_t> _sourcePhrase,
                 const QList<Common::WordIndex_t> _targetPhrase,
                 const QList<Targoman::Common::Cost_t> _costs, const QMap<int, int> _alignment);
    void addRule(QList<clsRuleNode>& _ruleNodeList,
                 const QString& _sourcePhrase,
                 const QString& _targetPhrase,
                 const QStringList &_costs,
                 const QStringList &_wordAlignments,
                 size_t _ruleNumber);
    void addUnkToUnkRule(QList<clsRuleNode>& _ruleNodeList);

private:
    int PhraseFeatureCount = 0;
    int ReorderingFeatureCount = 0;
    int PrecomputedValueIndex = -1;

private:
    static Targoman::Common::Configuration::tmplConfigurable<FilePath_t>   PhraseTableFilePath;            /**< File name of phrase table. */
    static Targoman::Common::Configuration::tmplConfigurable<FilePath_t>   ReorderingTableFilePath;        /**< File name of reordering table. */
    static Targoman::Common::Configuration::tmplConfigurable<QString>   WordAlignmentFilePath;        /**< File name of word level alignment of phrases. */
    static Targoman::Common::Configuration::tmplConfigurable<int>       MaxRuleNodeTargetRuleCount;     /**< Maximum number of target rules kept for each rule node. */

    TARGOMAN_DEFINE_MODULE(MosesPlainRuleTable);
};

}
}
}
}
#endif // TARGOMAN_CORE_PRIVATE_RULETABLE_CLSMOSESRULETABLE_H
