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

#ifndef TARGOMAN_CORE_PRIVATE_RULETABLE_INTFRULETABLE_H
#define TARGOMAN_CORE_PRIVATE_RULETABLE_INTFRULETABLE_H

#include "libTargomanCommon/Macros.h"
#include "Private/RuleTable/clsRuleNode.h"
#include "libTargomanCommon/FStreamExtended.h"
#include "Private/FeatureFunctions/PhraseTable/PhraseTable.h"
#include "libTargomanCommon/PrefixTree/tmplPrefixTree.h"

namespace Targoman {
namespace SMT {
namespace Private {
namespace RuleTable {

typedef Common::PrefixTree::tmplPrefixTree<
            Common::WordIndex_t, RuleTable::clsRuleNode> RulesPrefixTree_t;

static const QString TARGOMAN_BINARY_RULETABLE_HEADER = "TargomanBinaryRuleTable-v0.1";

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
    intfRuleTable() { }

    TARGOMAN_DEFINE_MODULE_SCOPE(intfRuleTable)

    virtual ~intfRuleTable(){}

    virtual void initializeSchema() = 0;
    virtual void loadTableData() = 0;
    virtual void binarizeTableData(const QString& _filePath) = 0;
    void saveBinaryRuleTable(const QString& _filePath){
        try{
            Common::clsOFStreamExtended OutStream(_filePath);
            //Write Bnary file header
            OutStream.write(TARGOMAN_BINARY_RULETABLE_HEADER.toLatin1().constData(),
                            TARGOMAN_BINARY_RULETABLE_HEADER.toLatin1().size());
            //write Vocab
            OutStream.write(gConfigs.SourceVocab.size());
            for(auto VocabIter = gConfigs.SourceVocab.begin();
                VocabIter != gConfigs.SourceVocab.end();
                ++VocabIter){
                OutStream.write(VocabIter.key());
                OutStream.write(VocabIter.value());
            }

            //Write TargetRuleColumnNames
            OutStream.write(clsTargetRule::columnNames().size());
            foreach (const QString& ColumnName, clsTargetRule::columnNames())
                OutStream.write(ColumnName);

            //Write Phrase table specific column names
            const QStringList PhraseTableColumnNames =
                    static_cast<FeatureFunction::intfFeatureFunction*>
                    (FeatureFunction::PhraseTable::moduleInstance())->columnNames();
            OutStream.write(PhraseTableColumnNames.size());
            foreach (const QString& ColumnName,PhraseTableColumnNames)
                OutStream.write(ColumnName);

            //Call prefix tree to store nodes
            PrefixTree->writeBinary(OutStream);
        }catch(std::exception &e){
            throw exRuleTable(QString::fromUtf8(e.what()));
        }
    }

    void saveBinarySchema(Common::clsOFStreamExtended& OutStream){
        try{

            //Write Bnary file header
            OutStream.write(TARGOMAN_BINARY_RULETABLE_HEADER.toLatin1().constData(),
                            TARGOMAN_BINARY_RULETABLE_HEADER.toLatin1().size());
            //write Vocab
            OutStream.write(gConfigs.SourceVocab.size());
            for(auto VocabIter = gConfigs.SourceVocab.begin();
                VocabIter != gConfigs.SourceVocab.end();
                ++VocabIter){
                OutStream.write(VocabIter.key());
                OutStream.write(VocabIter.value());
            }

            //Write TargetRuleColumnNames
            OutStream.write(clsTargetRule::columnNames().size());
            foreach (const QString& ColumnName, clsTargetRule::columnNames())
                OutStream.write(ColumnName);

            //Write Phrase table specific column names
            const QStringList PhraseTableColumnNames =
                    static_cast<FeatureFunction::intfFeatureFunction*>
                    (FeatureFunction::PhraseTable::moduleInstance())->columnNames();
            OutStream.write(PhraseTableColumnNames.size());
            foreach (const QString& ColumnName,PhraseTableColumnNames)
                OutStream.write(ColumnName);

        }catch(std::exception &e){
            throw exRuleTable(QString::fromUtf8(e.what()));
        }
    }

    virtual RuleTable::RulesPrefixTree_t& prefixTree(){
        return *this->PrefixTree;
    }

    int getPreComputedValueIndex(){
        return PrecomputedValueIndex;
    }

protected:
    void setReorderingAndAlignmentAvailability(bool _lexicalReorderingAvailable,
                                               bool _alignmentAvailable)
    {
        clsTargetRule::LexicalReorderingAvailable = _lexicalReorderingAvailable;
        clsTargetRule::AlignmentDataAvailable = _alignmentAvailable;
    }

protected:
    QScopedPointer<RulesPrefixTree_t> PrefixTree;               /**< The container which stores the source and target phrases. */
    int PrecomputedValueIndex = -1;
};


}
}
}
}

#endif // TARGOMAN_CORE_PRIVATE_RULETABLE_INTFRULETABLE_H
