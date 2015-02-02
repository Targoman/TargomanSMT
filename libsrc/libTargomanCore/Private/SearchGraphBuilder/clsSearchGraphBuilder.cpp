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
using namespace PhraseTable;
using namespace LanguageModel;
using namespace InputDecomposer;

class clsInvalidPhraseTable : public intfPhraseTable{
public:
    clsInvalidPhraseTable():
        intfPhraseTable("INVALID_PHRASETABLE"){}
    void init(){throw Common::exTargomanNotImplemented("clsInvalidPhraseTable::init");}
    void newSentence(){throw Common::exTargomanNotImplemented("clsInvalidPhraseTable::newSentence");}
    bool isBinary(){throw Common::exTargomanNotImplemented("clsInvalidPhraseTable::isBinary"); return false;}
}InvalidPhraseTable;


intfPhraseTable& clsSearchGraphBuilder::PhraseTable = InvalidPhraseTable;

clsSearchGraphBuilder::clsSearchGraphBuilder(const Sentence_t& _sentence):
    Sentence(_sentence)
{}

void clsSearchGraphBuilder::init()
{
    PhraseTable = *gConfigs.PhraseTable.getInstance<intfPhraseTable>();
    PhraseTable.init();
}

void clsSearchGraphBuilder::matchPhrase()
{
    for (size_t FirstPosition = 0; FirstPosition < this->Sentence.size(); ++FirstPosition) {
        this->PhraseMatchTable.append(QVector<const clsRuleNode*>(this->Sentence.size() - FirstPosition,NULL));
        const RulesPrefixTree_t::Node* PrevNode = this->PhraseTable.getPrefixTree().rootNode();
        //Max PhraseTable order will be implicitly checked by follow
        for (size_t LastPosition = FirstPosition; LastPosition < this->Sentence.size() ; ++LastPosition){
            PrevNode = PrevNode->follow(this->Sentence.at(LastPosition)->wordIndex());
            if (PrevNode == NULL)
                break; // appending next word breaks phrase lookup

            //ERROR !!!!!!!!!!!!! referencecounter will be buggy
            this->PhraseMatchTable[FirstPosition][LastPosition - FirstPosition] = &PrevNode->getData();
            if (this->PhraseMatchTable[FirstPosition][LastPosition - FirstPosition]->isInvalid() == false)
                this->MaxMatchingSourcePhraseCardinality = qMax(this->MaxMatchingSourcePhraseCardinality,
                                                                LastPosition - FirstPosition);
        }
    }
}

Cost_t clsSearchGraphBuilder::computeReorderingJumpCost(size_t JumpWidth)
{
    Cost_t ReorderingJumpCosts = JumpWidth;

    if (JumpWidth > this->ReorderingMaximumJumpWidth)
        ReorderingJumpCosts = JumpWidth + (JumpWidth * JumpWidth);

    return ReorderingJumpCosts;
}

