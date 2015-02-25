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
#include "Private/FeatureFunctions/LexicalReordering/LexicalReordering.h"

namespace Targoman {
namespace Core {
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

TARGOMAN_REGISTER_MODULE(clsMosesPlainRuleTable)

#define FFCONFIG_KEY_IDENTIFIER "Key"

tmplConfigurable<QString> clsMosesPlainRuleTable::PhraseTableFileName(
        clsMosesPlainRuleTable::baseConfigPath() + "/PhraseTableFileName",
        "Filename where phrase table is stored",
        "",
        Validators::tmplPathAccessValidator<(enuPathAccess::Type)(enuPathAccess::File | enuPathAccess::Readable)>
        );

tmplConfigurable<QString> clsMosesPlainRuleTable::ReorderingTableFileName(
        clsMosesPlainRuleTable::baseConfigPath() + "/ReorderingTableFileName",
        "Filename where reordering table is stored",
        "",
        Validators::tmplPathAccessValidator<(enuPathAccess::Type)(enuPathAccess::File | enuPathAccess::Readable)>
        );

clsMosesPlainRuleTable::clsMosesPlainRuleTable(quint64 _instanceID)  :
    intfRuleTable(this->moduleName(), _instanceID)
{}

clsMosesPlainRuleTable::~clsMosesPlainRuleTable()
{
    this->unregister();
}

void clsMosesPlainRuleTable::initializeSchema()
{
    TargomanLogInfo(5,
                    "Initializing clsMosesPlainRuleTable schema from " +
                    this->PhraseTableFileName.value() +
                    " and " + this->ReorderingTableFileName.value());

    clsCompressedInputStream PhraseTableInputStream(clsMosesPlainRuleTable::PhraseTableFileName.value().toStdString());
    clsCompressedInputStream ReorderingTableInputStream(clsMosesPlainRuleTable::ReorderingTableFileName.value().toStdString());


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
    if (ReorderingTableFields[mosesFormatSourcePhrase] != ReorderingTableFields[mosesFormatSourcePhrase] ||
            ReorderingTableFields[mosesFormatTargetPhrase] != ReorderingTableFields[mosesFormatTargetPhrase])
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
                reinterpret_cast<intfFeatureFunction*>(LexicalReordering::instance())->columnNames()
                );
    if(ReorderingCostsFields.size() == 3 && ColumnNames.size() != PhraseCostsFields.size() + 3)
        ColumnNames = ColumnNames.mid(0, ColumnNames.size() - 3);
    clsTargetRule::setColumnNames(ColumnNames);
}

void clsMosesPlainRuleTable::loadTableData()
{
    TargomanLogInfo(5,
                    "Loading Moses plain text rule set from " +
                    this->PhraseTableFileName.value() +
                    " and " + this->ReorderingTableFileName.value());


    this->PrefixTree.reset(new RulesPrefixTree_t());

    clsCmdProgressBar ProgressBar("Loading MosesRuleTable");

    clsCompressedInputStream PhraseTableInputStream(clsMosesPlainRuleTable::PhraseTableFileName.value().toStdString());
    clsCompressedInputStream ReorderingTableInputStream(clsMosesPlainRuleTable::ReorderingTableFileName.value().toStdString());
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

        if (ReorderingTableFields[mosesFormatSourcePhrase] != ReorderingTableFields[mosesFormatSourcePhrase] ||
                ReorderingTableFields[mosesFormatTargetPhrase] != ReorderingTableFields[mosesFormatTargetPhrase])
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

        PhraseCostsFields.append(ReorderingCostsFields);

        this->addRule(PhraseTableFields[mosesFormatSourcePhrase], PhraseTableFields[mosesFormatTargetPhrase], PhraseCostsFields, RulesRead);
    }

}

void clsMosesPlainRuleTable::addToRuleNodeSorted(clsRuleNode &_ruleNode, clsTargetRule &_targetRule)
{
    QList<clsTargetRule>& TargetRuleList = _ruleNode.targetRules();

    PhraseTable& Evaluator = *static_cast<PhraseTable*>(PhraseTable::instance());

    Cost_t CurrentTargetRuleCost = Evaluator.getTargetRuleCost(0, 0, _targetRule);

    int InsertionPos = TargetRuleList.size();

    int StartPos = 0, EndPos = TargetRuleList.size();
    while(EndPos > StartPos) {
        int MidPos = (StartPos + EndPos) / 2;
        Cost_t TargetRuleCost = Evaluator.getTargetRuleCost(0, 0, TargetRuleList.at(MidPos));
        if(TargetRuleCost > CurrentTargetRuleCost)
            EndPos = MidPos;
        else if (TargetRuleCost < CurrentTargetRuleCost)
            StartPos = MidPos + 1;
        else {
            InsertionPos = MidPos;
            break;
        }
    }
    if(StartPos == EndPos)
        InsertionPos = StartPos;
   TargetRuleList.insert(InsertionPos, _targetRule);
}

void clsMosesPlainRuleTable::addRule(const QString& _sourcePhrase,
                                     const QString& _targetPhrase,
                                     const QStringList &_costs,
                                     size_t _ruleNumber)
{
    Q_UNUSED(_ruleNumber)

    QList<Cost_t>       Costs;
    foreach(const QString& Cost, _costs)
        Costs.append(-log(Cost.toDouble()));

    QVector<WordIndex_t> SourcePhrase;
    foreach(const QString& Word, _sourcePhrase.split(" ", QString::SkipEmptyParts)){
        WordIndex_t WordIndex = gConfigs.SourceVocab.value(Word);
        if (WordIndex == 0 && Word != "<unk>"){
            WordIndex = gConfigs.SourceVocab.size() + 1;
            gConfigs.SourceVocab.insert(Word, WordIndex);
        }
        SourcePhrase.append(WordIndex);
    }


    QList<WordIndex_t> TargetPhrase;
    foreach(const QString& Word, _targetPhrase.split(" ", QString::SkipEmptyParts))
        TargetPhrase.append(gConfigs.EmptyLMScorer->getWordIndex(Word));

    RuleTable::clsTargetRule TargetRule(TargetPhrase, Costs);

    clsRuleNode& RuleNode = this->PrefixTree->getOrCreateNode(SourcePhrase.toStdVector())->getData();
    RuleNode.detachInvalidData();
    addToRuleNodeSorted(RuleNode, TargetRule);
    static int NumberOfTimesDebugDataPrinted = 0;
    if(NumberOfTimesDebugDataPrinted < 15 && _sourcePhrase.trimmed() == "این راکتور") {
        ++NumberOfTimesDebugDataPrinted;
        QList<clsTargetRule>& TargetRuleList = RuleNode.targetRules();

        PhraseTable& Evaluator = *static_cast<PhraseTable*>(PhraseTable::instance());

        std::cout << "Currently added phrase :" << _targetPhrase.toStdString() << "(" << Evaluator.getTargetRuleCost(0, 0, TargetRule) << ")" << std::endl;

        for(int i = 0; i < TargetRuleList.size(); ++i) {
            QString ithTargetPhrase;
            for(size_t j = 0; j < TargetRuleList[i].size(); ++j)
                ithTargetPhrase += gConfigs.EmptyLMScorer->getWordByIndex(TargetRuleList[i].at(j)) + " ";
            std::cout << "[" << i << "] : " << ithTargetPhrase.toStdString() << "(" << Evaluator.getTargetRuleCost(0, 0, TargetRuleList[i]) << ")" << std::endl;
        }
    }
}

}
}
}
}
