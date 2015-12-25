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

#include "clsBinaryRuleTable.h"
#include "libTargomanCommon/Logger.h"
#include "libTargomanCommon/Configuration/Validators.hpp"
#include "libTargomanCommon/clsCmdProgressBar.h"

namespace Targoman {
namespace SMT {
namespace Private {
namespace RuleTable {


using namespace Common;
using namespace Common::Configuration;
using namespace Common::PrefixTree;
using namespace FeatureFunction;

tmplConfigurable<FilePath_t> clsBinaryRuleTable::FilePath(
        MAKE_CONFIG_PATH("FilePath"),
        "FilePath where rule table is stored",
        "",
        ConditionalPathValidator(
            gConfigs.RuleTable.toVariant().toString() == clsBinaryRuleTable::moduleName(),
            enuPathAccess::File | enuPathAccess::Readable)
        );

tmplConfigurable<enuBinaryLoadMode::Type> clsBinaryRuleTable::LoadMode(
        MAKE_CONFIG_PATH("LoadMode"),
        "Binary file load mode. Can be [" + enuBinaryLoadMode::options().join("|")+"]",
        enuBinaryLoadMode::OnDemand,
        ReturnTrueCrossValidator);

tmplConfigurable<quint32> clsBinaryRuleTable::MaxCachedItems(
        MAKE_CONFIG_PATH("MaxCachedItems"),
        "Maximum items to be cached",
        100000,
        ReturnTrueCrossValidator);

TARGOMAN_REGISTER_MODULE(clsBinaryRuleTable);

clsBinaryRuleTable::clsBinaryRuleTable() :
    intfRuleTable()
{
}

clsBinaryRuleTable::~clsBinaryRuleTable()
{
}

void clsBinaryRuleTable::initializeSchema()
{
    this->InputStream.reset(new clsIFStreamExtended(
                                clsBinaryRuleTable::FilePath.value(),
                                clsBinaryRuleTable::LoadMode.value() == enuBinaryLoadMode::OnMemory));
    if (this->InputStream->is_open() == false)
        throw exRuleTable("Unable to open " + clsBinaryRuleTable::FilePath.value());
    try{
        QByteArray BinFileHeader(TARGOMAN_BINARY_RULETABLE_HEADER.size(), Qt::Uninitialized);
        this->InputStream->read(BinFileHeader.data(), BinFileHeader.size());

        if (BinFileHeader != TARGOMAN_BINARY_RULETABLE_HEADER)
            throw exRuleTable("Invalid Binary file");

        TargomanLogInfo(5, "Loading binary rule table schema from " + clsBinaryRuleTable::FilePath.value() + "...");

        //Load Vocab
        int VocabCount = this->InputStream->read<int>();
        gConfigs.SourceVocab.reserve(VocabCount);
        for(int i=0; i< VocabCount; ++i)
            gConfigs.SourceVocab[this->InputStream->read<QString>()] = this->InputStream->read<WordIndex_t>();

        //Load TargetRule column names
        int ColumnCount = this->InputStream->read<int>();
        QStringList Columns;
        for(int i=0; i< ColumnCount; ++i)
            Columns.append(this->InputStream->read<QString>());
        clsTargetRule::setColumnNames(Columns);

        // TODO: Implement a more decent approach!
        if(Columns.contains("BackwardMonotone"))
            intfRuleTable::setReorderingAndAlignmentAvailability(true, false);

        //Load PhraseTable column names
        ColumnCount = this->InputStream->read<int>();
        Columns.clear();
        for(int i=0; i< ColumnCount; ++i)
            Columns.append(this->InputStream->read<QString>());

        FeatureFunction::PhraseTable::setColumnNames(Columns);

    }catch(std::exception &e){
        throw exRuleTable(QString::fromUtf8(e.what()));
    }
    TargomanLogInfo(5, "Binary rule table set schema loaded. ");
}

void clsBinaryRuleTable::loadTableData()
{
    TargomanLogInfo(5, "Loading binary rule table from: " + this->FilePath.value());

    this->PrefixTree.reset(new RulesPrefixTree_t());
    this->PrefixTree->readBinary(*this->InputStream,
                                 clsBinaryRuleTable::LoadMode.value(),
                                 clsBinaryRuleTable::MaxCachedItems.value());
    TargomanLogInfo(5, "Binary rule table loaded. ");
}


}
}
}
}

ENUM_CONFIGURABLE_IMPL(Targoman::Common::PrefixTree::enuBinaryLoadMode)
