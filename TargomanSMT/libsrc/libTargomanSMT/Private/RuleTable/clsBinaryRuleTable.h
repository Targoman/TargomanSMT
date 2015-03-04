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

#ifndef TARGOMAN_CORE_PRIVATE_RULETABLE_CLSBINARYRULETABLE_H
#define TARGOMAN_CORE_PRIVATE_RULETABLE_CLSBINARYRULETABLE_H

#include "libTargomanCommon/Configuration/tmplConfigurable.h"
#include "libTargomanCommon/PrefixTree/tmplFullVectorFilePrefixTree.hpp"
#include "intfRuleTable.hpp"
#include "clsRuleNode.h"
#include "FStreamExtended.h"

namespace Targoman {
namespace SMT {
namespace Private {
namespace RuleTable {

TARGOMAN_ADD_EXCEPTION_HANDLER(exMosesPhraseTable, exRuleTable);

/**
 * @brief The clsBinaryRuleTable class is used to load moses phrase and reordering table.
 */
class clsBinaryRuleTable : public intfRuleTable
{
public:
    clsBinaryRuleTable(quint64 _instanceID);
    ~clsBinaryRuleTable();

    void initializeSchema();
    void loadTableData();

private:
    QScopedPointer<clsIFStreamExtended> InputStream;

private:
    static Targoman::Common::Configuration::tmplConfigurable<QString>   FilePath;            /**< File name of phrase table. */


    TARGOMAN_DEFINE_MODULE("BinaryRuleTable", clsBinaryRuleTable)
};

}
}
}
}
#endif // TARGOMAN_CORE_PRIVATE_RULETABLE_CLSBINARYRULETABLE_H