void clsSearchGraphBuilder::parseSentence()
{
    //JANE::startParsing()
    this->HypothesisHolder.clear();
    this->HypothesisHolder.resize(this->Sentence.size() + 1);

    this->RootNode = this->HypothesisHolder.getRootNode();

    this->initializeRestCostsMatrix();

    int PrunedAt2 = 0;
    int PrunedAt3 = 0;
    int PrunedAt4 = 0;
    int PrunedAt5 = 0;

    for (int NewCardinality = 1; NewCardinality <= this->Sentence.size(); ++NewCardinality){
        bool IsFinal = NewCardinality == this->Sentence.size();

        for (int PrevCardinality = qMax(NewCardinality - this->MaxMatchingSourcePhraseCardinality, 0);
             PrevCardinality < NewCardinality; ++PrevCardinality) {
            unsigned short NewPhraseCardinality = NewCardinality - PrevCardinality;
            // TODO: check if we have phrases of this cardinality. if not, continue.

            if(this->HypothesisHolder[PrevCardinality].isEmpty()) {
              //log() << "ERROR: cardinality Container for previous cardinality empty.\n";
              continue;
            }

            for(LexicalHypothesisContainer_t::ConstIterator PrevCoverageIter : this->HypothesisHolder[PrevCardinality].lexicalHypotheses()){
                const Coverage_t& PrevCoverage = PrevCoverageIter.key();
                const clsLexicalHypothesis& PrevLexHypoContainer = PrevCoverageIter.value();

                // TODO: this can be removed if pruning works properly
                if (PrevLexHypoContainer.isEmpty()){
                   // log() << "ERROR: prevLexHypContainer empty. card: " << previousCardinality << ", cov Vector: " << previousCoverageVector.getString() << "\n";
                    continue;
                }

                /*
                 * TODO at this point pbt performs cardinality pruning (cf. discardedAtA / cardhist, cardthres pruning)
                 * this is not implemented here.
                 */

                for (size_t StartPos = 0; StartPos < this->Sentence.size() - NewPhraseCardinality; ++StartPos){
                    unsigned LastPhrasePos = StartPos + NewPhraseCardinality;
                    //log() << "start position: "<< startPosition << "\n";

                    // skip if phrase coverage is not compatible with previous sentence coverage
                    bool SkipStep = false;
                    for (int i= StartPos; i<LastPhrasePos; ++i)
                        if (PrevCoverage.testBit(i)){
                            SkipStep = true;
                            break;
                        }
                    if (SkipStep)
                        continue;

                    // generate new coverage vector, containing the new coverage
                    Coverage_t NewCoverage(PrevCoverage);
                    for (int i=StartPos; i<LastPhrasePos; ++i)
                        NewCoverage.setBit(i);

                    //Compute number of transitions from 0 to 1 in NewCoverage bit vector
                    //TODO refactor
                    int Last=Runs=0;
                    for(unsigned i=0;i<NewCoverage.size();++i) {
                      bool Curr=NewCoverage.testBit(i);
                      if(Last==Curr) continue;
                      if(Last) Last=0;
                      else {Last=1;++Runs;if(Runs>MaxRuns) Runs=-1;break;}
                    }

                    if (Runs <0)// check IBM reordering constraint for new coverage
                        continue;

                    clsRuleNode* PhraseCandidates = this->PhraseMatchTable[StartPos][NewPhraseCardinality];

                    if (PhraseCandidates == NULL)
                        continue;

                    Cost_t RestCost =  this->calculateRestCost(NewCoverage, LastPhrasePos);

                    clsLexicalHypothesis& NewLexHypoContainer =
                            this->HypothesisHolder[NewCardinality].getOrCreateLexicalHypothesisContainer(NewCoverage);

                    Cost_t BaseCost = RestCost + PhraseCandidates->targetRules().first()->getPhraseCost();

                    if (this->PruneAtStage2 && NewLexHypoContainer.mustBePruned(
                                BaseCost + PrevLexHypoContainer.getBestCost())){
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

                    if (this->ReorderingHardJumpLimit == false || JumpWidth <= this->ReorderingMaximumJumpWidth){
                        Cost_t ReorderingJumpCosts = computeReorderingJumpCost(JumpWidth);

                        Cost_t ScaledReorderingJumpCost = ReorderingJumpCosts * this->ScalingFactorReorderingJump;

                        //log() << "hypothesisBaseCosts = " << hypothesisBaseCosts << "\n";
                        //log() << "scalingFactorReorderingJump_ * reorderingJumpCosts = " << scalingFactorReorderingJump_ * reorderingJumpCosts << "\n";
                        //log() << "reorderingJumpCosts = " << reorderingJumpCosts<< "\n";

                        foreach (const clsSearchGraphNode& PrevLexHypoNode, PrevLexHypoContainer.nodes()) {
                            Cost_t HypoBaseCost = BaseCost + PrevLexHypoNode.getCost() + ScaledReorderingJumpCost;

                            //log() << "restCosts = " << restCosts << "\n";
                            //log() << "getBestPhraseCosts_(phraseCandidates) = " << getBestPhraseCosts_(phraseCandidates) << "\n";
                            //log() << "previousLexicalHypothesisNode->getFirstBestCost() = " << previousLexicalHypothesisNode->getFirstBestCost() << "\n";
                            if (this->PruneAtStage3 && NewLexHypoContainer.mustBePruned(HypoBaseCost)){
                                PrunedAt3++;
                                continue;
                            }

                            // TODO: NASTY, instead save costs without best phrase costs?
                            HypoBaseCost -=  PhraseCandidates->targetRules().first()->getPhraseCost();

                            intfLMSentenceScorer* LMScorer = gConfigs.LM.getInstance<intfLMSentenceScorer>();
                            LMScorer->initHistory(PrevLexHypoNode.lmScorer());

                            size_t MaxCandidates = qMin(this->ObservationHistogramSize,
                                                        PhraseCandidates->targetRules().size());


                            /*TODO Maybe it must be deleted
                             * for (std::vector<SecondaryModel *>::const_iterator m = secondaryModels_->begin();
                                 m != secondaryModels_->end(); ++m)
                            {
                                (*m)->setCurrentState(*previousLexicalHypothesisNode);
                            }*/

                            for(size_t i = 0; i<MaxCandidates; ++i){
                                //log() << "phraseCandidate "<< phraseCandidateNumber << "\n";
                                //log() << "phraseCandidate "<< phraseCandidateNumber << ": "<< targetPart<<"\n";
                                //log() << "hypothesisBaseCosts = " << hypothesisBaseCosts << "\n";
                                const clsTargetRule& CurrentPhraseCandidate = *PhraseCandidates->targetRules().at(i);
                                Cost_t PhraseCost = CurrentPhraseCandidate.getPhraseCost();
                                //log() << "phraseCosts = " << phraseCosts << "\n";

                                if (this->PruneAtStage4 && NewLexHypoContainer.mustBePruned(HypoBaseCost + PhraseCost)){
                                    PrunedAt4++;
                                    break;
                                }

                                Cost_t LMCost = 0;
                                for (int j=0; j<CurrentPhraseCandidate.size(); ++j)
                                    LMScorer->wordProb(CurrentPhraseCandidate[j]);

                                Cost_t CurrCost = PrevLexHypoNode.getCost() + PhraseCost + ScaledReorderingJumpCost;

                                Cost_t FinalJumpCost = 0;

                                if (IsFinal){
                                    LMCost += LMScorer->wordProb(LMScorer->endOfSentence());
                                    size_t FinalJumpWidth = qAbs(this->Sentence.size() - LastPhrasePos);
                                    FinalJumpCost = this->computeReorderingJumpCost(FinalJumpWidth);
                                    //addtoSeparateCosts ignored
                                }

                                CurrCost += LMCost;

                                if (NewLexHypoContainer.mustBePruned(CurrCost + RestCost + 1e-10))
                                    continue;

                                clsSearchGraphNode* NewHypoNode =
                                        new clsSearchGraphNode(&PrevLexHypoNode,
                                                               CurrentPhraseCandidate,
                                                               CurrCost + FinalJumpCost * ScalingFactorReorderingJump.value(),
                                                               ReorderingJumpCosts,
                                                               RestCost,
                                                               LMCost,
                                                               StartPos,
                                                               LastPhrasePos,
                                                               PrevCoverageIter.key(),
                                                               IsFinal,
                                                               LMScorer);

                                //Final Pruning ignored

                                NewLexHypoContainer.insertHypothesis(NewHypoNode);
                            }
                        }//foreach (const clsSearchGraphNode& PrevLexHypoNode, PrevLexHypoContainer.nodes())
                        if (NewLexHypoContainer.nodes().isEmpty())
                            this->HypothesisHolder[NewCardinality].remove(NewCoverage);
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

        this->HypothesisHolder[NewCardinality].performReorderingPruning();
/*        if (verbosity_ >= Core::verbosityLevelAdditionalDebug)
          log() << "after ro pruning: CovContainer size = " << cardinalityContainer_[newCardinality]->size() << "\n";
  */
    }//for new cardinality

    Coverage_t FullCoverage;
    FullCoverage.fill(1, this->Sentence.size());

}

void clsSearchGraphBuilder::initializeRestCostsMatrix()
{

}

Common::Cost_t clsSearchGraphBuilder::calculateRestCost(Coverage_t _coverage, quint16 _lastPos) const
{
    Cost_t RestCosts = this->computePhraseRestCosts(_coverage);
    //log() << "rest costs from matrix: " << restCosts << "\n";
    if(this->DoComputeReorderingRestCosts.value()) {
      RestCosts += this->computeReorderingRestCosts(_coverage,_lastPos);
    }
    //log() << "rest costs with reordering: " << restCosts << "\n";
    return RestCosts;
}

Common::Cost_t clsSearchGraphBuilder::computePhraseRestCosts(Coverage_t _coverage) const
{
    /*Cost_t RestCosts = 0.0;
    size_t StartPosition = 0, length = 0;

    for(size_t i=0; i < coverageVector.size(); ++i)
      if(!coverageVector[i])
      {
        if(length == 0) StartPosition = i;
        ++length;
      }
      else if(length)
      {
        RestCosts += restCostsMatrix_[StartPosition][length-1];
        length = 0;
      }
    if(length)
      RestCosts += restCostsMatrix_[StartPosition][length-1];
    return RestCosts;*/
}

Common::Cost_t clsSearchGraphBuilder::computeReorderingRestCosts(Coverage_t _coverage, quint16 _lastPos) const
{

}

}
}
}
}
