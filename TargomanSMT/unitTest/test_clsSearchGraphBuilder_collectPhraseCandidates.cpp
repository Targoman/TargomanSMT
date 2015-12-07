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
 * @author Saeed Torabzadeh <saeed.torabzadeh@targoman.com>
 */

#include "UnitTest.h"

using namespace UnitTestNameSpace;
using namespace Targoman::SMT::Private::SearchGraphBuilder;
using namespace RuleTable;
using namespace InputDecomposer;
using namespace Targoman::Common;

class clsDummyScorerProxy : public Proxies::LanguageModel::intfLMSentenceScorer {
public:
    virtual void init(bool _justVocab) { Q_UNUSED(_justVocab) }
    virtual void initHistory(const intfLMSentenceScorer& _oldScorer) { Q_UNUSED(_oldScorer) }
    virtual void reset(bool _withStartOfSentence = true) { Q_UNUSED(_withStartOfSentence)}
    virtual LogP_t wordProb(const WordIndex_t& _wordIndex) { Q_UNUSED(_wordIndex); return 1;}
    virtual LogP_t endOfSentenceProb() { return 1;}
    virtual WordIndex_t getWordIndex(const QString& _word) { Q_UNUSED(_word); return 1;}
    virtual QString getWordByIndex(WordIndex_t _wordIndex) {Q_UNUSED(_wordIndex); return ""; }
    virtual int compareHistoryWith(const intfLMSentenceScorer& _otherScorer) const {Q_UNUSED(_otherScorer); return 0;}
    virtual void updateFutureStateHash(QCryptographicHash& _hash) const { Q_UNUSED(_hash); }

    clsDummyScorerProxy(int x) : Proxies::LanguageModel::intfLMSentenceScorer(this->moduleName(), x) { }


    TARGOMAN_DEFINE_MODULE("clsDummyScorerProxy", clsDummyScorerProxy);
};

TARGOMAN_REGISTER_MODULE(clsDummyScorerProxy);

class clsDummyRuleTable : public intfRuleTable {
public:

    void addRule(const QList<WordIndex_t> _sourcePhrase,
                 const QList<WordIndex_t> _targetPhrase,
                 const QList<Cost_t> _costs)
    {
        RuleTable::clsTargetRule TargetRule(_targetPhrase, _costs);

        clsRuleNode& RuleNode = this->PrefixTree->getOrCreateNode(_sourcePhrase)->getData();
        if (RuleNode.isInvalid()) {
            RuleNode.detachInvalidData();
        }
        RuleNode.targetRules().append(TargetRule);
    }

    clsDummyRuleTable(int x) : intfRuleTable(this->moduleName(), x) {
        this->PrefixTree.reset(new RulesPrefixTree_t());
    }

    void initializeSchema() {
        clsTargetRule::setColumnNames(QStringList() << "C1" << "C2");
    }

    void loadTableData() {

    }

    TARGOMAN_DEFINE_MODULE("DummyRuleTable", clsDummyRuleTable);
};

TARGOMAN_REGISTER_MODULE(clsDummyRuleTable);

