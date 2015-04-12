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

#include <functional>

#include "clsSearchGraphBuilder.h"
#include "../GlobalConfigs.h"
#include "Private/Proxies/intfLMSentenceScorer.hpp"
#include "Private/OOVHandler/OOVHandler.h"
#include <iostream>

#define PBT_MAXIMUM_COST 1e200

namespace Targoman{
namespace SMT {
namespace Private{
namespace SearchGraph {

using namespace Common;
using namespace Common::Configuration;
using namespace RuleTable;
using namespace Proxies;
using namespace InputDecomposer;
using namespace OOV;

tmplConfigurable<quint8> clsSearchGraphBuilder::HardReorderingJumpLimit(
        clsSearchGraphBuilder::moduleBaseconfig() + "/HardReorderingJumpLimit",
        "TODO Desc",
        6,
        [] (const intfConfigurable& _item, QString&) {
    clsCardinalityHypothesisContainer::setHardReorderingJumpLimit(
            _item.toVariant().Int
            );
    return true;
});

tmplConfigurable<quint8> clsSearchGraphBuilder::ReorderingConstraintMaximumRuns(
        clsSearchGraphBuilder::moduleBaseconfig() + "/ReorderingConstraintMaximumRuns",
        "IBM1 reordering constraint",
        2);
tmplConfigurable<bool>   clsSearchGraphBuilder::DoComputePositionSpecificRestCosts(
        clsSearchGraphBuilder::moduleBaseconfig() + "/DoComputePositionSpecificRestCosts",
        "TODO Desc",
        true);
tmplConfigurable<quint8> clsPhraseCandidateCollectionData::MaxTargetPhraseCount(
        clsSearchGraphBuilder::moduleBaseconfig() + "/MaxTargetPhraseCount",
        "TODO Desc",
        100);

tmplConfigurable<bool>   clsSearchGraphBuilder::DoPrunePreInsertion(
        clsSearchGraphBuilder::moduleBaseconfig() + "/PrunePreInsertion",
        "TODO Desc",
        true);

FeatureFunction::intfFeatureFunction*  clsSearchGraphBuilder::pPhraseTable = NULL;
RuleTable::intfRuleTable*              clsSearchGraphBuilder::pRuleTable = NULL;
RuleTable::clsRuleNode*                clsSearchGraphBuilder::UnknownWordRuleNode;

/**********************************************************************************/
clsSearchGraphBuilder::clsSearchGraphBuilder(const Sentence_t& _sentence):
    Data(new clsSearchGraphBuilderData(_sentence))
{}
/**
 * @brief Loads rule and phrase tables, inititializes all feature functions and sets #UnknownWordRuleNode.
 * @param _configFilePath Address of config file.
 */
void clsSearchGraphBuilder::init(const QString& _configFilePath)
{
    clsSearchGraphBuilder::pRuleTable = gConfigs.RuleTable.getInstance<intfRuleTable>();

    clsSearchGraphBuilder::pRuleTable->initializeSchema();

    //InvalidTargetRuleData has been marshalled here because it depends on loading RuleTable
    RuleTable::InvalidTargetRuleData = new RuleTable::clsTargetRuleData;

    //pInvalidTargetRule has been marshalled here because it depends on instantiation of InvalidTargetRuleData
    RuleTable::pInvalidTargetRule = new RuleTable::clsTargetRule;
    //InvalidSearchGraphNodeData has been marshalled here because it depends on initialization of gConfigs
    InvalidSearchGraphNodeData = new clsSearchGraphNodeData;
    pInvalidSearchGraphNode = new clsSearchGraphNode;

    foreach (FeatureFunction::intfFeatureFunction* FF, gConfigs.ActiveFeatureFunctions)
        FF->initialize(_configFilePath);
    clsSearchGraphBuilder::pRuleTable->loadTableData();
    clsSearchGraphBuilder::pPhraseTable = gConfigs.ActiveFeatureFunctions.value("PhraseTable");

    RulesPrefixTree_t::Node_t* Node = &clsSearchGraphBuilder::pRuleTable->getPrefixTree().rootNode();
    if(Node->isInvalid())
        throw exSearchGraph("Invalid empty Rule Table");

    Node = &Node->follow(gConfigs.EmptyLMScorer->unknownWordIndex()); //Search for UNKNOWN word Index
    if (Node->isInvalid())
        throw exSearchGraph("No Rule defined for UNKNOWN word");

    clsSearchGraphBuilder::UnknownWordRuleNode = new clsRuleNode(Node->getData());
}

/**
 * @brief Looks up prefix tree for all phrases that matches with some parts of input sentence and stores them in the
 * PhraseCandidateCollections of #Data. This function also calculates maximum length of matching source phrase with phrase table.
 */

void clsSearchGraphBuilder::collectPhraseCandidates()
{
    this->Data->MaxMatchingSourcePhraseCardinality = 0;
    for (size_t FirstPosition = 0; FirstPosition < (size_t)this->Data->Sentence.size(); ++FirstPosition) {
        this->Data->PhraseCandidateCollections.append(QVector<clsPhraseCandidateCollection>(this->Data->Sentence.size() - FirstPosition));
        RulesPrefixTree_t::Node_t* PrevNode = &this->pRuleTable->getPrefixTree().rootNode();

        if(true /* On 1-grams */)
        {
            WordIndex_t WordIndex = this->Data->Sentence.at(FirstPosition).wordIndex();
            PrevNode = &PrevNode->follow(WordIndex);
            if (PrevNode->isInvalid() || WordIndex == gConfigs.EmptyLMScorer->unknownWordIndex()) {
                clsRuleNode OOVRuleNode =
                        OOVHandler::instance().getRuleNode(this->Data->Sentence.at(FirstPosition).wordIndex());
                if (OOVRuleNode.isInvalid())
                    this->Data->PhraseCandidateCollections[FirstPosition][0] = clsPhraseCandidateCollection(FirstPosition, FirstPosition + 1, *clsSearchGraphBuilder::UnknownWordRuleNode);
                else
                    this->Data->PhraseCandidateCollections[FirstPosition][0] = clsPhraseCandidateCollection(FirstPosition, FirstPosition + 1, OOVRuleNode);
            }else
                this->Data->PhraseCandidateCollections[FirstPosition][0] = clsPhraseCandidateCollection(FirstPosition, FirstPosition + 1, PrevNode->getData());

            if (this->Data->PhraseCandidateCollections[FirstPosition][0].isInvalid() == false)
                this->Data->MaxMatchingSourcePhraseCardinality = qMax(this->Data->MaxMatchingSourcePhraseCardinality, 1);

            if (PrevNode->isInvalid())
                continue;
        }

        //Max PhraseTable order will be implicitly checked by follow
        for (size_t LastPosition = FirstPosition + 1; LastPosition < (size_t)this->Data->Sentence.size() ; ++LastPosition){
            PrevNode = &PrevNode->follow(this->Data->Sentence.at(LastPosition).wordIndex());

            if (PrevNode->isInvalid())
                break; // appending next word breaks phrase lookup

            this->Data->PhraseCandidateCollections[FirstPosition][LastPosition - FirstPosition] = clsPhraseCandidateCollection(FirstPosition, LastPosition + 1, PrevNode->getData());
            if (this->Data->PhraseCandidateCollections[FirstPosition][LastPosition - FirstPosition].isInvalid() == false)
                this->Data->MaxMatchingSourcePhraseCardinality = qMax(this->Data->MaxMatchingSourcePhraseCardinality,
                                                                      (int)(LastPosition - FirstPosition + 1));
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
bool clsSearchGraphBuilder::conformsIBM1Constraint(const Coverage_t& _newCoverage)
{
    //Find last bit set then check how many bits are zero before this.
    for(int i=_newCoverage.size() - 1; i>=0; --i)
        if(_newCoverage.testBit(i)){
            size_t CoutOfPrevZeros = _newCoverage.count(false) + i - _newCoverage.size() + 1;
            return (CoutOfPrevZeros <= this->ReorderingConstraintMaximumRuns.value());
        }
    return true;
}

bool clsSearchGraphBuilder::conformsHardReorderingJumpLimit(const Coverage_t &_coverage,
                                                            size_t _endPos)
{
    int FirstEmptyPosition = _coverage.size();
    for(int i = 0; i < _coverage.size(); ++i)
        if(_coverage.testBit(i) == false) {
            FirstEmptyPosition = i;
            break;
        }
    if(FirstEmptyPosition == _coverage.size())
        return true;
    size_t JumpWidth = qAbs((int)_endPos - FirstEmptyPosition);
    return JumpWidth <= clsSearchGraphBuilder::HardReorderingJumpLimit.value();
}


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
bool clsSearchGraphBuilder::decode()
{
    this->Data->HypothesisHolder.clear();
    this->Data->HypothesisHolder.resize(this->Data->Sentence.size() + 1);

    this->initializeRestCostsMatrix();


    for (int NewCardinality = 1; NewCardinality <= this->Data->Sentence.size(); ++NewCardinality){

        //int PrunedByIBMConstraint = 0;
        int PrunedByHardReorderingJumpLimit = 0;
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
                    //TODO: We must have this warning log in release mode also
#ifdef TARGOMAN_SHOW_DEBUG
                    TargomanLogWarn(1, "PrevLexHypoContainer is empty. PrevCard: " << PrevCardinality
                                  << "PrevCov: " << PrevCoverage);
#endif
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



                    // Skip these candidates if they will cause reordering jump violation in future
                    int firstUncoveredPosition = PrevCoverage.size();
                    for(int i = 0; i < PrevCoverage.size(); ++i) {
                        if(PrevCoverage.testBit(i) == false) {
                            firstUncoveredPosition = i;
                            break;
                        }
                    }
                    if(firstUncoveredPosition < PrevCoverage.size()) {
                        if(qAbs((int)NewPhraseEndPos - firstUncoveredPosition) >
                                HardReorderingJumpLimit.value())
                            continue;
                    }

                    Coverage_t NewCoverage(PrevCoverage);
                    for (size_t i=NewPhraseBeginPos; i<NewPhraseEndPos; ++i)
                        NewCoverage.setBit(i);

                    /*
                    if (this->conformsIBM1Constraint(NewCoverage) == false){
                        ++PrunedByIBMConstraint;
                        continue;
                    }
                    */

                    if (this->conformsHardReorderingJumpLimit(NewCoverage, NewPhraseEndPos) == false){
                        ++PrunedByHardReorderingJumpLimit;
                        continue;
                    }

                    clsPhraseCandidateCollection& PhraseCandidates =
                            this->Data->PhraseCandidateCollections[NewPhraseBeginPos][NewPhraseCardinality - 1];

                    //There is no rule defined in rule table for current phrase
                    if (PhraseCandidates.isInvalid())
                        continue; //TODO If there are no more places to fill after this startpos break

                    Cost_t RestCost =  this->calculateRestCost(NewCoverage, NewPhraseBeginPos, NewPhraseEndPos);

                    foreach (const clsSearchGraphNode& PrevLexHypoNode, PrevLexHypoContainer.nodes()) {

                        size_t MaxCandidates = qMin((int)PhraseCandidates.usableTargetRuleCount(),
                                                    PhraseCandidates.targetRules().size());
                        for(size_t i = 0; i<MaxCandidates; ++i){

                            const clsTargetRule& CurrentPhraseCandidate = PhraseCandidates.targetRules().at(i);

                            clsSearchGraphNode NewHypoNode(PrevLexHypoNode,
                                                           NewPhraseBeginPos,
                                                           NewPhraseEndPos,
                                                           NewCoverage,
                                                           CurrentPhraseCandidate,
                                                           IsFinal,
                                                           RestCost);

                            // If current NewHypoNode is worse than worst stored node ignore it
                            if (clsSearchGraphBuilder::DoPrunePreInsertion.value() &&
                                CurrCardHypoContainer.mustBePruned(NewHypoNode.getTotalCost())){
                                ++PrunedPreInsertion;
                                continue;
                            }



                            if(CurrCardHypoContainer.insertNewHypothesis(NewHypoNode)) {
                                // Log insertion of hypothesis here if it is needed
                            }
                        }

                    }//foreach PrevLexHypoNode
                }//for NewPhraseBeginPos
            }//for PrevCoverageIter
        }//for PrevCardinality
        CurrCardHypoContainer.finlizePruningAndcleanUp();

#ifdef TARGOMAN_SHOW_DEBUG
        // Vedadian
        //*

        if(true) {
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
            for(auto Iterator = CurrCardHypoContainer.lexicalHypotheses().end() - 1;
                true;
                --Iterator) {
                const QList<clsSearchGraphNode>& Nodes = Iterator->nodes();
                foreach(const clsSearchGraphNode& SelectedNode, Nodes) {
                    std::cout << (float)SelectedNode.getTotalCost() << "\t";
                    std::cout << "Cardinality:  ";
                    std::cout << car2str(NewCardinality).toUtf8().constData();
                    std::cout << "  Coverage:  " << cov2str(SelectedNode.coverage()).toUtf8().constData();
                    std::cout << "  Cost:  " << (float)(SelectedNode.getCost())
                              << " , RestCost: " << (float)(SelectedNode.getTotalCost() - SelectedNode.getCost())
                              << " , Str: (" << SelectedNode.prevNode().targetRule().toStr().toUtf8().constData()
                              << ")" << SelectedNode.targetRule().toStr().toUtf8().constData() << std::endl;
                }
                if(Iterator == CurrCardHypoContainer.lexicalHypotheses().begin())
                    break;
            }
            std::cout << std::endl << std::endl << std::endl;
        }
        //*/
#endif
    }//for NewCardinality

    Coverage_t FullCoverage;
    FullCoverage.fill(1, this->Data->Sentence.size());

    if(this->Data->HypothesisHolder[this->Data->Sentence.size()].lexicalHypotheses().size() > 0 &&
            this->Data->HypothesisHolder[this->Data->Sentence.size()][FullCoverage].nodes().isEmpty() == false)
    {
        this->Data->GoalNode = &this->Data->HypothesisHolder[this->Data->Sentence.size()][FullCoverage].nodes().first();
        this->Data->HypothesisHolder[this->Data->Sentence.size()][FullCoverage].finalizeRecombination();
        return true;
    } else {
        static clsSearchGraphNode InvalidGoalNode;
        this->Data->GoalNode = &InvalidGoalNode;
        // TODO: We need to have this log in release mode also
#ifdef TARGOMAN_SHOW_DEBUG
        TargomanLogWarn(1, "No translation option for: " << this->Data->Sentence);
#endif
        return false;
    }
}

/**
 * @brief Initializes rest cost matrix
 *
 * For every possible range of words of input sentence, finds approximate cost of every feature fucntions, then
 * tries to reduce that computed rest cost if sum of rest cost of splited phrase is less than whole phrase rest cost.
 */
void clsSearchGraphBuilder::initializeRestCostsMatrix()
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
            // Torabzadeh
            std::cerr << FirstPosition << ":" << FirstPosition + Length -1 << " "
                      << ((int(1000 * this->Data->RestCostMatrix[FirstPosition][Length - 1] + 0.5))/1000.0)
                      << std::endl;
        }
    }
    exit(0);

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

Cost_t clsSearchGraphBuilder::calculateRestCost(const Coverage_t& _coverage, size_t _beginPos, size_t _endPos) const
{
    Cost_t RestCosts = 0.0;
    size_t StartPosition = 0;
    size_t Length = 0;

    for(size_t i=0; i < (size_t)_coverage.size(); ++i)
        if(_coverage.testBit(i) == false){
            if(Length == 0)
                StartPosition = i;
            ++Length;
        }else if(Length){
            RestCosts += this->Data->RestCostMatrix[StartPosition][Length-1];
            Length = 0;
        }
    if(Length)
        RestCosts += this->Data->RestCostMatrix[StartPosition][Length-1];

    if(clsSearchGraphBuilder::DoComputePositionSpecificRestCosts.value()) {
        foreach(FeatureFunction::intfFeatureFunction* FF, gConfigs.ActiveFeatureFunctions.values()) {
            if(FF->canComputePositionSpecificRestCost())
                RestCosts += FF->getRestCostForPosition(_coverage, _beginPos, _endPos);
        }
    }
    return RestCosts;
}

clsPhraseCandidateCollectionData::clsPhraseCandidateCollectionData(size_t _beginPos, size_t _endPos, const clsRuleNode &_ruleNode)
{
    this->TargetRules = _ruleNode.targetRules();
    this->UsableTargetRuleCount = qMin(
                (int)clsPhraseCandidateCollectionData::MaxTargetPhraseCount.value(),
                this->TargetRules.size()
                );
    // TODO: Maybe sorting can be added here
    this->BestApproximateCost = INFINITY;
    // _observationHistogramSize must be taken care of to not exceed this->TargetRules.size()
    for(int Count = 0; Count < this->UsableTargetRuleCount; ++Count) {
        clsTargetRule& TargetRule = this->TargetRules[Count];
        // Compute the approximate cost for current target rule
        Cost_t ApproximateCost = 0;
        foreach (FeatureFunction::intfFeatureFunction* FF , gConfigs.ActiveFeatureFunctions)
            if(FF->canComputePositionSpecificRestCost() == false) {
                Cost_t Cost = FF->getApproximateCost(_beginPos, _endPos, TargetRule);
                ApproximateCost += Cost;
            }
        this->BestApproximateCost = qMin(this->BestApproximateCost, ApproximateCost);
    }
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
