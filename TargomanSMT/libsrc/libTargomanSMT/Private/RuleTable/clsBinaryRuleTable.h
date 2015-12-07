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

#ifndef TARGOMAN_CORE_PRIVATE_RULETABLE_CLSBINARYRULETABLE_H
#define TARGOMAN_CORE_PRIVATE_RULETABLE_CLSBINARYRULETABLE_H

#include "libTargomanCommon/Configuration/tmplConfigurable.h"
#include "libTargomanCommon/FStreamExtended.h"
#include "intfRuleTable.hpp"
#include "clsRuleNode.h"

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
    explicit clsBinaryRuleTable(quint64 _instanceID);
    ~clsBinaryRuleTable();

    void initializeSchema();
    void loadTableData();

private:
    QScopedPointer<Common::clsIFStreamExtended> InputStream;

private:
    static Targoman::Common::Configuration::tmplConfigurable<FilePath_t>   FilePath;            /**< File name of phrase table. */
    static Common::Configuration::tmplConfigurable<Common::PrefixTree::enuBinaryLoadMode::Type>  LoadMode;
    static Common::Configuration::tmplConfigurable<quint32>  MaxCachedItems;

    TARGOMAN_DEFINE_MODULE(BinaryRuleTable);
};



}
}
}
}

ENUM_CONFIGURABLE(Targoman::Common::PrefixTree::enuBinaryLoadMode);

#endif // TARGOMAN_CORE_PRIVATE_RULETABLE_CLSBINARYRULETABLE_H
