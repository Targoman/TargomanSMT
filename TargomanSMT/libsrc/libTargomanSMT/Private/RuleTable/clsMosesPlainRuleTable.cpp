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
        clsMosesPlainRuleTable::baseConfigPath() + "/PhraseTableFilePath",
        "Filepath where phrase table is stored",
        "",
        ConditionalPathValidator(
            gConfigs.RuleTable.toVariant().toString() == clsMosesPlainRuleTable::moduleName(),
            enuPathAccess::File | enuPathAccess::Readable)
        );

tmplConfigurable<FilePath_t> clsMosesPlainRuleTable::ReorderingTableFilePath(
        clsMosesPlainRuleTable::baseConfigPath() + "/ReorderingTableFilePath",
        "Filepath where reordering table is stored",
        "",
        ConditionalPathValidator(
            gConfigs.RuleTable.toVariant().toString() == clsMosesPlainRuleTable::moduleName(),
            enuPathAccess::File | enuPathAccess::Readable)
        );

tmplConfigurable<int> clsMosesPlainRuleTable::MaxRuleNodeTargetRuleCount(
        clsMosesPlainRuleTable::baseConfigPath() + "/MaxRuleNodeTargetRuleCount",
        "Maximum number of target rules kept for each rule node.",
        20,
        Validators::tmplNumericValidator<int, 0, 65536>
        );

clsMosesPlainRuleTable::clsMosesPlainRuleTable(quint64 _instanceID)  :
    intfRuleTable(this->moduleName(), _instanceID)
{
    this->PrecomputedValueIndex = clsTargetRule::allocatePrecomputedValue();
}

clsMosesPlainRuleTable::~clsMosesPlainRuleTable()
{
    this->unregister();
}

/**
 * @brief clsMosesPlainRuleTable::initializeSchema Just loads first line of phrase table to set column names and table parameters.
 */
