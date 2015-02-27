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

#define PBT_MAXIMUM_COST 1e200

namespace Targoman{
namespace Core {
namespace Private{
namespace SearchGraphBuilder {

using namespace Common;
using namespace Common::Configuration;
using namespace RuleTable;
using namespace Proxies;
using namespace InputDecomposer;
using namespace OOV;

tmplConfigurable<quint8> clsSearchGraphBuilder::ReorderingConstraintMaximumRuns(
        clsSearchGraphBuilder::moduleBaseconfig() + "/ReorderingConstraintMaximumRuns",
        "IBM1 reordering constraint",
        2);
tmplConfigurable<bool>   clsSearchGraphBuilder::DoComputePositionSpecificRestCosts(
        clsSearchGraphBuilder::moduleBaseconfig() + "/DoComputeReorderingRestCosts",
        "TODO Desc",
        true);
tmplConfigurable<bool>   clsSearchGraphBuilder::PruneAtStage2(
        clsSearchGraphBuilder::moduleBaseconfig() + "/PruneAtStage2",
        "TODO Desc",
        true);
tmplConfigurable<bool>   clsSearchGraphBuilder::PruneAtStage3(
        clsSearchGraphBuilder::moduleBaseconfig() + "/PruneAtStage3",
        "TODO Desc",
        true);
tmplConfigurable<bool>   clsSearchGraphBuilder::PruneAtStage4(
        clsSearchGraphBuilder::moduleBaseconfig() + "/PruneAtStage4",
        "TODO Desc",
        true);
tmplConfigurable<quint8> clsSearchGraphBuilder::ObservationHistogramSize(
        clsSearchGraphBuilder::moduleBaseconfig() + "/ObservationHistogramSize",
        "TODO Desc",
        100);

FeatureFunction::intfFeatureFunction*  clsSearchGraphBuilder::pPhraseTable = NULL;
RuleTable::intfRuleTable*              clsSearchGraphBuilder::pRuleTable = NULL;
RuleTable::clsRuleNode*                clsSearchGraphBuilder::UnknownWordRuleNode;

/**********************************************************************************/
clsSearchGraphBuilder::clsSearchGraphBuilder(const Sentence_t& _sentence):
    Data(new clsSearchGraphBuilderData(_sentence))
{}

void clsSearchGraphBuilder::init(const QString& _configFilePath)
{
    clsSearchGraphBuilder::pRuleTable = gConfigs.RuleTable.getInstance<intfRuleTable>();
    clsSearchGraphBuilder::pRuleTable->initializeSchema();
    foreach (FeatureFunction::intfFeatureFunction* FF, gConfigs.ActiveFeatureFunctions)
        FF->initialize(_configFilePath);
    clsSearchGraphBuilder::pRuleTable->loadTableData();
    clsSearchGraphBuilder::pPhraseTable = gConfigs.ActiveFeatureFunctions.value("PhraseTable");

    RulesPrefixTree_t::Node* Node = clsSearchGraphBuilder::pRuleTable->getPrefixTree().rootNode();
    if(Node == NULL)
        throw exSearchGraphBuilder("Invalid empty Rule Table");

    Node = Node->follow(0); //Search for UNKNOWN word Index
    if (Node == NULL)
        throw exSearchGraphBuilder("No Rule defined for UNKNOWN word");

    clsSearchGraphBuilder::UnknownWordRuleNode = new clsRuleNode(Node->getData());

    //InvalidTargetRule has been marshalled here because it depends on loading RuleTable
    RuleTable::InvalidTargetRule = new RuleTable::clsTargetRule;

    //InvalidSearchGraphNodeData has been marshalled here because it depends on initialization of gConfigs
    InvalidSearchGraphNodeData = new clsSearchGraphNodeData;

    pInvalidSearchGraphNode = new clsSearchGraphNode;
}

void clsSearchGraphBuilder::matchPhrase()
{
    this->Data->MaxMatchingSourcePhraseCardinality = 0;
    for (size_t FirstPosition = 0; FirstPosition < (size_t)this->Data->Sentence.size(); ++FirstPosition) {
        this->Data->PhraseMatchTable.append(QVector<clsRuleNode>(this->Data->Sentence.size() - FirstPosition));
        RulesPrefixTree_t::Node* PrevNode = this->pRuleTable->getPrefixTree().rootNode();

        { //On Uni-Grams
            PrevNode = PrevNode->follow(this->Data->Sentence.at(FirstPosition).wordIndex());

            if (PrevNode == NULL){
                clsRuleNode OOVRuleNode =
                        OOVHandler::instance().getRuleNode(this->Data->Sentence.at(FirstPosition).wordIndex());
                if (OOVRuleNode.isInvalid())
                    this->Data->PhraseMatchTable[FirstPosition][0] = *clsSearchGraphBuilder::UnknownWordRuleNode;
                else
                    this->Data->PhraseMatchTable[FirstPosition][0] = OOVRuleNode;
            }else
                this->Data->PhraseMatchTable[FirstPosition][0] = PrevNode->getData();

            if (this->Data->PhraseMatchTable[FirstPosition][0].isInvalid() == false)
                this->Data->MaxMatchingSourcePhraseCardinality = qMax(this->Data->MaxMatchingSourcePhraseCardinality,1);

            if (PrevNode == NULL)
                continue;
        }

        //Max PhraseTable order will be implicitly checked by follow
        for (size_t LastPosition = FirstPosition + 1; LastPosition < (size_t)this->Data->Sentence.size() ; ++LastPosition){
            PrevNode = PrevNode->follow(this->Data->Sentence.at(LastPosition).wordIndex());

            if (PrevNode == NULL)
                break; // appending next word breaks phrase lookup

            this->Data->PhraseMatchTable[FirstPosition][LastPosition - FirstPosition] = PrevNode->getData();
            if (this->Data->PhraseMatchTable[FirstPosition][LastPosition - FirstPosition].isInvalid() == false)
                this->Data->MaxMatchingSourcePhraseCardinality = qMax(this->Data->MaxMatchingSourcePhraseCardinality,
                                                                      (int)(LastPosition - FirstPosition + 1));
        }
    }
}

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

bool clsSearchGraphBuilder::parseSentence()
{
    this->Data->HypothesisHolder.clear();
    this->Data->HypothesisHolder.resize(this->Data->Sentence.size() + 1);

    this->initializeRestCostsMatrix();


    for (int NewCardinality = 1; NewCardinality <= this->Data->Sentence.size(); ++NewCardinality){

        int PrunedByIBMConstraint;
        int PrunedByLexicalHypothesis;

        bool IsFinal = (NewCardinality == this->Data->Sentence.size());
        int MinPrevCardinality = qMax(NewCardinality - this->Data->MaxMatchingSourcePhraseCardinality, 0);

        for (int PrevCardinality = MinPrevCardinality;
             PrevCardinality < NewCardinality; ++PrevCardinality) {

            unsigned short NewPhraseCardinality = NewCardinality - PrevCardinality;

            //This happens when we have for ex. 2 bi-grams and a quad-gram but no similar 3-gram. due to bad training
            if(this->Data->HypothesisHolder[PrevCardinality].isEmpty()) {

                //std::cout<<__LINE__<< ": ERROR: cardinality Container for previous cardinality empty.\n";
                continue;
            }

            for(LexicalHypothesisContainer_t::Iterator PrevCoverageIter = this->Data->HypothesisHolder[PrevCardinality].lexicalHypotheses().begin();
                PrevCoverageIter != this->Data->HypothesisHolder[PrevCardinality].lexicalHypotheses().end();
                ++PrevCoverageIter){


                const Coverage_t& PrevCoverage = PrevCoverageIter.key();
                clsLexicalHypothesis& PrevLexHypoContainer = PrevCoverageIter.value();

                Q_ASSERT(PrevCoverage.count(true) == PrevCardinality);
                Q_ASSERT(PrevLexHypoContainer.nodes().size());


                // TODO: this can be removed if pruning works properly
                if (PrevLexHypoContainer.nodes().isEmpty()){
                    //TODO convert to log
                    TargomanDebug(1,"Warning: prevLexHypContainer empty. PrevCard: " <<PrevCardinality
                                  << "PrevCov: " << bitArray2Str(PrevCoverage)
                                  <<" Addr:" <<(void*)PrevLexHypoContainer.Data.data());


                    continue;
                }

                /*
                 * TODO at this point pbt performs cardinality pruning (cf. discardedAtA / cardhist, cardthres pruning)
                 * this is not implemented here.
                 */
                for (size_t NewPhraseBeginPos = 0;
                     NewPhraseBeginPos <= (size_t)this->Data->Sentence.size() - NewPhraseCardinality;
                     ++NewPhraseBeginPos){
                    size_t NewPhraseEndPos = NewPhraseBeginPos + NewPhraseCardinality;
                    //TargomanDebug(1, "start position: "<< StartPos);

                    // skip if phrase coverage is not compatible with previous sentence coverage
                    bool SkipStep = false;
                    for (size_t i= NewPhraseBeginPos; i<NewPhraseEndPos; ++i)
                        if (PrevCoverage.testBit(i)){
                            SkipStep = true;
                            break;
                        }
                    if (SkipStep)
                        continue;//TODO if NewPhraseCardinality has not continous place breaK

                    Coverage_t NewCoverage(PrevCoverage);
                    for (size_t i=NewPhraseBeginPos; i<NewPhraseEndPos; ++i)
                        NewCoverage.setBit(i);

                    if (this->conformsIBM1Constraint(NewCoverage) == false){
                        ++PrunedByIBMConstraint;
                        continue;
                    }

                    clsRuleNode& PhraseCandidates =
                            this->Data->PhraseMatchTable[NewPhraseBeginPos][NewPhraseCardinality - 1];

                    //There is no rule defined in rule table for current phrase
                    if (PhraseCandidates.isInvalid())
                        continue; //TODO If there are no more places to fill after this startpos break

                    clsLexicalHypothesis& NewLexHypoContainer =
                            this->Data->HypothesisHolder[NewCardinality][NewCoverage];

                    Cost_t RestCost =  this->calculateRestCost(NewCoverage, NewPhraseBeginPos, NewPhraseEndPos);

                    foreach (const clsSearchGraphNode& PrevLexHypoNode, PrevLexHypoContainer.nodes()) {

                        size_t MaxCandidates = qMin((int)this->ObservationHistogramSize.value(),
                                                    PhraseCandidates.targetRules().size());
                        bool MustBreak = false;

                        for(size_t i = 0; i<MaxCandidates; ++i){

                            const clsTargetRule& CurrentPhraseCandidate = PhraseCandidates.targetRules().at(i);

                            clsSearchGraphNode NewHypoNode(PrevLexHypoNode,
                                                           NewPhraseBeginPos,
                                                           NewPhraseEndPos,
                                                           NewCoverage,
                                                           CurrentPhraseCandidate,
                                                           IsFinal,
                                                           RestCost);

                            TargomanDebug(7,
                                          "\nNewHypoNode: " <<
                                          "Cardinality: [" << NewCardinality << "] " <<
                                          "Coverage[" << NewCoverage << "] " <<
                                          "Cost: [" << NewHypoNode.getCost() << "] " <<
                                          "RestCost: [" << RestCost << "] " <<
                                          "TargetPhrase: [" << CurrentPhraseCandidate.toStr() << "]\n" <<
                                          "\t\t\tScores: " << NewHypoNode.costElements()
                                          )


                            // If current NewHypoNode is worse than worst stored node ignore it
                            if (this->PruneAtStage4.value() &&
                                NewLexHypoContainer.mustBePruned(NewHypoNode.getTotalCost())){
                                ++PrunedByLexicalHypothesis;
                                MustBreak = true;
                                break;
                            }

                            if(this->Data->HypothesisHolder[NewCardinality].insertNewHypothesis(NewCoverage,
                                                                                                NewLexHypoContainer,
                                                                                                NewHypoNode)) {
                                TargomanDebug(7, "\nNew Hypothesis Inserted.")
                            }
                        }

                        // In case the currently created node is not worth putting in the stack,
                        // its inferiors are definitely are not worth also
                        if(MustBreak)
                                break;

                    }//foreach PrevLexHypoNode
                    if (NewLexHypoContainer.nodes().isEmpty())
                        this->Data->HypothesisHolder[NewCardinality].remove(NewCoverage);

                }//for NewPhraseBeginPos
            }//for PrevCoverageIter
        }//for PrevCardinality
        // Vedadian
        if(NewCardinality == 1)
            exit(0);
    }//for NewCardinality

    Coverage_t FullCoverage;
    FullCoverage.fill(1, this->Data->Sentence.size());

    if(this->Data->HypothesisHolder[this->Data->Sentence.size()].lexicalHypotheses().size() > 0 &&
            this->Data->HypothesisHolder[this->Data->Sentence.size()][FullCoverage].nodes().isEmpty() == false)
    {
        this->Data->GoalNode = &this->Data->HypothesisHolder[this->Data->Sentence.size()][FullCoverage].bestNode();
        this->Data->HypothesisHolder[this->Data->Sentence.size()][FullCoverage].finalizeRecombination();
        return true;
    } else {
        TargomanLogWarn(1, "No translation option for: " << this->Data->Sentence);
        return false;
    }
}

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
            clsRuleNode& PhraseCandidates = this->Data->PhraseMatchTable[FirstPosition][Length-1];
            if (PhraseCandidates.isInvalid() || PhraseCandidates.targetRules().isEmpty())
                continue;

            Cost_t BestCost = PBT_MAXIMUM_COST;

            for (size_t PhraseCandidateNumber= 0;
                 PhraseCandidateNumber < (size_t)PhraseCandidates.targetRules().size() &&
                 PhraseCandidateNumber < clsSearchGraphBuilder::ObservationHistogramSize.value();
                 ++PhraseCandidateNumber ){

                Cost_t Cost = 0;
                foreach (FeatureFunction::intfFeatureFunction* FF , gConfigs.ActiveFeatureFunctions)
                    if(FF->canComputePositionSpecificRestCost() == false)
                        Cost += FF->getApproximateCost(
                                    FirstPosition,
                                    FirstPosition + Length,
                                    PhraseCandidates.targetRules().at(PhraseCandidateNumber));

                BestCost = qMin(BestCost, Cost);
            }
            this->Data->RestCostMatrix[FirstPosition][Length - 1]  = BestCost;
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
