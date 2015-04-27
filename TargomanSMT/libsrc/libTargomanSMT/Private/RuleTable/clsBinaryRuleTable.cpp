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
using namespace FeatureFunction;

tmplConfigurable<QString> clsBinaryRuleTable::FilePath(
        clsBinaryRuleTable::baseConfigPath() + "/FilePath",
        "FilePath where rule table is stored",
        "",
        ConditionalPathValidator(
            gConfigs.RuleTable.toVariant().toString() == clsBinaryRuleTable::moduleName(),
            enuPathAccess::File | enuPathAccess::Readable)
        );

tmplConfigurable<bool> clsBinaryRuleTable::LoadOnDemand(
        clsBinaryRuleTable::baseConfigPath() + "/LoadOnDemand",
        "TODO Desc",
        true
        );

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
    this->InputStream.reset(new clsIFStreamExtended(clsBinaryRuleTable::FilePath.value(),
                                                    clsBinaryRuleTable::LoadOnDemand.value() == false));
    if (this->InputStream->is_open() == false)
        throw exRuleTable("Unable to open " + clsBinaryRuleTable::FilePath.value());
    try{
        QByteArray BinFileHeader(TARGOMAN_BINARY_RULETABLE_HEADER.size(), Qt::Uninitialized);
        this->InputStream->read(BinFileHeader.data(), BinFileHeader.size());

        if (BinFileHeader != TARGOMAN_BINARY_RULETABLE_HEADER)
            throw exRuleTable("Invalid Binary file");

        TargomanLogInfo(5, "Loading binary rule table from " + clsBinaryRuleTable::FilePath.value() + "...");

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
}

void clsBinaryRuleTable::loadTableData()
{
    this->PrefixTree.reset(new RulesPrefixTree_t());
    this->PrefixTree->readBinary(*this->InputStream, clsBinaryRuleTable::LoadOnDemand.value() == false);
}


}
}
}
}