void clsMosesPlainRuleTable::initializeSchema()
{
    TargomanLogInfo(5,
                    "Initializing Moses Plain Rule Table schema from " +
                    this->PhraseTableFilePath.value() +
                    " and " + this->ReorderingTableFilePath.value());

    clsCompressedInputStream PhraseTableInputStream(clsMosesPlainRuleTable::PhraseTableFilePath.value().toStdString());
    clsCompressedInputStream ReorderingTableInputStream(clsMosesPlainRuleTable::ReorderingTableFilePath.value().toStdString());


    if(PhraseTableInputStream.peek() < 0 && ReorderingTableInputStream.peek() < 0)
        throw exMosesPhraseTable("Either phrase table or reordering table file is of size zero.");

    std::string PhraseTableLine, ReorderingTableLine;
    getline(PhraseTableInputStream, PhraseTableLine);
    getline(ReorderingTableInputStream, ReorderingTableLine);
    if (PhraseTableLine == "" || ReorderingTableLine == "")
        throw exMosesPhraseTable("Empty first line in either phrase or reordering table.");

    QStringList PhraseTableFields = QString::fromUtf8(PhraseTableLine.c_str()).split("|||");
    if (PhraseTableFields.size() < 3)
        throw exMosesPhraseTable(QString("Bad phrase table file format in first line : %1").arg(PhraseTableLine.c_str()));
    QStringList ReorderingTableFields = QString::fromUtf8(ReorderingTableLine.c_str()).split("|||");
    if (ReorderingTableFields.size() < 3)
        throw exMosesPhraseTable(QString("Bad reordering table file format in first line : %1").arg(ReorderingTableLine.c_str()));
    if (ReorderingTableFields[mosesFormatSourcePhrase] != PhraseTableFields[mosesFormatSourcePhrase] ||
            ReorderingTableFields[mosesFormatTargetPhrase] != PhraseTableFields[mosesFormatTargetPhrase])
        throw exMosesPhraseTable(QString("Reordering and phrase tables do not match (at first line) : %1").arg(PhraseTableLine.c_str()));

    QStringList PhraseCostsFields = PhraseTableFields[mosesFormatScores].split(" ", QString::SkipEmptyParts);
    QStringList ReorderingCostsFields = ReorderingTableFields[mosesFormatScores].split(" ", QString::SkipEmptyParts);

    if (ReorderingCostsFields.size() != 3 && ReorderingCostsFields.size() != 6 )
        throw exMosesPhraseTable(QString("Invalid count of reordering scores in first line : %1").arg(ReorderingTableLine.c_str()));

    if(PhraseCostsFields.size() == 0)
        throw exMosesPhraseTable(
                QString("No phrase features defined (at first line) : %1")
                .arg(PhraseTableLine.c_str()));

    this->PhraseFeatureCount = PhraseCostsFields.size();
    this->ReorderingFeatureCount = ReorderingCostsFields.size();
    QStringList ColumnNames;
    for(int i = 0; i < PhraseCostsFields.size(); ++i)
        ColumnNames.append("PhraseFeature" + QString::number(i));

    PhraseTable::setColumnNames(ColumnNames);
    ColumnNames.append(
                reinterpret_cast<intfFeatureFunction*>(LexicalReordering::moduleInstance())->columnNames()
                );
    if(ReorderingCostsFields.size() == 3 && ColumnNames.size() != PhraseCostsFields.size() + 3)
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
    clsCompressedInputStream ReorderingTableInputStream(clsMosesPlainRuleTable::ReorderingTableFilePath.value().toStdString());
    size_t RulesRead = 0;

    while (PhraseTableInputStream.peek() >= 0 && ReorderingTableInputStream.peek() >= 0){
        std::string PhraseTableLine, ReorderingTableLine;
        getline(PhraseTableInputStream, PhraseTableLine);
        getline(ReorderingTableInputStream, ReorderingTableLine);
        if (PhraseTableLine == "" || ReorderingTableLine == "")
            continue;
        ++RulesRead;
        ProgressBar.setValue(RulesRead);

        QStringList PhraseTableFields = QString::fromUtf8(PhraseTableLine.c_str()).split("|||");

        if (PhraseTableFields.size() < 3)
            throw exMosesPhraseTable(QString("Bad phrase table file format in line %1 : %2").arg(RulesRead).arg(PhraseTableLine.c_str()));

        QStringList ReorderingTableFields = QString::fromUtf8(ReorderingTableLine.c_str()).split("|||");

        if (ReorderingTableFields.size() < 3)
            throw exMosesPhraseTable(QString("Bad reordering table file format in line %1 : %2").arg(RulesRead).arg(ReorderingTableLine.c_str()));

        if (ReorderingTableFields[mosesFormatSourcePhrase] != PhraseTableFields[mosesFormatSourcePhrase] ||
                ReorderingTableFields[mosesFormatTargetPhrase] != PhraseTableFields[mosesFormatTargetPhrase])
            throw exMosesPhraseTable(QString("Reordering and phrase tables do not match (at line %1) : %2").arg(RulesRead).arg(PhraseTableLine.c_str()));

        if (PhraseTableFields[mosesFormatTargetPhrase].isEmpty()){
            TargomanWarn(5,"Ignoring phrase with empty target side at line: " + QString::number(RulesRead));
            continue;
        }

        QStringList PhraseCostsFields = PhraseTableFields[mosesFormatScores].split(" ", QString::SkipEmptyParts);
        QStringList ReorderingCostsFields = ReorderingTableFields[mosesFormatScores].split(" ", QString::SkipEmptyParts);

        if (ReorderingCostsFields.size() != 3 && ReorderingCostsFields.size() != 6 )
            throw exMosesPhraseTable(QString("Invalid count of reordering scores in line %1 : %2").arg(RulesRead).arg(ReorderingTableLine.c_str()));

        if (PhraseCostsFields.size() != this->PhraseFeatureCount)
            throw exMosesPhraseTable(QString("Inconsistent phrase scores in line %1 : %2").arg(RulesRead).arg(PhraseTableLine.c_str()));

        if (ReorderingCostsFields.size() != this->ReorderingFeatureCount)
            throw exMosesPhraseTable(QString("Inconsistent reordering scores in line %1 : %2").arg(RulesRead).arg(ReorderingTableLine.c_str()));

        if(this->ReorderingFeatureCount == 6)
            PhraseCostsFields.append(ReorderingCostsFields.mid(3, 3));
        PhraseCostsFields.append(ReorderingCostsFields.mid(0, 3));

        this->addRule(TotalRuleNodes, PhraseTableFields[mosesFormatSourcePhrase], PhraseTableFields[mosesFormatTargetPhrase], PhraseCostsFields, RulesRead);
    }

    TargomanLogInfo(5, "Sorting rule nodes ...");

    for(int i = 0; i < TotalRuleNodes.size(); ++i) {
        QList<clsTargetRule>& TargetRuleList = TotalRuleNodes[i].targetRules();
        int NumberOfRulesToKeep = qMin(
                    clsMosesPlainRuleTable::MaxRuleNodeTargetRuleCount.value(),
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
    }
    TargomanLogInfo(5, "Moses plain text rule set loaded. ");
}

/**
 * @brief getPrematureTargetRuleCost    helper function for clsMosesPlainRuleTable::addRule() that computes a score for target rules forgetting about where they are to be placed
 * @param _targetRule                   input target rule for which the cost is computed
 * @return                              the computed cost
 */
inline Cost_t getPrematureTargetRuleCost(const clsTargetRule& _targetRule)
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
                                     const QList<Cost_t> _costs)
{
    RuleTable::clsTargetRule TargetRule(_targetPhrase, _costs);

    clsRuleNode& RuleNode = this->PrefixTree->getOrCreateNode(_sourcePhrase).getData();
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

    this->addRule(_ruleNodeList, SourcePhrase, TargetPhrase, Costs);
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
    addRule(_ruleNodeList, SrcUnk, TgtUnk, Costs);
}

}
}
}
}
