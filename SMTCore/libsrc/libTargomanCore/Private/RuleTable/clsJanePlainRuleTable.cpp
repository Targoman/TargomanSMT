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

#include "clsJanePlainRuleTable.h"
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
    janeFormatCostsPosition = 0,
    janeFormatLeftHandSidePosition,
    janeFormatSourcePosition,
    janeFormatTargetPosition,
    janeFormatCountPosition,
    janeFormatStandardNumberOfFields
};


using namespace Common;
using namespace Common::Configuration;
using namespace Common::CompressedStream;
using namespace FeatureFunction;

TARGOMAN_REGISTER_MODULE(clsJanePlainRuleTable)

#define FFCONFIG_KEY_IDENTIFIER "Key"

tmplConfigurable<QString> clsJanePlainRuleTable::FileName(
        clsJanePlainRuleTable::baseConfigPath() + "/FileName",
        "Filename where phrase table is stored",
        "",
        Validators::tmplPathAccessValidator<(enuPathAccess::Type)(enuPathAccess::File | enuPathAccess::Readable)>
        );

tmplConfigurable<QString> clsJanePlainRuleTable::PhraseCostNames(
        clsJanePlainRuleTable::baseConfigPath() + "/CostNames",
        "CostsNames as defined in Jane config File",
        "s2t,t2s,ibm1s2t,ibm1t2s,phrasePenalty,wordPenalty,s2tRatio,t2sRatio,cnt1,cnt2,cnt3");

QList<tmplConfigurable<QString>> clsJanePlainRuleTable::FeatureFunctions = {
    tmplConfigurable<QString>(clsJanePlainRuleTable::baseConfigPath() +
    "/FeatureFunctions/" + FeatureFunction::LexicalReordering::moduleName() + FFCONFIG_KEY_IDENTIFIER,
    "Abbreviation used for each Feature function in the rule table file",
    "lrm")
};

clsJanePlainRuleTable::clsJanePlainRuleTable(quint64 _instanceID)  :
    intfRuleTable(this->moduleName(), _instanceID)
{}

clsJanePlainRuleTable::~clsJanePlainRuleTable()
{
    this->unregister();
}

void clsJanePlainRuleTable::initializeSchema()
{
    TargomanLogInfo(5, "Loading Jane plain text rule set from: " + this->FileName.value());

    this->PrefixTree.reset(new RulesPrefixTree_t());
    QStringList ColumnNames = clsJanePlainRuleTable::PhraseCostNames.value().split(",");

    clsCompressedInputStream InputStream(clsJanePlainRuleTable::FileName.value().toStdString());

    if(InputStream.peek() < 0)
        throw exJanePhraseTable("Empty phrase table.");

    std::string Line;
    getline(InputStream, Line);
    if (Line == "")
        throw exJanePhraseTable("Empty first line.");

    QStringList Fields = QString::fromUtf8(Line.c_str()).split("#");

    if (Fields.size() < 3)
        throw exJanePhraseTable(QString("Bad file format in first line : %2").arg(Line.c_str()));


    PhraseTable::setColumnNames(ColumnNames);

    bool Accepted = false;
    for (unsigned AdditionalFieldIndex = janeFormatStandardNumberOfFields;
         AdditionalFieldIndex < (size_t)Fields.size();
         ++AdditionalFieldIndex) {
        QStringList AdditionalField = Fields.at(AdditionalFieldIndex).split(" ", QString::SkipEmptyParts);
        QString FieldName = AdditionalField.first();
        for(int i = 0; i< clsJanePlainRuleTable::FeatureFunctions.size(); ++i){
            const tmplConfigurable<QString>& FFConfig = clsJanePlainRuleTable::FeatureFunctions.at(i);
            if (FFConfig.value() == FieldName){
                QString FFModuleName = FFConfig.configPath().split("/").last();
                FFModuleName.truncate(FFModuleName.size() - sizeof(FFCONFIG_KEY_IDENTIFIER) + 1);
                if (gConfigs.ActiveFeatureFunctions.value(FFModuleName)->columnNames().size() != AdditionalField.size() - 1)
                    throw exJanePhraseTable("Invalid count of costs for field: " + FFModuleName);
                ColumnNames.append(gConfigs.ActiveFeatureFunctions.value(FFModuleName)->columnNames());
                Accepted = true;
                this->AcceptedAdditionalFieldsIndexes.append(AdditionalFieldIndex);
                break;
            }
        }
        if (Accepted == false){
            TargomanWarn(5,"Unsupported Additional field <" + FieldName + "> ignored!!!");
        }

        clsTargetRule::setColumnNames(ColumnNames);
    }

}

