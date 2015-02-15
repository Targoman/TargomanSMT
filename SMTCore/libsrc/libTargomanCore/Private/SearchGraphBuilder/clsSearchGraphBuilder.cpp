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

#include "clsSearchGraphBuilder.h"
#include "../GlobalConfigs.h"
#include "Private/LanguageModel/intfLMSentenceScorer.hpp"

namespace Targoman{
namespace Core {
namespace Private{
namespace SearchGraphBuilder {

using namespace Common;
using namespace Common::Configuration;
using namespace RuleTable;
using namespace LanguageModel;
using namespace InputDecomposer;

tmplConfigurable<quint8> clsSearchGraphBuilder::ReorderingConstraintMaximumRuns(
        "TODO",
        "TODO");
tmplConfigurable<bool>   clsSearchGraphBuilder::DoComputeReorderingRestCosts(
        "TODO",
        "TODO");
tmplConfigurable<bool>   clsSearchGraphBuilder::PruneAtStage2(
        "TODO",
        "TODO");
tmplConfigurable<bool>   clsSearchGraphBuilder::PruneAtStage3(
        "TODO",
        "TODO");
tmplConfigurable<bool>   clsSearchGraphBuilder::PruneAtStage4(
        "TODO",
        "TODO");

tmplConfigurable<bool>   clsSearchGraphBuilder::ReorderingHardJumpLimit(
        "TODO",
        "TODO");
tmplConfigurable<quint8> clsSearchGraphBuilder::ReorderingMaximumJumpWidth(
        "TODO",
        "TODO");
tmplConfigurable<quint8> clsSearchGraphBuilder::ObservationHistogramSize(
        "TODO",
        "TODO");

tmplConfigurable<double> clsSearchGraphBuilder::ScalingFactorReorderingJump(
        "TODO",
        "TODO");

FeatureFunction::intfFeatureFunction*  clsSearchGraphBuilder::pPhraseTable = NULL;
RuleTable::intfRuleTable*              clsSearchGraphBuilder::pRuleTable = NULL;

/**********************************************************************************/
clsSearchGraphBuilder::clsSearchGraphBuilder(const Sentence_t& _sentence):
    Data(new clsSearchGraphBuilderData(_sentence))
{}

void clsSearchGraphBuilder::init()
{
    clsSearchGraphBuilder::pRuleTable = gConfigs.RuleTable.getInstance<intfRuleTable>();
    clsSearchGraphBuilder::pRuleTable->init();
    clsSearchGraphBuilder::pPhraseTable = gConfigs.ActiveFeatureFunctions.value("PhraseTable");
}

void clsSearchGraphBuilder::matchPhrase()
{
    for (size_t FirstPosition = 0; FirstPosition < (size_t)this->Data->Sentence.size(); ++FirstPosition) {
        this->Data->PhraseMatchTable.append(QVector<clsRuleNode>(this->Data->Sentence.size() - FirstPosition));
        RulesPrefixTree_t::Node* PrevNode = this->pRuleTable->getPrefixTree().rootNode();
        //Max PhraseTable order will be implicitly checked by follow
        for (size_t LastPosition = FirstPosition; LastPosition < (size_t)this->Data->Sentence.size() ; ++LastPosition){
            PrevNode = PrevNode->follow(this->Data->Sentence.at(LastPosition).wordIndex());
            if (PrevNode == NULL)
                break; // appending next word breaks phrase lookup

            //TODO WARNING !!!!!!!!!!!!! referencecounter may be buggy
            this->Data->PhraseMatchTable[FirstPosition][LastPosition - FirstPosition] = PrevNode->getData();
            if (this->Data->PhraseMatchTable[FirstPosition][LastPosition - FirstPosition].isInvalid() == false)
                this->Data->MaxMatchingSourcePhraseCardinality = qMax(this->Data->MaxMatchingSourcePhraseCardinality,
                                                                (int)(LastPosition - FirstPosition));
        }
    }
}

Cost_t clsSearchGraphBuilder::computeReorderingJumpCost(size_t JumpWidth) const
{
    Cost_t ReorderingJumpCosts = JumpWidth;

    if (JumpWidth > this->ReorderingMaximumJumpWidth.value())
        ReorderingJumpCosts = JumpWidth + (JumpWidth * JumpWidth);

    return ReorderingJumpCosts;
}

bool clsSearchGraphBuilder::parseSentence()
{
    //JANE::startParsing()
    this->Data->HypothesisHolder.clear();
    this->Data->HypothesisHolder.resize(this->Data->Sentence.size() + 1);

    this->Data->RootNode = this->Data->HypothesisHolder.getRootNode();

    this->initializeRestCostsMatrix();

    int PrunedAt2 = 0;
    int PrunedAt3 = 0;
    int PrunedAt4 = 0;

    for (int NewCardinality = 1; NewCardinality <= this->Data->Sentence.size(); ++NewCardinality){
        bool IsFinal = NewCardinality == this->Data->Sentence.size();

        for (int PrevCardinality = qMax(NewCardinality - this->Data->MaxMatchingSourcePhraseCardinality, 0);
             PrevCardinality < NewCardinality; ++PrevCardinality) {
            unsigned short NewPhraseCardinality = NewCardinality - PrevCardinality;
            // TODO: check if we have phrases of this cardinality. if not, continue.

            if(this->Data->HypothesisHolder[PrevCardinality].isEmpty()) {
                //log() << "ERROR: cardinality Container for previous cardinality empty.\n";
                continue;
            }

            for(LexicalHypothesisContainer_t::Iterator PrevCoverageIter = this->Data->HypothesisHolder[PrevCardinality].lexicalHypotheses().begin();
                PrevCoverageIter != this->Data->HypothesisHolder[PrevCardinality].lexicalHypotheses().end();
                ++PrevCoverageIter){
                const Coverage_t& PrevCoverage = PrevCoverageIter.key();
                clsLexicalHypothesis& PrevLexHypoContainer = PrevCoverageIter.value();

                // TODO: this can be removed if pruning works properly
                if (PrevLexHypoContainer.nodes().isEmpty()){
                    // log() << "ERROR: prevLexHypContainer empty. card: " << previousCardinality << ", cov Vector: " << previousCoverageVector.getString() << "\n";
                    continue;
                }

                /*
                 * TODO at this point pbt performs cardinality pruning (cf. discardedAtA / cardhist, cardthres pruning)
                 * this is not implemented here.
                 */

                for (size_t StartPos = 0; StartPos < (size_t)this->Data->Sentence.size() - NewPhraseCardinality; ++StartPos){
                    unsigned LastPhrasePos = StartPos + NewPhraseCardinality;
                    //log() << "start position: "<< startPosition << "\n";

                    // skip if phrase coverage is not compatible with previous sentence coverage
                    bool SkipStep = false;
                    for (size_t i= StartPos; i<LastPhrasePos; ++i)
                        if (PrevCoverage.testBit(i)){
                            SkipStep = true;
                            break;
                        }
                    if (SkipStep)
                        continue;

                    // generate new coverage vector, containing the new coverage
                    Coverage_t NewCoverage(PrevCoverage);
                    for (size_t i=StartPos; i<LastPhrasePos; ++i)
                        NewCoverage.setBit(i);

                    //Compute number of transitions from 0 to 1 in NewCoverage bit vector
                    //TODO refactor
                    int Last = 0;
                    int Runs = 0;

                    for(unsigned i=0;i<(size_t)NewCoverage.size();++i) {
                        bool Curr=NewCoverage.testBit(i);
                        if(Last==Curr) continue;
                        if(Last) Last=0;
                        else {Last=1;++Runs;if(Runs>this->ReorderingMaximumJumpWidth.value()) Runs=-1;break;}
                    }

                    if (Runs <0)// check IBM reordering constraint for new coverage
                        continue;

                    clsRuleNode& PhraseCandidates = this->Data->PhraseMatchTable[StartPos][NewPhraseCardinality];

                    if (PhraseCandidates.isInvalid())
                        continue;

                    Cost_t RestCost =  this->calculateRestCost(NewCoverage, LastPhrasePos);
                    Cost_t BestCandidateCost = clsSearchGraphBuilder::pPhraseTable->getTargetRuleCost(
                                0,
                                0,
                                PhraseCandidates.targetRules().first());

                    clsLexicalHypothesis& NewLexHypoContainer =
                            this->Data->HypothesisHolder[NewCardinality][NewCoverage];

                    if (this->PruneAtStage2.value() && NewLexHypoContainer.mustBePruned(
                                BestCandidateCost + RestCost + PrevLexHypoContainer.getBestCost())){
                        PrunedAt2++;
                        continue;
                    }


                    int SourceRangeEnd = 0;
                    for (int i=PrevCoverage.size() - 1; i>=0; --i)
                        if (PrevCoverage.testBit(i)){
                            SourceRangeEnd = i;
                            break;
                        }


                    // get reordering distance
                    size_t JumpWidth = qAbs(SourceRangeEnd - StartPos);

                    if (this->ReorderingHardJumpLimit.value() == false ||
                            JumpWidth <= this->ReorderingMaximumJumpWidth.value()){
                        Cost_t ReorderingJumpCosts = computeReorderingJumpCost(JumpWidth);

                        Cost_t ScaledReorderingJumpCost = ReorderingJumpCosts * this->ScalingFactorReorderingJump.value();

                        //log() << "hypothesisBaseCosts = " << hypothesisBaseCosts << "\n";
                        //log() << "scalingFactorReorderingJump_ * reorderingJumpCosts = " << scalingFactorReorderingJump_ * reorderingJumpCosts << "\n";
                        //log() << "reorderingJumpCosts = " << reorderingJumpCosts<< "\n";

                        foreach (const clsSearchGraphNode& PrevLexHypoNode, PrevLexHypoContainer.nodes()) {
                            Cost_t HypoBaseCost =
                                    RestCost +
                                    PrevLexHypoNode.getCost() +
                                    ScaledReorderingJumpCost;

                            //log() << "restCosts = " << restCosts << "\n";
                            //log() << "getBestPhraseCosts_(phraseCandidates) = " << getBestPhraseCosts_(phraseCandidates) << "\n";
                            //log() << "previousLexicalHypothesisNode->getFirstBestCost() = " << previousLexicalHypothesisNode->getFirstBestCost() << "\n";
                            if (this->PruneAtStage3.value() &&
                                    NewLexHypoContainer.mustBePruned(HypoBaseCost + BestCandidateCost)){
                                PrunedAt3++;
                                continue;
                            }

                            intfLMSentenceScorer* LMScorer = gConfigs.LM.getInstance<intfLMSentenceScorer>();
                            LMScorer->initHistory(PrevLexHypoNode.lmScorer());

                            size_t MaxCandidates = qMin((int)this->ObservationHistogramSize.value(),
                                                        PhraseCandidates.targetRules().size());

                            for(size_t i = 0; i<MaxCandidates; ++i){
                                //log() << "phraseCandidate "<< phraseCandidateNumber << "\n";
                                //log() << "phraseCandidate "<< phraseCandidateNumber << ": "<< targetPart<<"\n";
                                //log() << "hypothesisBaseCosts = " << hypothesisBaseCosts << "\n";
                                const clsTargetRule& CurrentPhraseCandidate = PhraseCandidates.targetRules().at(i);
                                Cost_t PhraseCost =
                                        clsSearchGraphBuilder::pPhraseTable->getTargetRuleCost(
                                            0,
                                            0,
                                            CurrentPhraseCandidate);
                                //log() << "phraseCosts = " << phraseCosts << "\n";

                                if (this->PruneAtStage4.value() &&
                                        NewLexHypoContainer.mustBePruned(HypoBaseCost + PhraseCost)){
                                    PrunedAt4++;
                                    break;
                                }

                                Cost_t LMCost = 0;
                                for (size_t j=0; j<CurrentPhraseCandidate.size(); ++j)
                                    LMScorer->wordProb(CurrentPhraseCandidate.at(j));

                                Cost_t CurrCost = PrevLexHypoNode.getCost() + PhraseCost + ScaledReorderingJumpCost;

                                Cost_t FinalJumpCost = 0;

                                if (IsFinal){
                                    LMCost += LMScorer->wordProb(LMScorer->endOfSentence());
                                    size_t FinalJumpWidth = qAbs(this->Data->Sentence.size() - LastPhrasePos);
                                    FinalJumpCost = this->computeReorderingJumpCost(FinalJumpWidth) *
                                            clsSearchGraphBuilder::ScalingFactorReorderingJump.value();
                                    //addtoSeparateCosts ignored
                                }

                                CurrCost += LMCost;

                                if (NewLexHypoContainer.mustBePruned(CurrCost + RestCost + 1e-10))
                                    continue;

                                //Pruning based on Reordering
                                if (this->Data->HypothesisHolder[NewCardinality].mustBePruned(CurrCost + RestCost))
                                    continue;

                                clsSearchGraphNode* NewHypoNode =
                                        new clsSearchGraphNode(PrevLexHypoNode,
                                                               CurrentPhraseCandidate,
                                                               CurrCost + FinalJumpCost,
                                                               ReorderingJumpCosts,
                                                               RestCost,
                                                               LMCost,
                                                               StartPos,
                                                               LastPhrasePos,
                                                               PrevCoverageIter.key(),
                                                               IsFinal,
                                                               LMScorer);

                                //Final Pruning ignored

                                if (NewLexHypoContainer.insertHypothesis(*NewHypoNode) == false)
                                    delete NewHypoNode;
                                else
                                    this->Data->HypothesisHolder[NewCardinality].updateWorstNode(NewCoverage, *NewHypoNode);

                            }
                        }//foreach (const clsSearchGraphNode& PrevLexHypoNode, PrevLexHypoContainer.nodes())
                        if (NewLexHypoContainer.nodes().isEmpty())
                            this->Data->HypothesisHolder[NewCardinality].remove(NewCoverage);
                    }
                }
            }
        }

        /*        if (verbosity_ >= Core::verbosityLevelAdditionalDebug) {
          log() << "before ro pruning: CovContainer size = " << cardinalityContainer_[newCardinality]->size() << "\n";
          log() << LeftToRightDecoder::constructed << " Hyps created after cardinality " << newCardinality << "\n";
          log() << LeftToRightDecoder::insertedHyps << " Hyps inserted\n";
          log() << " pruned at 2 3 4 lmlookahead lmlookaheadFullPhrase lmlookaheadBoth: " << prunedAt2 << " " << prunedAt3 << " " << prunedAt4 << " " << prunedLmLookAhead << " " << prunedLmLookAheadFullPhrase << " " << prunedLmLookAheadBoth << "\n";
        }*/

    }//for new cardinality

    Coverage_t FullCoverage;
    FullCoverage.fill(1, this->Data->Sentence.size());

    if(this->Data->HypothesisHolder[this->Data->Sentence.size()].lexicalHypotheses().size() > 0 &&
            this->Data->HypothesisHolder[this->Data->Sentence.size()][FullCoverage].nodes().isEmpty() == false)
    {
        this->Data->GoalNode = this->Data->HypothesisHolder[this->Data->Sentence.size()][FullCoverage].bestNode();

        if(gConfigs.KeepRecombined.value())
            this->Data->HypothesisHolder[this->Data->Sentence.size()][FullCoverage].finalizeRecombination();
        return true;
    } else {
        // no translation with full coverage found
        return false;
    }
}

void clsSearchGraphBuilder::initializeRestCostsMatrix()
{
    this->Data->RestCostMatrix.resize(this->Data->Sentence.size());
    for (int i=0; i<this->Data->Sentence.size(); ++i)
        this->Data->RestCostMatrix[i].resize(this->Data->Sentence.size());

    for(size_t FirstPosition = 0; FirstPosition < (size_t)this->Data->Sentence.size(); ++FirstPosition){
        size_t MaxLength = qMin(this->Data->Sentence.size() - FirstPosition, (size_t)this->Data->MaxMatchingSourcePhraseCardinality);
        for(size_t Length = 1; Length <= MaxLength; ++Length){
            clsRuleNode& PhraseCandidates = this->Data->PhraseMatchTable[FirstPosition][Length-1];
            if (PhraseCandidates.isInvalid() || PhraseCandidates.targetRules().isEmpty())
                continue;

            Cost_t BestCost = INFINITY;

            for (size_t i= 0;
                 i< (size_t)PhraseCandidates.targetRules().size() &&
                 i< this->ObservationHistogramSize.value();
                 ++i ){

                Cost_t Cost = 0;
                foreach (FeatureFunction::intfFeatureFunction* FF , gConfigs.ActiveFeatureFunctions)
                    Cost += FF->getApproximateCost(FirstPosition, FirstPosition + Length, PhraseCandidates.targetRules().at(i));


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
            this->Data->RestCostMatrix[FirstPosition][Length - 1]  = qMin(
                        this->Data->RestCostMatrix[FirstPosition][Length - 1],
                        SumSplit);
        }
}

Common::Cost_t clsSearchGraphBuilder::calculateRestCost(const Coverage_t &_coverage, quint16 _lastPos) const
{
    Cost_t RestCosts = this->computePhraseRestCosts(_coverage);
    //log() << "rest costs from matrix: " << restCosts << "\n";
    if(this->DoComputeReorderingRestCosts.value()) {
        RestCosts += this->computeReorderingRestCosts(_coverage,_lastPos);
    }
    //log() << "rest costs with reordering: " << restCosts << "\n";
    return RestCosts;
}

Common::Cost_t clsSearchGraphBuilder::computePhraseRestCosts(const Coverage_t &_coverage) const
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
    return RestCosts;
}

Common::Cost_t clsSearchGraphBuilder::computeReorderingRestCosts(const Coverage_t& _coverage, quint16 _lastPos) const
{
    Cost_t Sum = 0.0;
    bool LastPositionCovered = false;
    bool CurrentPositionCovered = false;
    bool NextPositionCovered = _coverage.at(0);
    size_t InputSentenceSize = _coverage.size();
    size_t JumpWidth = 0;

    size_t Position = 0;
    for(; Position < InputSentenceSize; ++Position)
    {
        LastPositionCovered = CurrentPositionCovered;
        CurrentPositionCovered = NextPositionCovered;
        NextPositionCovered = (Position + 1 == InputSentenceSize || _coverage.at(Position + 1));

        if( (Position == 0 || LastPositionCovered) && CurrentPositionCovered == 0 )
        {
            JumpWidth = std::abs((int)(_lastPos - Position));
            Sum += this->computeReorderingJumpCost(JumpWidth);
        }

        if(Position > 0 && CurrentPositionCovered == 0 && NextPositionCovered )
            _lastPos = Position + 1;
    }
    JumpWidth = std::abs((int)(_lastPos - Position));
    Sum += this->computeReorderingJumpCost(JumpWidth);;
    Q_ASSERT(Sum >= 0);
    return clsSearchGraphBuilder::ScalingFactorReorderingJump.value() * Sum;
}

}
}
}
}
