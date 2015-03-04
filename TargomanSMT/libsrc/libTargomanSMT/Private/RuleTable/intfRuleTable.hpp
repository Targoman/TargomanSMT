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

#ifndef TARGOMAN_CORE_PRIVATE_RULETABLE_INTFRULETABLE_H
#define TARGOMAN_CORE_PRIVATE_RULETABLE_INTFRULETABLE_H

#include "libTargomanCommon/Macros.h"
#include "Private/InputDecomposer/clsInput.h"
#include "Private/RuleTable/clsRuleNode.h"
#include "FStreamExtended.h"
#include "Private/FeatureFunctions/PhraseTable/PhraseTable.h"

namespace Targoman {
namespace SMT {
namespace Private {
namespace RuleTable{

typedef Common::PrefixTree::tmplFullVectorFilePrefixTree<RuleTable::clsRuleNode> RulesPrefixTree_t;
static const char* TARGOMAN_BINARY_RULETABLE_HEADER = "TargomanBinaryRuleTable-v0.1";

/**
 * @brief The intfRuleTable class is used to store source and target phrases in #prefixTree.
 * This class is an interface so clsJaneRuleTable or clsMosesRuleTable can be derived from this class.
 *
 * @note: For each phrases in the source language, there is a node in the #PrefixTree.
 * Type of each node is clsRuleNode and it stores the list of translations (phrases in target language) for source phrase.
 *
 * @note: The string (vector of wordIndex) of source phrase is not stored anywhere.
 * PrefixTree helps us to store translations for each source phrase in correct place.
 *
 */
class intfRuleTable : public Common::Configuration::intfModule
{
public:
    intfRuleTable(const QString& _moduleName, quint64 _instanceID) :
    intfModule(_moduleName, _instanceID)
    {}

    virtual ~intfRuleTable(){}

    virtual void initializeSchema() = 0;
    virtual void loadTableData() = 0;
    void saveBinaryRuleTable(const QString& _filePath){
        try{
            clsOFStreamExtended OutStream(_filePath);
            OutStream.write<const char*>(TARGOMAN_BINARY_RULETABLE_HEADER);
            OutStream.write(clsTargetRule::columnNames().size());
            foreach (const QString& ColumnName, clsTargetRule::columnNames())
                OutStream.write(ColumnName);

            const QStringList PhraseTableColumnNames =
                    static_cast<FeatureFunction::intfFeatureFunction*>
                    (FeatureFunction::PhraseTable::moduleInstance())->columnNames();

            OutStream.write(PhraseTableColumnNames.size());
            foreach (const QString& ColumnName,PhraseTableColumnNames)
                OutStream.write(ColumnName);


            PrefixTree->writeBinary(OutStream);
        }catch(std::exception &e){
            throw exRuleTable(QString::fromUtf8(e.what()));
        }
    }

    virtual RuleTable::RulesPrefixTree_t& getPrefixTree(){
        return *this->PrefixTree;
    }

protected:
    QScopedPointer<RulesPrefixTree_t> PrefixTree;               /**< The container which stores the source and target phrases. */
};


}
}
}
}

#endif // TARGOMAN_CORE_PRIVATE_RULETABLE_INTFRULETABLE_H