void clsJanePlainRuleTable::loadTableData()
{
    TargomanLogInfo(5, "Loading Jane plain text rule set from: " + this->FileName.value());

    this->PrefixTree.reset(new RulesPrefixTree_t());
    QStringList ColumnNames = clsJanePlainRuleTable::PhraseCostNames.value().split(",");
    size_t      PhraseCostsCount = ColumnNames.size();

    clsCmdProgressBar ProgressBar("Loading RuleTable");

    clsCompressedInputStream InputStream(clsJanePlainRuleTable::FileName.value().toStdString());
    size_t RulesRead = 0;

    while (InputStream.peek() >= 0){
        std::string Line;
        getline(InputStream, Line);
        if (Line == "")
            continue;
        ++RulesRead;
        ProgressBar.setValue(RulesRead);

        QStringList Fields = QString::fromUtf8(Line.c_str()).split("#");

        if (Fields.size() < 3)
            throw exJanePhraseTable(QString("Bad file format in line %1 : %2").arg(RulesRead).arg(Line.c_str()));

        if (Fields[janeFormatLeftHandSidePosition] == "S")
            continue;

        if (Fields[janeFormatCostsPosition].isEmpty()){
            TargomanWarn(5,"Ignoring phrase with empty target side at line: " + QString::number(RulesRead));
            continue;
        }

        QStringList PhraseCostsFields = Fields[janeFormatCostsPosition].split(" ", QString::SkipEmptyParts);
        if ((size_t)PhraseCostsFields.size() < PhraseCostsCount){
            TargomanWarn(5,"Invalid count of costs at line: " + QString::number(RulesRead));
            continue;
        }

        this->addRule(PhraseCostsFields, Fields, this->AcceptedAdditionalFieldsIndexes, RulesRead);
    }

}

void clsJanePlainRuleTable::addRule(const QStringList& _phraseCosts,
                                    const QStringList &_allFields,
                                    const QList<size_t>& _acceptedAdditionalFields,
                                    size_t _ruleNumber)
{
    Q_UNUSED(_ruleNumber)

    QList<Cost_t>      Costs;
    foreach(const QString& Cost, _phraseCosts)
        Costs.append(Cost.toDouble());

    QVector<WordIndex_t> SourcePhrase;
    foreach(const QString& Word, _allFields[janeFormatSourcePosition].split(" ", QString::SkipEmptyParts)){
        WordIndex_t WordIndex = gConfigs.SourceVocab.value(Word);
        if (WordIndex == 0 && Word != "<unknown-word>"){
            WordIndex = gConfigs.SourceVocab.size() + 1;
            gConfigs.SourceVocab.insert(Word, WordIndex);
        }
        SourcePhrase.append(WordIndex);
    }


    QList<WordIndex_t> TargetPhrase;
    foreach(const QString& Word, _allFields[janeFormatTargetPosition].split(" ", QString::SkipEmptyParts))
        TargetPhrase.append(gConfigs.EmptyLMScorer->getWordIndex(Word));

    foreach(size_t FieldIndex, _acceptedAdditionalFields){
        const QStringList& FieldCosts = _allFields.at(FieldIndex).split(" ", QString::SkipEmptyParts);
        for (size_t i = 1; i<(size_t)FieldCosts.size(); ++i)
            Costs.append(FieldCosts.at(i).toDouble());
    }

    clsTargetRule TargetRule(TargetPhrase, Costs);

    clsRuleNode& RuleNode = this->PrefixTree->getOrCreateNode(SourcePhrase.toStdVector())->getData();
    RuleNode.detachInvalidData();
    RuleNode.targetRules().append(TargetRule);
}

}
}
}
}
