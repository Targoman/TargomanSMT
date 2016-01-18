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

#include "clsMosesPlainRuleTable.h"
#include "libTargomanCommon/Logger.h"
#include "libTargomanCommon/Configuration/Validators.hpp"
#include "libTargomanCommon/CompressedStream/clsCompressedInputStream.h"
#include "libTargomanCommon/clsCmdProgressBar.h"

#include "Private/FeatureFunctions/PhraseTable/PhraseTable.h"
#include "Private/FeatureFunctions/LanguageModel/LanguageModel.h"
#include "Private/FeatureFunctions/WordPenalty/WordPenalty.h"
#include "Private/FeatureFunctions/LexicalReordering/LexicalReordering.h"

namespace Targoman {
namespace SMT {
namespace Private {
namespace RuleTable {

enum {
    mosesFormatSourcePhrase = 0,
    mosesFormatTargetPhrase,
    mosesFormatScores
};

using namespace Common;
using namespace Common::Configuration;
using namespace Common::CompressedStream;
using namespace FeatureFunction;

TARGOMAN_REGISTER_MODULE(clsMosesPlainRuleTable);

#define FFCONFIG_KEY_IDENTIFIER "Key"

tmplConfigurable<FilePath_t> clsMosesPlainRuleTable::PhraseTableFilePath(
        MAKE_CONFIG_PATH("PhraseTableFilePath"),
        "Filepath where phrase table is stored. Relative to config file path unless specified as absolute path.",
        "",
        ConditionalPathValidator(
            gConfigs.RuleTable.toVariant().toString() == clsMosesPlainRuleTable::moduleName(),
            enuPathAccess::File | enuPathAccess::Readable)
        );

tmplConfigurable<FilePath_t> clsMosesPlainRuleTable::ReorderingTableFilePath(
        MAKE_CONFIG_PATH("ReorderingTableFilePath"),
        "Filepath where reordering table is stored. Relative to config file path unless specified as absolute path.",
        "",
        [] (const intfConfigurable& _item, QString& _errorMessage) { \
            if(gConfigs.RuleTable.toVariant().toString() == clsMosesPlainRuleTable::moduleName())
                return Validators::tmplPathAccessValidator<
                        (enuPathAccess::Type)(enuPathAccess::File | enuPathAccess::Readable), false>(
                            _item, _errorMessage);
            else
                 return true;
        }
    );

tmplConfigurable<QString> clsMosesPlainRuleTable::WordAlignmentFilePath(
        MAKE_CONFIG_PATH("WordAlignmentFilePath"),
        "Filepath where word alignment data of phrases is stored. Relative to config file path unless specified as absolute path.",
        "",
        [] (const intfConfigurable& _item, QString& _errorMessage) { \
            if(gConfigs.RuleTable.toVariant().toString() == clsMosesPlainRuleTable::moduleName())
                return Validators::tmplPathAccessValidator<
                        (enuPathAccess::Type)(enuPathAccess::File | enuPathAccess::Readable), false>(
                            _item, _errorMessage);
            else
                 return true;
        }
    );


tmplConfigurable<quint16> clsMosesPlainRuleTable::MaxRuleNodeTargetRuleCount(
        MAKE_CONFIG_PATH("MaxRuleNodeTargetRuleCount"),
        "Maximum number of target rules kept for each rule node.",
        20
        );

clsMosesPlainRuleTable::clsMosesPlainRuleTable() {
    this->PrecomputedValueIndex = clsTargetRule::allocatePrecomputedValue();
}

clsMosesPlainRuleTable::~clsMosesPlainRuleTable()
{
}

/**
 * @brief clsMosesPlainRuleTable::initializeSchema Just loads first line of phrase table to set column names and table parameters.
 */
void clsMosesPlainRuleTable::initializeSchema()
{
    TargomanLogInfo(5,
                    "Initializing Moses Plain Rule Table schema from " +
                    this->PhraseTableFilePath.value() +
                    (this->ReorderingTableFilePath.value().isEmpty() ?
                    "" :
                    " and " + this->ReorderingTableFilePath.value())
                    );

    clsCompressedInputStream PhraseTableInputStream(clsMosesPlainRuleTable::PhraseTableFilePath.value().toStdString());
    clsCompressedInputStream ReorderingTableInputStream;
    clsCompressedInputStream AlignmentInputStream;
    bool AlignmentFileExists = QFile::exists(clsMosesPlainRuleTable::WordAlignmentFilePath.value());
    if(AlignmentFileExists)
        AlignmentInputStream.open(clsMosesPlainRuleTable::WordAlignmentFilePath.value().toStdString(), true);
    bool ReorderingFileExists = QFile::exists(clsMosesPlainRuleTable::ReorderingTableFilePath.value());
    if(ReorderingFileExists )
        ReorderingTableInputStream.open(clsMosesPlainRuleTable::ReorderingTableFilePath.value().toStdString(), true);

    intfRuleTable::setReorderingAndAlignmentAvailability(
                ReorderingFileExists,
                AlignmentFileExists
                );

    if(PhraseTableInputStream.peek() < 0 || (ReorderingFileExists && ReorderingTableInputStream.peek() < 0) || (AlignmentFileExists && AlignmentInputStream.peek() < 0))
        throw exMosesPhraseTable("Phrase table, reordering table or alignment file is of size zero.");

    std::string PhraseTableLine, ReorderingTableLine, AlignmentFileLine;
    getline(PhraseTableInputStream, PhraseTableLine);
    if(ReorderingFileExists)
        getline(ReorderingTableInputStream, ReorderingTableLine);
    if(AlignmentFileExists)
        getline(AlignmentInputStream, AlignmentFileLine);
    if (PhraseTableLine == "" || (ReorderingFileExists && ReorderingTableLine == "") || (AlignmentFileExists && AlignmentFileLine == ""))
        throw exMosesPhraseTable("Empty first line in alignmetn file, phrase table or reordering table.");

    QStringList PhraseTableFields = QString::fromUtf8(PhraseTableLine.c_str()).split("|||");
    if (PhraseTableFields.size() < 3)
        throw exMosesPhraseTable(QString("Bad phrase table file format in first line : %1").arg(PhraseTableLine.c_str()));
    int ReorderingCostsFieldsSize = 0;
    this->ReorderingFeatureCount = 0;
    if(ReorderingFileExists) {
        QStringList ReorderingTableFields = QString::fromUtf8(ReorderingTableLine.c_str()).split("|||");
        if (ReorderingTableFields.size() < 3)
            throw exMosesPhraseTable(QString("Bad reordering table file format in first line : %1").arg(ReorderingTableLine.c_str()));
        if (ReorderingTableFields[mosesFormatSourcePhrase] != PhraseTableFields[mosesFormatSourcePhrase] ||
                ReorderingTableFields[mosesFormatTargetPhrase] != PhraseTableFields[mosesFormatTargetPhrase])
            throw exMosesPhraseTable(QString("Reordering and phrase tables do not match (at first line) : %1").arg(PhraseTableLine.c_str()));

        QStringList ReorderingCostsFields = ReorderingTableFields[mosesFormatScores].split(" ", QString::SkipEmptyParts);

        if (ReorderingCostsFields.size() != 3 && ReorderingCostsFields.size() != 6 )
            throw exMosesPhraseTable(QString("Invalid count of reordering scores in first line : %1").arg(ReorderingTableLine.c_str()));

        ReorderingCostsFieldsSize = ReorderingCostsFields.size();
        this->ReorderingFeatureCount = ReorderingCostsFields.size();
    }
    if(AlignmentFileExists){
        QStringList AlignmentFileFields = QString::fromUtf8(AlignmentFileLine.c_str()).split("|||");
        if (AlignmentFileFields.size() < 3)
            throw exMosesPhraseTable(QString("Bad alginment file format in first line : %1").arg(AlignmentFileLine.c_str()));
    }

    QStringList PhraseCostsFields = PhraseTableFields[mosesFormatScores].split(" ", QString::SkipEmptyParts);
    if(PhraseCostsFields.size() == 0)
        throw exMosesPhraseTable(
                QString("No phrase features defined (at first line) : %1")
                .arg(PhraseTableLine.c_str()));

    this->PhraseFeatureCount = PhraseCostsFields.size();
    QStringList ColumnNames;
    for(int i = 0; i < PhraseCostsFields.size(); ++i)
        ColumnNames.append("PhraseFeature" + QString::number(i));

    PhraseTable::setColumnNames(ColumnNames);
    if(ReorderingFileExists)
        ColumnNames.append(
                reinterpret_cast<intfFeatureFunction*>(LexicalReordering::moduleInstance())->columnNames()
                );
    if(ReorderingCostsFieldsSize == 3 && ColumnNames.size() != PhraseCostsFields.size() + 3)
        ColumnNames = ColumnNames.mid(0, ColumnNames.size() - 3);
    clsTargetRule::setColumnNames(ColumnNames);
    TargomanLogInfo(5, "Moses plain text rule set schema loaded. ");
}

/**
 * @brief clsMosesPlainRuleTable::loadTableData Loads phrase table from file and adds each phrase (rule) to pefix tree.
 */
void clsMosesPlainRuleTable::loadTableData()
{
    TargomanLogInfo(5,
                    "Loading Moses plain text rule set from " +
                    this->PhraseTableFilePath.value() +
                    " and " + this->ReorderingTableFilePath.value());


    this->PrefixTree.reset(new RulesPrefixTree_t());

    clsCmdProgressBar ProgressBar("Loading MosesRuleTable");

    QList<clsRuleNode> TotalRuleNodes;

    addUnkToUnkRule(TotalRuleNodes);

    clsCompressedInputStream PhraseTableInputStream(clsMosesPlainRuleTable::PhraseTableFilePath.value().toStdString());
    clsCompressedInputStream ReorderingTableInputStream;
    bool ReorderingFileExists = QFile::exists(clsMosesPlainRuleTable::ReorderingTableFilePath.value());
    if(ReorderingFileExists)
        ReorderingTableInputStream.open(clsMosesPlainRuleTable::ReorderingTableFilePath.value().toStdString(), true);

    size_t RulesRead = 0;

    clsCompressedInputStream AlignmentInputStream;
    bool AlignmentFileExists = QFile::exists(clsMosesPlainRuleTable::WordAlignmentFilePath.value());
    if(AlignmentFileExists)
        AlignmentInputStream.open(clsMosesPlainRuleTable::WordAlignmentFilePath.value().toStdString(), true);

    while (PhraseTableInputStream.peek() >= 0
           && (ReorderingFileExists == false || ReorderingTableInputStream.peek() >= 0)
           && (AlignmentFileExists == false || AlignmentInputStream.peek() >= 0)){
        std::string PhraseTableLine, ReorderingTableLine, AlignmentFileLine;
        getline(PhraseTableInputStream, PhraseTableLine);
        if(ReorderingFileExists)
            getline(ReorderingTableInputStream, ReorderingTableLine);
        if(AlignmentFileExists)
            getline(AlignmentInputStream, AlignmentFileLine);

        if (PhraseTableLine == "" ||
            (ReorderingFileExists && ReorderingTableLine == "") ||
            (AlignmentFileExists && AlignmentFileLine == ""))
            continue;

        ++RulesRead;
        ProgressBar.setValue(RulesRead);

        QStringList PhraseTableFields = QString::fromUtf8(PhraseTableLine.c_str()).split("|||");

        if (PhraseTableFields.size() < 3)
            throw exMosesPhraseTable(QString("Bad phrase table file format in line %1 : %2").arg(RulesRead).arg(PhraseTableLine.c_str()));

        QStringList ReorderingTableFields;
        if(ReorderingFileExists) {
            ReorderingTableFields = QString::fromUtf8(ReorderingTableLine.c_str()).split("|||");

            if (ReorderingTableFields.size() < 3)
                throw exMosesPhraseTable(QString("Bad reordering table file format in line %1 : %2").arg(RulesRead).arg(ReorderingTableLine.c_str()));

            if (ReorderingTableFields[mosesFormatSourcePhrase] != PhraseTableFields[mosesFormatSourcePhrase] ||
                    ReorderingTableFields[mosesFormatTargetPhrase] != PhraseTableFields[mosesFormatTargetPhrase])
                throw exMosesPhraseTable(QString("Reordering and phrase tables do not match (at line %1) : %2").arg(RulesRead).arg(PhraseTableLine.c_str()));
        }

        if (PhraseTableFields[mosesFormatTargetPhrase].isEmpty()){
            TargomanWarn(5,"Ignoring phrase with empty target side at line: " + QString::number(RulesRead));
            continue;
        }

        QStringList AlignmentFileFields;
        if(AlignmentFileExists)
            AlignmentFileFields = QString::fromUtf8(AlignmentFileLine.c_str()).split("|||");

        QStringList PhraseCostsFields = PhraseTableFields[mosesFormatScores].split(" ", QString::SkipEmptyParts);
        QStringList ReorderingCostsFields;
        if(ReorderingFileExists)
            ReorderingCostsFields = ReorderingTableFields[mosesFormatScores].split(" ", QString::SkipEmptyParts);
        QStringList WordAlignments;
        if(AlignmentFileExists)
            WordAlignments = AlignmentFileFields[mosesFormatScores].split(" ", QString::SkipEmptyParts);

        if(ReorderingFileExists){
            if (ReorderingCostsFields.size() != 3 && ReorderingCostsFields.size() != 6 )
                throw exMosesPhraseTable(QString("Invalid count of reordering scores in line %1 : %2").arg(RulesRead).arg(ReorderingTableLine.c_str()));
        }

        if (PhraseCostsFields.size() != this->PhraseFeatureCount)
            throw exMosesPhraseTable(QString("Inconsistent phrase scores in line %1 : %2").arg(RulesRead).arg(PhraseTableLine.c_str()));

        if(ReorderingFileExists){
            if (ReorderingCostsFields.size() != this->ReorderingFeatureCount)
                throw exMosesPhraseTable(QString("Inconsistent reordering scores in line %1 : %2").arg(RulesRead).arg(ReorderingTableLine.c_str()));
        }

        if(ReorderingFileExists){
            if(this->ReorderingFeatureCount == 6)
                PhraseCostsFields.append(ReorderingCostsFields.mid(3, 3));
            PhraseCostsFields.append(ReorderingCostsFields.mid(0, 3));
        }

        this->addRule(TotalRuleNodes, PhraseTableFields[mosesFormatSourcePhrase], PhraseTableFields[mosesFormatTargetPhrase], PhraseCostsFields, WordAlignments, RulesRead);
    }

    TargomanLogInfo(5, "Sorting rule nodes ...");

    for(int i = 0; i < TotalRuleNodes.size(); ++i) {
        QList<clsTargetRule>& TargetRuleList = TotalRuleNodes[i].targetRules();
        int NumberOfRulesToKeep = qMin(
                    (int)clsMosesPlainRuleTable::MaxRuleNodeTargetRuleCount.value(),
                    TargetRuleList.size()
                    );
        std::nth_element(
                    TargetRuleList.begin(),
                    TargetRuleList.begin() + NumberOfRulesToKeep,
                    TargetRuleList.end(),
                    [&] (const clsTargetRule& _first, const clsTargetRule& _second) {
                        return _first.precomputedValue(this->PrecomputedValueIndex) <
                                _second.precomputedValue(this->PrecomputedValueIndex);
                    }
        );

        // Prune the unnecessary rules
        if(NumberOfRulesToKeep < TargetRuleList.size())
            TargetRuleList.erase(
                    TargetRuleList.begin() + NumberOfRulesToKeep,
                    TargetRuleList.end()
                    );

        qStableSort(TargetRuleList.begin(), TargetRuleList.end(),
                    [&] (const clsTargetRule& _first, const clsTargetRule& _second) {
                        return _first.precomputedValue(this->PrecomputedValueIndex) <
                        _second.precomputedValue(this->PrecomputedValueIndex);
                    }
        );
    }
    TargomanLogInfo(5, "Moses plain text rule set loaded. ");
}

/**
 * @brief getPrematureTargetRuleCost    helper function for clsMosesPlainRuleTable::addRule() that computes a score for target rules forgetting about where they are to be placed
 * @param _targetRule                   input target rule for which the cost is computed
 * @return                              the computed cost
 */
/*inline */Cost_t getPrematureTargetRuleCost(const clsTargetRule& _targetRule)
{
    PhraseTable& PhraseCostFeature = *static_cast<PhraseTable*>(PhraseTable::moduleInstance());
    LanguageModel& LanguageModelFeature = *static_cast<LanguageModel*>(LanguageModel::moduleInstance());
    WordPenalty& WordPenaltyFeature = *static_cast<WordPenalty*>(WordPenalty::moduleInstance());
    Cost_t PhraseCost = PhraseCostFeature.getPhraseCost(_targetRule);
    Cost_t LanguageModelCost = LanguageModelFeature.getLanguageModelCost(_targetRule);
    Cost_t WordPenaltyCost = WordPenaltyFeature.getWordPenaltyCost(_targetRule);
    return  PhraseCost + LanguageModelCost + WordPenaltyCost;
}

/**
 * @brief clsMosesPlainRuleTable::addRule   adds a new rule to the rule prefix tree
 * @param _sourcePhrase                     source phrase, pointing to the specific prefix tree node
 * @param _targetPhrase                     target phrase for which a target rule will be created
 * @param _costs                            list of cost fields read from the files
 * @note                                    the prefix tree node will be created if it does not exist already
 */
void clsMosesPlainRuleTable::addRule(QList<clsRuleNode>& _ruleNodeList,
                                     const QList<WordIndex_t> _sourcePhrase,
                                     const QList<WordIndex_t> _targetPhrase,
                                     const QList<Cost_t> _costs,
                                     const QMap<int, int> _alignment)
{
    RuleTable::clsTargetRule TargetRule(_targetPhrase, _costs, _alignment);

    clsRuleNode& RuleNode = this->PrefixTree->getOrCreateNode(_sourcePhrase)->getData();
    if (RuleNode.isInvalid()) {
        RuleNode.detachInvalidData();
        _ruleNodeList.append(RuleNode);
    }
    TargetRule.setPrecomputedValue(
                this->PrecomputedValueIndex,
                getPrematureTargetRuleCost(TargetRule)
                );
    RuleNode.targetRules().append(TargetRule);
}

/**
 * @brief clsMosesPlainRuleTable::addRule   creates source phrase and target phrase representations from their string counterparts and adds a rule to prefix tree
 * @param _sourcePhrase                     string representation of the source phrase
 * @param _targetPhrase                     string representation of the target phrase
 * @param _costs                            the list containing string representations of the cost field values
 * @param _ruleNumber                       index of the line read from the input file
 */
void clsMosesPlainRuleTable::addRule(QList<clsRuleNode>& _ruleNodeList,
                                     const QString& _sourcePhrase,
                                     const QString& _targetPhrase,
                                     const QStringList &_costs,
                                     const QStringList &_wordAlignments,
                                     size_t _ruleNumber)
{
    Q_UNUSED(_ruleNumber)

    QList<Cost_t>       Costs;
    foreach(const QString& Cost, _costs)
        Costs.append(-log(Cost.toDouble()));

    QList<WordIndex_t> SourcePhrase;
    foreach(const QString& Word, _sourcePhrase.split(" ", QString::SkipEmptyParts)){
        WordIndex_t WordIndex = gConfigs.SourceVocab.value(Word, Constants::SrcVocabUnkWordIndex);
        if (WordIndex == Constants::SrcVocabUnkWordIndex && Word != "<unk>"){
            WordIndex = gConfigs.SourceVocab.size() + 1;
            gConfigs.SourceVocab.insert(Word, WordIndex);
        }
        SourcePhrase.append(WordIndex);
    }

    QList<WordIndex_t> TargetPhrase;
    foreach(const QString& Word, _targetPhrase.split(" ", QString::SkipEmptyParts))
        TargetPhrase.append(gConfigs.EmptyLMScorer->getWordIndex(Word));

    QMap<int, int> Alignments;
    for(QString wordAlignment : _wordAlignments) {
        QStringList AlignmentParts = wordAlignment.split("-");
        Alignments.insertMulti(AlignmentParts[1].toInt(), AlignmentParts[0].toInt());
    }
    this->addRule(_ruleNodeList, SourcePhrase, TargetPhrase, Costs, Alignments);
}

/**
 * @brief clsMosesPlainRuleTable::addUnkToUnkRule   adds the unknown to unkown word translation rule to avoid stucking at unknown words
 */
void clsMosesPlainRuleTable::addUnkToUnkRule(QList<clsRuleNode>& _ruleNodeList)
{
    QList<Cost_t> Costs;
    for(int i = 0; i < this->PhraseFeatureCount + this->ReorderingFeatureCount; ++i)
        Costs.append(0.0);
    QList<WordIndex_t> SrcUnk;
    SrcUnk.append(gConfigs.EmptyLMScorer->unknownWordIndex());
    QList<WordIndex_t> TgtUnk;
    TgtUnk.append(gConfigs.EmptyLMScorer->unknownWordIndex());
    QMap<int, int> Alignment;
    Alignment.insert(0, 0);
    addRule(_ruleNodeList, SrcUnk, TgtUnk, Costs, Alignment);
}

}
}
}
}
