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
#include <functional>

#include "clsSearchGraph.h"
#include "../GlobalConfigs.h"
#include "Private/Proxies/LanguageModel/intfLMSentenceScorer.hpp"
#include "Private/SpecialTokenHandler/SpecialTokensRegistry.hpp"
#include "Private/SpecialTokenHandler/OOVHandler/OOVHandler.h"
#include <iostream>
#include <iomanip>
#include <sstream>


// Torabzadeh
namespace Targoman {
namespace SMT {
namespace Private {
namespace RuleTable {
    Cost_t getPrematureargetRuleCost(const clsTargetRule& _targetRule);
}
}
}
}


#define PBT_MAXIMUM_COST 1e200

namespace Targoman{
namespace SMT {
namespace Private{
namespace SearchGraphBuilder {

using namespace Common;
using namespace Common::Configuration;
using namespace RuleTable;
using namespace Proxies;
using namespace Proxies::LanguageModel;
using namespace InputDecomposer;
using namespace SpecialTokenHandler;
using namespace SpecialTokenHandler::OOV;

tmplRangedConfigurable<quint8> clsSearchGraph::HardReorderingJumpLimit(
        MAKE_CONFIG_PATH("HardReorderingJumpLimit"),
        "Maximum jump width limit. Hard constrain.",
        1,64,
        6,
        [] (const intfConfigurable& _item, QString&) {
            clsCardinalityHypothesisContainer::setHardReorderingJumpLimit(
                    _item.toVariant().Int
            );
            return true;
        });

tmplRangedConfigurable<quint8> clsSearchGraph::ReorderingConstraintMaximumRuns(
        MAKE_CONFIG_PATH("ReorderingConstraintMaximumRuns"),
        "IBM1 reordering constraint",
        0,100,
        2);

tmplConfigurable<bool>   clsSearchGraph::DoComputePositionSpecificRestCosts(
        MAKE_CONFIG_PATH("DoComputePositionSpecificRestCosts"),
        "Compute position specific RestCosts (default) or not",
        true);

tmplRangedConfigurable<quint8> clsPhraseCandidateCollectionData::MaxTargetPhraseCount(
        MAKE_CONFIG_PATH("MaxTargetPhraseCount"),
        "Maximum target phrases collected before decoding",
        1,254,
        100);

tmplConfigurable<bool>   clsSearchGraph::DoPrunePreInsertion(
        MAKE_CONFIG_PATH("PrunePreInsertion"),
        "Prune hypotheses before insertion(default) or not",
        true);

FeatureFunction::intfFeatureFunction*  clsSearchGraph::pPhraseTable = NULL;
RuleTable::intfRuleTable*              clsSearchGraph::pRuleTable = NULL;
RuleTable::clsRuleNode*                clsSearchGraph::UnknownWordRuleNode;

/**********************************************************************************/
clsSearchGraph::clsSearchGraph(const Sentence_t& _sentence):
    Data(new clsSearchGraphData(_sentence))
{
    this->collectPhraseCandidates();
    this->decode();
}
/**
 * @brief Loads rule and phrase tables, inititializes all feature functions and sets #UnknownWordRuleNode.
 * @param _configFilePath Address of config file.
 */
void clsSearchGraph::init(QSharedPointer<QSettings> _configSettings, bool isDecoding)
{
    clsSearchGraph::pRuleTable = gConfigs.RuleTable.getInstance<intfRuleTable>();

    clsSearchGraph::pRuleTable->initializeSchema();

    //InvalidTargetRuleData has been marshalled here because it depends on loading RuleTable
    RuleTable::InvalidTargetRuleData = new RuleTable::clsTargetRuleData;

    //pInvalidTargetRule has been marshalled here because it depends on instantiation of InvalidTargetRuleData
    RuleTable::pInvalidTargetRule = new RuleTable::clsTargetRule;
    //InvalidSearchGraphNodeData has been marshalled here because it depends on initialization of gConfigs
    InvalidSearchGraphNodeData = new clsSearchGraphNodeData;
    pInvalidSearchGraphNode = new clsSearchGraphNode;

    foreach (FeatureFunction::intfFeatureFunction* FF, gConfigs.ActiveFeatureFunctions)
        FF->initialize(_configSettings);
    if(!isDecoding){
        return;
    }
    clsSearchGraph::pRuleTable->loadTableData();
    clsSearchGraph::pPhraseTable = gConfigs.ActiveFeatureFunctions.value("PhraseTable");

    RulesPrefixTree_t::pNode_t Node = clsSearchGraph::pRuleTable->prefixTree().rootNode();
    if(Node->isInvalid())
        throw exSearchGraph("Invalid empty Rule Table");

    Node = Node->follow(Constants::SrcVocabUnkWordIndex); //Search for UNKNOWN word Index
    if (Node->isInvalid())
        throw exSearchGraph("No Rule defined for UNKNOWN word");


    clsSearchGraph::UnknownWordRuleNode = new clsRuleNode(Node->getData(), true);

        TargomanLogInfo(7, "Checking Source Vocab");
        try{
//            QList<QHash<QString, Common::WordIndex_t>::iterator> ItersToRemove;
            /// @note As a result of aligning some words to NULL by general word aligners, we need to take care of
            ///       tokens that have a word index but only contribute to multi-word phrases. These will cause
            ///       malfunction of OOV handler module as it will assume these words have translations by themselves
//            for(auto TokenIter = gConfigs.SourceVocab.begin();
//                TokenIter != gConfigs.SourceVocab.end();
//                ++TokenIter) {
//
                QList<WordIndex_t> Children = clsSearchGraph::pRuleTable->prefixTree().rootNode()->getChildren();
//                TargomanLogInfo(7, "number of root's children: " << Children.size());
                for(size_t i = 0; i < Children.size(); i++){
                    clsRuleNode& SingleWordRuleNode =
                                            clsSearchGraph::pRuleTable->prefixTree().getNode(Children[i])->getData();
                    if(!SingleWordRuleNode.isInvalid() &&
                            !SingleWordRuleNode.targetRules().isEmpty())
                        gConfigs.VocabWithSingleWordRule.insert(Children[i]);
//                    if(i % 100 == 0)
//                        TargomanLogInfo(7, i);
                }
//                    ItersToRemove.append(TokenIter);
                /*if(SingleWordRuleNode.isInvalid())
                    SingleWordRuleNode.detachInvalidData();

                if(SingleWordRuleNode.targetRules().isEmpty())
                    SingleWordRuleNode.targetRules().append(
                                OOVHandler::instance().generateTargetRules(TokenIter.key())
                                );*/
//            }
            TargomanLogWarn(8, "Number of existing vocabs for which there is single word rule:"
                            << gConfigs.VocabWithSingleWordRule.size());

//            foreach(auto TokenIter, ItersToRemove)
    //            gConfigs.SourceVocab.erase(TokenIter);
//                gConfigs.VocabWithoutSingleWordRule.insert(TokenIter.key());

        }catch(exTargomanNotImplemented &e){
            throw exTargomanNotImplemented(
                        e.what() +
                        ". Maybe you are using an incompatible BinaryRuleTable.");
        }
    
    TargomanLogInfo(7, "Search Graph Initialized successfully.");
}

void clsSearchGraph::extendSourcePhrase(const QList<WordIndex_t>& _wordIndexes,
                                        INOUT QList<RulesPrefixTree_t::pNode_t>& _prevNodes,
                                        QList<clsRuleNode>& _ruleNodes)
{
    QList<RulesPrefixTree_t::pNode_t> NextNodes;
    foreach(RulesPrefixTree_t::pNode_t PrevNode, _prevNodes) {
        foreach(WordIndex_t WordIndex, _wordIndexes) {
            RulesPrefixTree_t::pNode_t NextNode = PrevNode->follow(WordIndex);
            if(NextNode->isInvalid() == false) {
                _ruleNodes.append(NextNode->getData());
                NextNodes.append(NextNode);
            }
        }
    }
    _prevNodes = NextNodes;
}

/**
 * @brief Looks up prefix tree for all phrases that matches with some parts of input sentence and stores them in the
 * PhraseCandidateCollections of #Data. This function also calculates maximum length of matching source phrase with phrase table.
 */
void clsSearchGraph::collectPhraseCandidates()
{
    // TODO: When looking for phrases containing IXML tags, search both for the tagged version
    // and surface form version, e.g. "I ate <num>3</num>" => search for "I ate <num/>" and "I ate 3"
    this->Data->MaxMatchingSourcePhraseCardinality = 0;
    for (size_t FirstPosition = 0; FirstPosition < (size_t)this->Data->Sentence.size(); ++FirstPosition) {
        this->Data->PhraseCandidateCollections.append(QVector<clsPhraseCandidateCollection>(this->Data->Sentence.size() - FirstPosition));
        QList<RulesPrefixTree_t::pNode_t> PrevNodes =
                QList<RulesPrefixTree_t::pNode_t>() << this->pRuleTable->prefixTree().rootNode();

        if(true /* On 1-grams */)
        {
            QList<clsRuleNode> RuleNodes;
            this->extendSourcePhrase(this->Data->Sentence.at(FirstPosition).wordIndexes(),
                                     PrevNodes,
                                     RuleNodes);

            foreach(WordIndex_t WordIndex, this->Data->Sentence.at(FirstPosition).wordIndexes()) {
                clsRuleNode SpecialRuleNode = SpecialTokensRegistry::instance().getRuleNode(WordIndex);
                if(SpecialRuleNode.isInvalid() == false)
                    RuleNodes.append(SpecialRuleNode);
            }

            RuleNodes.append(this->Data->Sentence.at(FirstPosition).temporaryRuleNode());

            if(RuleNodes.isEmpty())
                RuleNodes.append(*clsSearchGraph::UnknownWordRuleNode);

            this->Data->MaxMatchingSourcePhraseCardinality = qMax(this->Data->MaxMatchingSourcePhraseCardinality, 1);

            this->Data->PhraseCandidateCollections[FirstPosition][0] = clsPhraseCandidateCollection(FirstPosition, FirstPosition + 1, this->Data->Sentence, RuleNodes);
/*
            for(int i = 0; i < this->Data->PhraseCandidateCollections[FirstPosition][0].targetRules().size(); i++){

               // for(int j = FirstPosition; j <= LastPosition; j++)
                    std::cout << this->Data->Sentence.at(FirstPosition).string().toStdString() << " ";
                              //<<this->Data->PhraseCandidateCollections[FirstPosition][0].targetRules()[i];

                std::cout << "\t" <<  this->Data->PhraseCandidateCollections[FirstPosition][0].targetRules()[i].toStr().toStdString()
                          << "\t" ;

                for(int j = 0; j < this->Data->PhraseCandidateCollections[FirstPosition][0].targetRules()[i].PrecomputedValuesSize; j++ ){
                    std::cout << this->Data->PhraseCandidateCollections[FirstPosition][0].targetRules()[i].precomputedValue(j)
                            << "\t";
                }
                std::cout << std::endl;
            }
*/
        }

        for (size_t LastPosition = FirstPosition + 1; LastPosition < (size_t)this->Data->Sentence.size() ; ++LastPosition){

            QList<clsRuleNode> RuleNodes;
            extendSourcePhrase(this->Data->Sentence.at(LastPosition).wordIndexes(), PrevNodes, RuleNodes);

            if (RuleNodes.isEmpty())
                break; // Appending next word breaks phrase lookup

            this->Data->PhraseCandidateCollections[FirstPosition][LastPosition - FirstPosition] = clsPhraseCandidateCollection(FirstPosition, LastPosition + 1,
                                                                                                                               this->Data->Sentence, RuleNodes);
            this->Data->MaxMatchingSourcePhraseCardinality = qMax(this->Data->MaxMatchingSourcePhraseCardinality,
                                                                      (int)(LastPosition - FirstPosition + 1));
/*
            for(int i = 0; i < this->Data->PhraseCandidateCollections[FirstPosition][LastPosition - FirstPosition].targetRules().size(); i++){

                for(int j = FirstPosition; j <= LastPosition; j++)
                    std::cout << this->Data->Sentence.at(j).string().toStdString() << " ";

                std::cout << "\t" << this->Data->PhraseCandidateCollections[FirstPosition][LastPosition - FirstPosition].targetRules()[i].toStr().toStdString()
                          << "\t";
                for(int j = 0; j < this->Data->PhraseCandidateCollections[FirstPosition][LastPosition - FirstPosition].targetRules()[i].PrecomputedValuesSize; j++ ){
                    std::cout << this->Data->PhraseCandidateCollections[FirstPosition][LastPosition - FirstPosition].targetRules()[i].precomputedValue(j)
                            << "\t";
                }
                std::cout << std::endl;
            }
*/
        }
    }
}

/**
 * @brief This function checks IBM1 Constraints.
 *
 * Checks whether number of zeros before last 1 in the input coverage is less than #ReorderingConstraintMaximumRuns or not.
 * @param _newCoverage
 * @return
 */
bool clsSearchGraph::conformsIBM1Constraint(const Coverage_t& _newCoverage)
{
    //Find last bit set then check how many bits are zero before this.
    for(int i=_newCoverage.size() - 1; i>=0; --i)
        if(_newCoverage.testBit(i)){
            size_t CountOfPrevZeros = _newCoverage.count(false) + i - _newCoverage.size() + 1;
            return (CountOfPrevZeros <= this->ReorderingConstraintMaximumRuns.value());
        }
    return true;
}

bool clsSearchGraph::conformsHardReorderingJumpLimit(const Coverage_t& _prevCoverage, size_t _prevStart, size_t _prevEnd,
                                                     size_t _startPos, size_t _endPos)
{
    // For empty previous coverage
    if(_prevStart == _prevEnd) {
        // No need to check _startPos as it must be less than or equal to _endPos
//        if(_startPos > clsSearchGraph::HardReorderingJumpLimit.value())
//            return false;
        if(_endPos > clsSearchGraph::HardReorderingJumpLimit.value())
            return false;
        return true;
    }

    int JumpWidth = abs(_prevEnd - _startPos);
    if(JumpWidth > clsSearchGraph::HardReorderingJumpLimit.value())
        return false;

    int FirstEmptyPosition = _prevCoverage.size();
    for(int i = 0; i < _prevCoverage.size(); ++i)
        if(_prevCoverage.testBit(i) == false) {
            FirstEmptyPosition = i;
            break;
        }

    JumpWidth = qAbs((int)_endPos - FirstEmptyPosition);
    return JumpWidth <= clsSearchGraph::HardReorderingJumpLimit.value();
}

#ifdef TARGOMAN_SHOW_DEBUG
float roundDouble(double inputNumber)
{
    return (float)((int(100 * inputNumber + 0.5))/100.0);
}
#endif



/**
 * @brief This is the main function that performs the decoding process.
 *
 * This function first initializes the rest cost matrix in order to be able to calculate rest costs in each path of translation.
 * Then in order to seperate all search graph nodes, this function seperates them by their number of translated words. This notion is called cardinality.
 * For each cardinality loops over all posible previous cardinalities and for each one of them finds all nodes with those cardinalities.
 * In the next step, these founded previous nodes, based on their translation coverage, will be expanded to make new search
 * graph nodes with NewCardinality size. In this process, some of new Nodes will be prunned to have valid and more
 * promising search graph nodes.
 *
 * @return Returns whether it was able to find a translation or not.
 */
bool clsSearchGraph::decode()
{
    this->Data->HypothesisHolder.clear();
    this->Data->HypothesisHolder.resize(this->Data->Sentence.size() + 1);

    this->initializeRestCostsMatrix();

    int PrunedByHardReorderingJumpLimit = 0;

    for (int NewCardinality = 1; NewCardinality <= this->Data->Sentence.size(); ++NewCardinality){

//        int PrunedByIBMConstraint = 0;
//        int PrunedByHardReorderingJumpLimit = 0;
        int PrunedPreInsertion = 0;

        bool IsFinal = (NewCardinality == this->Data->Sentence.size());
        int MinPrevCardinality = qMax(NewCardinality - this->Data->MaxMatchingSourcePhraseCardinality, 0);

        clsCardinalityHypothesisContainer& CurrCardHypoContainer =
                this->Data->HypothesisHolder[NewCardinality];

        for (int PrevCardinality = MinPrevCardinality;
             PrevCardinality < NewCardinality; ++PrevCardinality) {

            unsigned short NewPhraseCardinality = NewCardinality - PrevCardinality;

            clsCardinalityHypothesisContainer& PrevCardHypoContainer =
                    this->Data->HypothesisHolder[PrevCardinality];

            //This happens when we have for ex. 2 bi-grams and a quad-gram but no similar 3-gram. due to bad training
            if(PrevCardHypoContainer.isEmpty()) {
                TargomanLogWarn(1, "Previous cardinality is empty. (PrevCard: " << PrevCardinality << ", CurrentCard: " << NewCardinality << ")");
                continue;
            }

            for(CoverageLexicalHypothesisMap_t::Iterator PrevCoverageIter = PrevCardHypoContainer.lexicalHypotheses().begin();
                PrevCoverageIter != PrevCardHypoContainer.lexicalHypotheses().end();
                ++PrevCoverageIter){


                const Coverage_t& PrevCoverage = PrevCoverageIter.key();
                clsLexicalHypothesisContainer& PrevLexHypoContainer = PrevCoverageIter.value();

                Q_ASSERT(PrevCoverage.count(true) == PrevCardinality);
                Q_ASSERT(PrevLexHypoContainer.nodes().size());


                // This can be removed if training has been done properly and we have a sane phrase table
                if (PrevLexHypoContainer.nodes().isEmpty()){
                    TargomanLogWarn(1, "PrevLexHypoContainer is empty. (PrevCard: " << PrevCardinality
                                  << " PrevCov: " << PrevCoverage << ")");
                    continue;
                }

                for (size_t NewPhraseBeginPos = 0;
                     NewPhraseBeginPos <= (size_t)this->Data->Sentence.size() - NewPhraseCardinality;
                     ++NewPhraseBeginPos){
                    size_t NewPhraseEndPos = NewPhraseBeginPos + NewPhraseCardinality;

                    // Skip if phrase coverage is not compatible with previous sentence coverage
                    bool SkipStep = false;
                    for (size_t i= NewPhraseBeginPos; i<NewPhraseEndPos; ++i)
                        if (PrevCoverage.testBit(i)){
                            SkipStep = true;
                            break;
                        }
                    if (SkipStep)
                        continue;//TODO if NewPhraseCardinality has not contigeous place breaK

                    Coverage_t NewCoverage(PrevCoverage);
                    for (size_t i=NewPhraseBeginPos; i<NewPhraseEndPos; ++i)
                        NewCoverage.setBit(i);

                    /*
                    if (this->conformsIBM1Constraint(NewCoverage) == false){
                        ++PrunedByIBMConstraint;
                        continue;
                    }
                    */

//                    if (this->conformsHardReorderingJumpLimit(NewCoverage, NewPhraseEndPos) == false){
//                        ++PrunedByHardReorderingJumpLimit;
//                        continue;
//                    }

                    clsPhraseCandidateCollection& PhraseCandidates =
                            this->Data->PhraseCandidateCollections[NewPhraseBeginPos][NewPhraseCardinality - 1];

                    //There is no rule defined in rule table for current phrase
                    if (PhraseCandidates.isInvalid())
                        continue; //TODO If there are no more places to fill after this startpos break

                    Cost_t RestCost =  this->calculateRestCost(NewCoverage, NewPhraseBeginPos, NewPhraseEndPos);

                    CurrCardHypoContainer.setLexicalHypothesis(NewCoverage);

                    foreach (const clsSearchGraphNode& PrevLexHypoNode, PrevLexHypoContainer.nodes()) {

                        if (this->conformsHardReorderingJumpLimit(
                                    PrevLexHypoNode.coverage(),
                                    PrevLexHypoNode.sourceRangeBegin(),
                                    PrevLexHypoNode.sourceRangeEnd(),
                                    NewPhraseBeginPos,
                                    NewPhraseEndPos
                                  ) == false){
                            ++PrunedByHardReorderingJumpLimit;
                            continue;
                        }

                        size_t MaxCandidates = PhraseCandidates.usableTargetRuleCount();

                        for(size_t i = 0; i<MaxCandidates; ++i){

                            const clsTargetRule& CurrentPhraseCandidate = PhraseCandidates.targetRules().at(i);


                            clsSearchGraphNode NewHypoNode(PrevLexHypoNode,
                                                           NewPhraseBeginPos,
                                                           NewPhraseEndPos,
                                                           NewCoverage,
                                                           CurrentPhraseCandidate,
                                                           this->Data->Sentence,
                                                           IsFinal,
                                                           RestCost);
                            // If current NewHypoNode is worse than worst stored node ignore it
                            if (clsSearchGraph::DoPrunePreInsertion.value() &&
                                CurrCardHypoContainer.mustBePruned(NewHypoNode.getTotalCost())){
                                ++PrunedPreInsertion;
                                continue;
                            }

                            if(CurrCardHypoContainer.insertNewHypothesis(NewHypoNode)) {
                                // Log insertion of hypothesis here if it is needed
//                                auto cov2str = [] (const Coverage_t _coverage) {
//                                    QString result;
//                                    QTextStream stream;
//                                    stream.setString(&result);
//                                    stream << _coverage;
//                                    return result;
//                                };

//                                std::stringstream Stream;
//                                Stream << NewHypoNode.getNodeNumber() << " ||| ";
////                                Stream << std::fixed << std::setprecision(3) << SelectedNode.getTotalCost() << "\t";
//                                Stream << "Cardinality:  ";
////                                Stream << car2str(NewCardinality).toUtf8().constData();
//                                Stream << "  Coverage:  " << cov2str(NewHypoNode.coverage()).toUtf8().constData();

//                                QString res = "";
//                                for(auto ffIter = gConfigs.ActiveFeatureFunctions.constBegin(); ffIter != gConfigs.ActiveFeatureFunctions.constEnd(); ffIter++){

//                                    size_t index = ffIter.value()->getDataIndex();
//                                    if(NewHypoNode.featureFunctionDataAt(index) != NULL){

//                                        QStringList ffName = ffIter.key().split("/");
//                                        QStringList colName = ffIter.value()->columnNames();
//                                        QVector<Cost_t> costs = NewHypoNode.featureFunctionDataAt(index)->costElements();
//                                        for(int j = 0; j < costs.size(); j++){
//                                            if(colName.size() > j) res += " " + ffName.back() + "/" + colName[j];
//                                            else res += " " + ffName.back();
//                                            res += "= " + QString::number(-costs[j]);

//                                        }
//                                    }
//                                }
//                                Stream << " " << res.toStdString();
//                                Stream << " |||  Cost:  " << std::setprecision(3) << NewHypoNode.getCost()
//                                       << " , RestCost: " << std::setprecision(3) << (NewHypoNode.getTotalCost() - NewHypoNode.getCost())
//                                       << " , Str: (" << NewHypoNode.prevNode().targetRule().toStr().toUtf8().constData()
//                                       << ")" << NewHypoNode.targetRule().toStr().toUtf8().constData() << std::endl;
//                                std::cout << Stream.str().c_str();
                            }
                        }
                    }//foreach PrevLexHypoNode

                    CurrCardHypoContainer.removeSelectedLexicalHypothesisIfEmpty();

                }//for NewPhraseBeginPos
            }//for PrevCoverageIter
        }//for PrevCardinality
        CurrCardHypoContainer.finlizePruningAndcleanUp();

#ifdef TARGOMAN_SHOW_DEBUG
        // Vedadian
        /*
        auto car2str = [] (int _cardinality) {
            QString result;
            for(int i = 0; i < 5; ++i) {
                result = ('0' + _cardinality % 10) + result;
                _cardinality /= 10;
            }
            return result;
        };
        auto cov2str = [] (const Coverage_t _coverage) {
            QString result;
            QTextStream stream;
            stream.setString(&result);
            stream << _coverage;
            return result;
        };
        if(CurrCardHypoContainer.lexicalHypotheses().size() != 0) {
            for(auto Iterator = CurrCardHypoContainer.lexicalHypotheses().end() - 1;
                true;
                --Iterator) {
                const QList<clsSearchGraphNode>& Nodes = Iterator->nodes();
                foreach(const clsSearchGraphNode& SelectedNode, Nodes) {
                    std::stringstream Stream;
                    Stream << std::fixed << std::setprecision(3) << SelectedNode.getTotalCost() << "\t";
                    Stream << "Cardinality:  ";
                    Stream << car2str(NewCardinality).toUtf8().constData();
                    Stream << "  Coverage:  " << cov2str(SelectedNode.coverage()).toUtf8().constData();
                    Stream << "  Cost:  " << std::setprecision(3) << SelectedNode.getCost()
                           << " , RestCost: " << std::setprecision(3) << (SelectedNode.getTotalCost() - SelectedNode.getCost())
                           << " , Str: (" << SelectedNode.prevNode().targetRule().toStr().toUtf8().constData()
                           << ")" << SelectedNode.targetRule().toStr().toUtf8().constData() << std::endl;
                    std::cout << Stream.str().c_str();
                    //TargomanDebug(5, QString::fromUtf8(Stream.str().c_str()));
                }
                if(Iterator == CurrCardHypoContainer.lexicalHypotheses().begin())
                    break;
            }
        }
        std::cout << "\n\n\n";
        //TargomanDebug(5,"\n\n\n");
        //*/
#endif
    }//for NewCardinality

    Coverage_t FullCoverage;
    FullCoverage.fill(1, this->Data->Sentence.size());

    if(this->Data->HypothesisHolder[this->Data->Sentence.size()].lexicalHypotheses().size() > 0 &&
            this->Data->HypothesisHolder[this->Data->Sentence.size()][FullCoverage].nodes().isEmpty() == false)
    {
        this->Data->GoalNode = &this->Data->HypothesisHolder[this->Data->Sentence.size()][FullCoverage].nodes().bestNode();
        this->Data->HypothesisHolder[this->Data->Sentence.size()][FullCoverage].finalizeRecombination();
        PrintBestPathInfo();
        return true;
    } else {
        static clsSearchGraphNode InvalidGoalNode;
        this->Data->GoalNode = &InvalidGoalNode;
        TargomanLogWarn(1, "No translation option for: " << this->Data->Sentence);
        return false;
    }
}

void clsSearchGraph::PrintBestPathInfo(){
    TargomanLogInfo(8, "Translation Score: " << this->Data->GoalNode->getTotalCost());

    clsSearchGraphNode Hypo = *this->Data->GoalNode;
    QList<int> Alignments;
    QVector<clsSearchGraphNode> Nodes;

    while(!Hypo.isInvalid()){
        Nodes.push_back(Hypo);
        Hypo = Hypo.prevNode();
    }
    int TargetIndex = 0;
    for(int node = (int)Nodes.size() - 1; node >= 0; node--){
        Hypo = Nodes[node];
        if(Hypo.targetRule().alignmentDataAvailable()){
            for(size_t i = Hypo.sourceRangeBegin(); i <= Hypo.sourceRangeEnd(); i++){
                QList<int> Aligned = Hypo.targetRule().wordLevelAlignment(i - Hypo.sourceRangeBegin());
                for(int j = 0; j < Aligned.size(); j++){
                    Alignments.push_back(i);
                    Alignments.push_back(Aligned[j] + TargetIndex);
                }

            }
        }
        TargetIndex += Hypo.targetRule().size();
    }

    QString Alignment = "";
    for(int i = 0; i < Alignments.size(); i += 2){
        Alignment += QString::number(Alignments[i]) + "-" + QString::number(Alignments[i + 1]) + " ";
    }
    TargomanLogInfo(8, "Translation Alignment:  " << Alignment);

    for(int node = (int)Nodes.size() - 1; node >= 0; node--){
        Hypo = Nodes[node];
        QString SrcPhrase = "";
        for(size_t i = Hypo.sourceRangeBegin(); i < Hypo.sourceRangeEnd(); i++){
            SrcPhrase += this->Data->Sentence.at(i).string();
            SrcPhrase += " ";
        }
        TargomanLogInfo(9, "[ " + SrcPhrase + "] Translated to [ " +
                        Hypo.targetRule().toStr() + " ] => Hypo Score : " + QString::number(Hypo.getCost()));
    }
}

/**
 * @brief Initializes rest cost matrix
 *
 * For every possible range of words of input sentence, finds approximate cost of every feature fucntions, then
 * tries to reduce that computed rest cost if sum of rest cost of splited phrase is less than whole phrase rest cost.
 */
void clsSearchGraph::initializeRestCostsMatrix()
{
    this->Data->RestCostMatrix.resize(this->Data->Sentence.size());
    for (int SentenceStartPos=0; SentenceStartPos<this->Data->Sentence.size(); ++SentenceStartPos)
        this->Data->RestCostMatrix[SentenceStartPos].fill(
                PBT_MAXIMUM_COST,
                this->Data->Sentence.size() - SentenceStartPos);

    for(size_t FirstPosition = 0; FirstPosition < (size_t)this->Data->Sentence.size(); ++FirstPosition){
        size_t MaxLength = qMin(this->Data->Sentence.size() - FirstPosition,
                                (size_t)this->Data->MaxMatchingSourcePhraseCardinality);
        for(size_t Length = 1; Length <= MaxLength; ++Length){
            this->Data->RestCostMatrix[FirstPosition][Length - 1]  = this->Data->PhraseCandidateCollections[FirstPosition][Length-1].bestApproximateCost();
        }
    }

    for(size_t Length = 2; Length <= (size_t)this->Data->Sentence.size(); ++Length)
        for(size_t FirstPosition = 0; FirstPosition + Length <= (size_t)this->Data->Sentence.size(); ++FirstPosition)
            for(size_t SplitPosition = 1; SplitPosition < Length; ++SplitPosition){
                Cost_t SumSplit = this->Data->RestCostMatrix[FirstPosition][SplitPosition - 1] +
                        this->Data->RestCostMatrix[FirstPosition + SplitPosition][Length - 1 - SplitPosition];
                this->Data->RestCostMatrix[FirstPosition][Length - 1]  =
                        qMin(
                            this->Data->RestCostMatrix[FirstPosition][Length - 1],
                            SumSplit);
            }
}

/**
 * @brief This function approximates rest cost of translation for every feature function.
 * This approximation is based on covered word of transltion and begin and end position of source sentence words.
 * @param _coverage Covered word for translation.
 * @param _beginPos start postion of source sentence.
 * @param _endPos end position of source sentence
 * @note _beginPos and _endPos helps us to infer previous node coverage.
 * @return returns approximate cost of rest cost.
 */

Cost_t clsSearchGraph::calculateRestCost(const Coverage_t& _coverage, size_t _beginPos, size_t _endPos) const
{
    Cost_t RestCosts = 0.0;
    size_t StartPosition = 0;
    size_t Length = 0;

    for(size_t i=0; i < (size_t)_coverage.size(); ++i)
        if(_coverage.testBit(i) == false){ // start of contiguous zero bits.
            if(Length == 0)
                StartPosition = i;
            ++Length;
        }else if(Length){ // end of contiguous zero bits.
            RestCosts += this->Data->RestCostMatrix[StartPosition][Length-1];
            Length = 0;
        }
    if(Length)
        RestCosts += this->Data->RestCostMatrix[StartPosition][Length-1];

    if(clsSearchGraph::DoComputePositionSpecificRestCosts.value()) {
        foreach(FeatureFunction::intfFeatureFunction* FF, gConfigs.ActiveFeatureFunctions.values()) {
            if(FF->canComputePositionSpecificRestCost())
                RestCosts += FF->getRestCostForPosition(_coverage, _beginPos, _endPos);
        }
    }
    return RestCosts;
}

clsPhraseCandidateCollectionData::clsPhraseCandidateCollectionData(size_t _beginPos, size_t _endPos, const InputDecomposer::Sentence_t& _input, const QList<clsRuleNode> &_ruleNodes)
{
    foreach(const clsRuleNode& RuleNode, _ruleNodes)
        this->TargetRules.append(RuleNode.targetRules());

    this->UsableTargetRuleCount = qMin(
                (int)clsPhraseCandidateCollectionData::MaxTargetPhraseCount.value(),
                this->TargetRules.size()
                );

    std::nth_element(
                TargetRules.begin(),
                TargetRules.begin() + UsableTargetRuleCount,
                TargetRules.end(),
                [&] (const clsTargetRule& _first, const clsTargetRule& _second) {
                    return _first.precomputedValue(clsSearchGraph::ruleTablePrecomputedValueIndex()) <
                            _second.precomputedValue(clsSearchGraph::ruleTablePrecomputedValueIndex());
                }
    );

    // Prune the unnecessary rules
    if(UsableTargetRuleCount < TargetRules.size())
        TargetRules.erase(
                TargetRules.begin() + UsableTargetRuleCount,
                TargetRules.end()
                );

    this->BestApproximateCost = INFINITY;
    // _observationHistogramSize must be taken care of to not exceed this->TargetRules.size()
    for(int Count = 0; Count < this->UsableTargetRuleCount; ++Count) {
        clsTargetRule& TargetRule = this->TargetRules[Count];
        // Compute the approximate cost for current target rule
        Cost_t ApproximateCost = 0;
        foreach (FeatureFunction::intfFeatureFunction* FF , gConfigs.ActiveFeatureFunctions)
            if(FF->canComputePositionSpecificRestCost() == false) {
                Cost_t Cost = FF->getApproximateCost(_beginPos, _endPos, _input, TargetRule);
                ApproximateCost += Cost;
            }
        this->BestApproximateCost = qMin(this->BestApproximateCost, ApproximateCost);
    }
}

QString clsPhraseCandidateCollectionData::moduleName()
{
    return clsSearchGraph::moduleName();
}

}
}
}
}




/*************************************************
* TODO PREMATURE OPTIMIZATION that does not work properly
*                 size_t StartLookingPos = 0;
size_t NeededSpace = NewPhraseCardinality;
while(StartLookingPos <= (size_t)this->Data->Sentence.size() - NewPhraseCardinality){
    size_t LookingPos = StartLookingPos;
    while (LookingPos < PrevCoverage.size()) {
        if (PrevCoverage.testBit(LookingPos)){
            NeededSpace = NewPhraseCardinality;
        }else{
            --NeededSpace;
            if(NeededSpace == 0)
                break;
        }
        ++LookingPos;
    }

    if (NeededSpace > 0)
        break;

    NeededSpace = 1;
    StartLookingPos = LookingPos+1;
    size_t NewPhraseBeginPos = LookingPos - NewPhraseCardinality + 1;
    size_t NewPhraseEndPos = NewPhraseBeginPos + NewPhraseCardinality;
*************************************************/
