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
        "Filename where rule table is stored",
        "",
        Validators::tmplPathAccessValidator<(enuPathAccess::Type)(enuPathAccess::File | enuPathAccess::Readable)>
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
    try{
        this->InputStream.reset(new clsIFStreamExtended(clsBinaryRuleTable::FilePath.value()));
        QByteArray BinFileHeader(sizeof(TARGOMAN_BINARY_RULETABLE_HEADER), Qt::Uninitialized);
        this->InputStream->read(BinFileHeader.data(), BinFileHeader.size());

        if (BinFileHeader != TARGOMAN_BINARY_RULETABLE_HEADER)
            throw exRuleTable("Invalid Binary file");
        int ColumnCount = this->InputStream->read<int>();
        QStringList Columns;
        for(int i=0; i< ColumnCount; ++i)
            Columns.append(this->InputStream->read<QString>());
        clsTargetRule::setColumnNames(Columns);

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
    this->PrefixTree->readBinary((std::istream&)this->InputStream);
    delete this->InputStream.take();
}


}
}
}
}
