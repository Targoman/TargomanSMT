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
        clsBinaryRuleTable::baseConfigPath() + "/FilePath",
        "FilePath where rule table is stored",
        "",
        ConditionalPathValidator(
            gConfigs.RuleTable.toVariant().toString() == clsBinaryRuleTable::moduleName(),
            enuPathAccess::File | enuPathAccess::Readable)
        );

tmplConfigurable<enuBinaryLoadMode::Type> clsBinaryRuleTable::LoadMode(
        clsBinaryRuleTable::baseConfigPath() + "/LoadMode",
        "Binary file load mode. Can be [" + enuBinaryLoadMode::options().join("|")+"]",
        enuBinaryLoadMode::OnDemand,
        ReturnTrueCrossValidator);

tmplConfigurable<quint32> clsBinaryRuleTable::MaxCachedItems(
        clsBinaryRuleTable::baseConfigPath() + "/MaxCachedItems",
        "Maximum items to be cached",
        100000,
        ReturnTrueCrossValidator);

TARGOMAN_REGISTER_MODULE(clsBinaryRuleTable);

clsBinaryRuleTable::clsBinaryRuleTable(quint64 _instanceID) :
    intfRuleTable(this->moduleName(),_instanceID)
{
}

clsBinaryRuleTable::~clsBinaryRuleTable()
{
    this->unregister();
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