void clsUnitTest::test_clsSearchGraphBuilder_collectPhraseCandidates()
{
    gConfigs.EmptyLMScorer.reset(new clsDummyScorerProxy(0));

    Sentence_t Sentence;
    Sentence << clsToken("word1", 1, "", QVariantMap())
             << clsToken("word2", 2, "", QVariantMap())
             << clsToken("word3", 3, "", QVariantMap())
             << clsToken("word4", 4, "", QVariantMap())
             << clsToken("word5", 5, "", QVariantMap());

    clsDummyRuleTable RuleTable(0);

    for(int i = 0; i < Sentence.size(); ++i)
        RuleTable.addRule(
                    QList<WordIndex_t>() << Sentence.at(i).wordIndexes(),
                    QList<WordIndex_t>() << i + 1,
                    QList<Cost_t>() << 1 << 1
                    );

    RuleTable.addRule(
                QList<WordIndex_t>() << 1 << 2 << 3,
                QList<WordIndex_t>() << 1 << 2 << 3,
                QList<Cost_t>() << 2 << 4
                );
    RuleTable.addRule(
                QList<WordIndex_t>() << 2 << 3 << 4,
                QList<WordIndex_t>() << 1 << 4 << 7,
                QList<Cost_t>() << 1 << 3
                );
    RuleTable.addRule(
                QList<WordIndex_t>() << 2 ,
                QList<WordIndex_t>() << 5 << 7,
                QList<Cost_t>() << 2 << 5
                );
    RuleTable.addRule(
                QList<WordIndex_t>() << 2 << 3 ,
                QList<WordIndex_t>() << 1 << 7 << 4,
                QList<Cost_t>() << 4 << 2
                );

    clsSearchGraph Builder(false, Sentence);
    Builder.pRuleTable = &RuleTable;
    Builder.collectPhraseCandidates();


    QVERIFY( Builder.Data->PhraseCandidateCollections.size() == 5 );
    QVERIFY( Builder.Data->PhraseCandidateCollections.at(0).size() == 5 );
    QVERIFY( Builder.Data->PhraseCandidateCollections.at(0).at(0).isInvalid() == 0 );
    QVERIFY( Builder.Data->PhraseCandidateCollections.at(0).at(0).targetRules().size() == 1 );
    QVERIFY( Builder.Data->PhraseCandidateCollections.at(0).at(0).targetRules().at(0).size() == 1 );
    QVERIFY( Builder.Data->PhraseCandidateCollections.at(0).at(0).targetRules().at(0).at(0) == 1 );
    QVERIFY( Builder.Data->PhraseCandidateCollections.at(0).at(1).isInvalid() == 1 );
    QVERIFY( Builder.Data->PhraseCandidateCollections.at(0).at(2).isInvalid() == 0 );
    QVERIFY( Builder.Data->PhraseCandidateCollections.at(0).at(2).targetRules().size() == 1 );
    QVERIFY( Builder.Data->PhraseCandidateCollections.at(0).at(2).targetRules().at(0).size() == 3 );
    QVERIFY( Builder.Data->PhraseCandidateCollections.at(0).at(2).targetRules().at(0).at(0) == 1 );
    QVERIFY( Builder.Data->PhraseCandidateCollections.at(0).at(2).targetRules().at(0).at(1) == 2 );
    QVERIFY( Builder.Data->PhraseCandidateCollections.at(0).at(2).targetRules().at(0).at(2) == 3 );
    QVERIFY( Builder.Data->PhraseCandidateCollections.at(0).at(3).isInvalid() == 1 );
    QVERIFY( Builder.Data->PhraseCandidateCollections.at(0).at(4).isInvalid() == 1 );
    QVERIFY( Builder.Data->PhraseCandidateCollections.at(1).size() == 4 );
    QVERIFY( Builder.Data->PhraseCandidateCollections.at(1).at(0).isInvalid() == 0 );
    QVERIFY( Builder.Data->PhraseCandidateCollections.at(1).at(0).targetRules().size() == 2 );
    QVERIFY( Builder.Data->PhraseCandidateCollections.at(1).at(0).targetRules().at(0).size() == 1 );
    QVERIFY( Builder.Data->PhraseCandidateCollections.at(1).at(0).targetRules().at(0).at(0) == 2 );
    QVERIFY( Builder.Data->PhraseCandidateCollections.at(1).at(0).targetRules().at(1).size() == 2 );
    QVERIFY( Builder.Data->PhraseCandidateCollections.at(1).at(0).targetRules().at(1).at(0) == 5 );
    QVERIFY( Builder.Data->PhraseCandidateCollections.at(1).at(0).targetRules().at(1).at(1) == 7 );
    QVERIFY( Builder.Data->PhraseCandidateCollections.at(1).at(1).isInvalid() == 0 );
    QVERIFY( Builder.Data->PhraseCandidateCollections.at(1).at(1).targetRules().size() == 1 );
    QVERIFY( Builder.Data->PhraseCandidateCollections.at(1).at(1).targetRules().at(0).size() == 3 );
    QVERIFY( Builder.Data->PhraseCandidateCollections.at(1).at(1).targetRules().at(0).at(0) == 1 );
    QVERIFY( Builder.Data->PhraseCandidateCollections.at(1).at(1).targetRules().at(0).at(1) == 7 );
    QVERIFY( Builder.Data->PhraseCandidateCollections.at(1).at(1).targetRules().at(0).at(2) == 4 );
    QVERIFY( Builder.Data->PhraseCandidateCollections.at(1).at(2).isInvalid() == 0 );
    QVERIFY( Builder.Data->PhraseCandidateCollections.at(1).at(2).targetRules().size() == 1 );
    QVERIFY( Builder.Data->PhraseCandidateCollections.at(1).at(2).targetRules().at(0).size() == 3 );
    QVERIFY( Builder.Data->PhraseCandidateCollections.at(1).at(2).targetRules().at(0).at(0) == 1 );
    QVERIFY( Builder.Data->PhraseCandidateCollections.at(1).at(2).targetRules().at(0).at(1) == 4 );
    QVERIFY( Builder.Data->PhraseCandidateCollections.at(1).at(2).targetRules().at(0).at(2) == 7 );
    QVERIFY( Builder.Data->PhraseCandidateCollections.at(1).at(3).isInvalid() == 1 );
    QVERIFY( Builder.Data->PhraseCandidateCollections.at(2).size() == 3 );
    QVERIFY( Builder.Data->PhraseCandidateCollections.at(2).at(0).isInvalid() == 0 );
    QVERIFY( Builder.Data->PhraseCandidateCollections.at(2).at(0).targetRules().size() == 1 );
    QVERIFY( Builder.Data->PhraseCandidateCollections.at(2).at(0).targetRules().at(0).size() == 1 );
    QVERIFY( Builder.Data->PhraseCandidateCollections.at(2).at(0).targetRules().at(0).at(0) == 3 );
    QVERIFY( Builder.Data->PhraseCandidateCollections.at(2).at(1).isInvalid() == 1 );
    QVERIFY( Builder.Data->PhraseCandidateCollections.at(2).at(2).isInvalid() == 1 );
    QVERIFY( Builder.Data->PhraseCandidateCollections.at(3).size() == 2 );
    QVERIFY( Builder.Data->PhraseCandidateCollections.at(3).at(0).isInvalid() == 0 );
    QVERIFY( Builder.Data->PhraseCandidateCollections.at(3).at(0).targetRules().size() == 1 );
    QVERIFY( Builder.Data->PhraseCandidateCollections.at(3).at(0).targetRules().at(0).size() == 1 );
    QVERIFY( Builder.Data->PhraseCandidateCollections.at(3).at(0).targetRules().at(0).at(0) == 4 );
    QVERIFY( Builder.Data->PhraseCandidateCollections.at(3).at(1).isInvalid() == 1 );
    QVERIFY( Builder.Data->PhraseCandidateCollections.at(4).size() == 1 );
    QVERIFY( Builder.Data->PhraseCandidateCollections.at(4).at(0).isInvalid() == 0 );
    QVERIFY( Builder.Data->PhraseCandidateCollections.at(4).at(0).targetRules().size() == 1 );
    QVERIFY( Builder.Data->PhraseCandidateCollections.at(4).at(0).targetRules().at(0).size() == 1 );
    QVERIFY( Builder.Data->PhraseCandidateCollections.at(4).at(0).targetRules().at(0).at(0) == 5 );



    //    PRINT_QVERIFY(Builder.Data->PhraseCandidateCollections.size());
    //    for(int i = 0; i < Builder.Data->PhraseCandidateCollections.size(); ++i) {
    //        PRINT_QVERIFY(Builder.Data->PhraseCandidateCollections.at(i).size(),i);
    //        for(int j = 0; j < Builder.Data->PhraseCandidateCollections.at(i).size(); ++j) {
    //            PRINT_QVERIFY(Builder.Data->PhraseCandidateCollections.at(i).at(j).isInvalid(),i,j);
    //            if(Builder.Data->PhraseCandidateCollections.at(i).at(j).isInvalid() == false) {
    //                PRINT_QVERIFY(Builder.Data->PhraseCandidateCollections.at(i).at(j).targetRules().size(),i,j);
    //                for(int k = 0; k < Builder.Data->PhraseCandidateCollections.at(i).at(j).targetRules().size(); ++k) {
    //                    PRINT_QVERIFY(Builder.Data->PhraseCandidateCollections.at(i).at(j).targetRules().at(k).size(),i,j,k);
    //                    for(int l = 0; l < (int)Builder.Data->PhraseCandidateCollections.at(i).at(j).targetRules().at(k).size(); ++l)
    //                    {
    //                        PRINT_QVERIFY(Builder.Data->PhraseCandidateCollections.at(i).at(j).targetRules().at(k).at(l),i,j,k,l);
    //                    }
    //                }
    //            }
    //        }
    //    }
}
