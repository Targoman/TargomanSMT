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
#include "Private/LanguageModel/intfLMSentenceScorer.hpp"


#define PBT_MAXIMUM_COST 1e200
#define PBT_IMMUNITY_BOUND 1e-10


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
        clsSearchGraphBuilder::moduleBaseconfig() + "/ReorderingConstraintMaximumRuns",
        "IBM1 reordering constraint",
        2);
tmplConfigurable<bool>   clsSearchGraphBuilder::DoComputeReorderingRestCosts(
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
tmplConfigurable<bool>   clsSearchGraphBuilder::ReorderingHardJumpLimit(
        clsSearchGraphBuilder::moduleBaseconfig() + "/ReorderingHardJumpLimit",
        "TODO Desc",
        true);
tmplConfigurable<quint8> clsSearchGraphBuilder::ReorderingMaximumJumpWidth(
        clsSearchGraphBuilder::moduleBaseconfig() + "/ReorderingMaximumJumpWidth",
        "TODO Desc",
        5);
tmplConfigurable<quint8> clsSearchGraphBuilder::ObservationHistogramSize(
        clsSearchGraphBuilder::moduleBaseconfig() + "/ObservationHistogramSize",
        "TODO Desc",
        100);
tmplConfigurable<double> clsSearchGraphBuilder::ScalingFactorReorderingJump(
        clsSearchGraphBuilder::moduleBaseconfig() + "/ScalingFactorReorderingJump",
        "TODO Desc",
        1);
tmplConfigurable<double> clsSearchGraphBuilder::ScalingFactorLM(
        clsSearchGraphBuilder::moduleBaseconfig() + "/ScalingFactorLM",
        "TODO Desc",
        1);

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

    //InvalidTargetRule has been marshalled here because it depends on loading RuleTable
    RuleTable::InvalidTargetRule = new RuleTable::clsTargetRule;

    //InvalidSearchGraphNodeData has been marshalled here because it depends on initialization of gConfigs
    InvalidSearchGraphNodeData = new clsSearchGraphNodeData;

    InvalidSearchGraphNodePointer = new clsSearchGraphNode;
}

void clsSearchGraphBuilder::matchPhrase()
{
    this->Data->MaxMatchingSourcePhraseCardinality = 0;
    for (size_t FirstPosition = 0; FirstPosition < (size_t)this->Data->Sentence.size(); ++FirstPosition) {
        this->Data->PhraseMatchTable.append(QVector<clsRuleNode>(this->Data->Sentence.size() - FirstPosition));
        RulesPrefixTree_t::Node* PrevNode = this->pRuleTable->getPrefixTree().rootNode();

        { //On Uni-Grams
            PrevNode = PrevNode->follow(this->Data->Sentence.at(FirstPosition).wordIndex());

            if (PrevNode == NULL)
                continue; // appending next word breaks phrase lookup

            this->Data->PhraseMatchTable[FirstPosition][0] = PrevNode->getData();

            //#define DEBUG_MATCH_PHRASE
#ifdef DEBUG_MATCH_PHRASE
            QString TargetPhrasesCounters;
            if (this->Data->PhraseMatchTable[FirstPosition][0].isInvalid() == false){
                clsRuleNode& RuleNode = this->Data->PhraseMatchTable[FirstPosition][0];

                for(int i=0; i< RuleNode.targetRules().size(); ++i)
                    TargetPhrasesCounters += QString::number(RuleNode.targetRules().at(i).size()) + ";";
            }

            QBitArray Cov;
            Cov.resize(this->Data->Sentence.size());
            for (int i=FirstPosition; i<= FirstPosition; ++i )
                Cov.setBit(i);

            std::cout<<"JANECOMPARE: FP:"<<FirstPosition<<
                       " LP:"<<FirstPosition<<
                       " TRs:"<<this->Data->PhraseMatchTable[FirstPosition][0].targetRules().size()<<
                       " TRTPs:"<<TargetPhrasesCounters.toLatin1().constData()<<
                       " Cov:"<<bitArray2Str(Cov).toLatin1().constData()<<std::endl;

#endif

            if (this->Data->PhraseMatchTable[FirstPosition][0].isInvalid() == false)
                this->Data->MaxMatchingSourcePhraseCardinality = qMax(this->Data->MaxMatchingSourcePhraseCardinality,1);
            //else
            //TODO getRulonde from OOVHandler
        }

        //Max PhraseTable order will be implicitly checked by follow
        for (size_t LastPosition = FirstPosition + 1; LastPosition < (size_t)this->Data->Sentence.size() ; ++LastPosition){
            PrevNode = PrevNode->follow(this->Data->Sentence.at(LastPosition).wordIndex());

            if (PrevNode == NULL)
                break; // appending next word breaks phrase lookup

            this->Data->PhraseMatchTable[FirstPosition][LastPosition - FirstPosition] = PrevNode->getData();

#ifdef DEBUG_MATCH_PHRASE
            QString TargetPhrasesCounters;
            if (this->Data->PhraseMatchTable[FirstPosition][LastPosition - FirstPosition].isInvalid() == false){
                clsRuleNode& RuleNode = this->Data->PhraseMatchTable[FirstPosition][LastPosition - FirstPosition];

                for(int i=0; i< RuleNode.targetRules().size(); ++i)
                    TargetPhrasesCounters += QString::number(RuleNode.targetRules().at(i).size()) + ";";
            }

            QBitArray Cov;
            Cov.resize(this->Data->Sentence.size());
            for (int i=FirstPosition; i<= LastPosition; ++i )
                Cov.setBit(i);

            std::cout<<"JANECOMPARE: FP:"<<FirstPosition<<
                       " LP:"<<LastPosition<<
                       " TRs:"<<this->Data->PhraseMatchTable[FirstPosition][LastPosition - FirstPosition].targetRules().size()<<
                       " TRTPs:"<<TargetPhrasesCounters.toLatin1().constData()<<
                       " Cov:"<<bitArray2Str(Cov).toLatin1().constData()<<std::endl;

#endif
            if (this->Data->PhraseMatchTable[FirstPosition][LastPosition - FirstPosition].isInvalid() == false)
                this->Data->MaxMatchingSourcePhraseCardinality = qMax(this->Data->MaxMatchingSourcePhraseCardinality,
                                                                      (int)(LastPosition - FirstPosition + 1));
        }
    }


#ifdef DEBUG_MATCH_PHRASE
    exit(0);
#endif
}

Cost_t clsSearchGraphBuilder::computeReorderingJumpCost(size_t JumpWidth) const
{
    Cost_t ReorderingJumpCosts = JumpWidth;

    if (JumpWidth > this->ReorderingMaximumJumpWidth.value())
        ReorderingJumpCosts = JumpWidth + (JumpWidth * JumpWidth);

    return ReorderingJumpCosts;
}

bool clsSearchGraphBuilder::conformsIBM1Constraint(const Coverage_t& _newCoverage)
{
   /* //Jane implementation that seems to be wrong
         bool last = 0;
    int runs=0;
    for(unsigned i=0;i<_newCoverage.size();++i) {
      bool curr=_newCoverage.testBit(i);
      if(last==curr) continue;
      if(last) last=0;
      else {last=1;++runs;if(runs>this->ReorderingMaximumJumpWidth.value()) return false;}
    }
    return true;
*/

    //Find last bit set then check how many bits are zero before this.
    for(int i=_newCoverage.size() - 1; i>=0; --i)
        if(_newCoverage.testBit(i)){
            size_t CoutOfPrevZeros = _newCoverage.count(false) + i - _newCoverage.size() + 1;
            return (CoutOfPrevZeros <= this->ReorderingConstraintMaximumRuns.value());
        }
    return true;
}

bool clsSearchGraphBuilder::conformsHardJumpConstraint(size_t _newPhraseBeginPos, Coverage_t _newCoverage, size_t _newPhraseEndPos)
{
    for (int i=_newPhraseBeginPos - 1; i>=0; --i)
        if (_newCoverage.testBit(i) == false){
            if(_newPhraseEndPos - i > clsSearchGraphBuilder::ReorderingMaximumJumpWidth.value())
                return false;
            else
                return true;
        }
    return true;
}

bool clsSearchGraphBuilder::parseSentence()
{
//#define DEBUG_PARSE_SENTENCE_CARDINALITY 5

    this->Data->HypothesisHolder.clear();
    this->Data->HypothesisHolder.resize(this->Data->Sentence.size() + 1);

    this->Data->RootNode = &this->Data->HypothesisHolder.getRootNode();

    this->initializeRestCostsMatrix();


    for (int NewCardinality = 1; NewCardinality <= this->Data->Sentence.size(); ++NewCardinality){
        int PrunedAt2 = 0;
        int PrunedAt3 = 0;
        int PrunedAt4 = 0;
        int PrunedAtReordering = 0;
        int PrunedByIBMConstraint = 0;
        int PrunedByHardJumpConstraint = 0;

        bool IsFinal = (NewCardinality == this->Data->Sentence.size());
        int MinPrevCardinality = qMax(NewCardinality - this->Data->MaxMatchingSourcePhraseCardinality, 0);
        clsSearchGraphNode* BestHardJumpViolatingNode = NULL;

        for (int PrevCardinality = MinPrevCardinality;
             PrevCardinality < NewCardinality; ++PrevCardinality) {

            unsigned short NewPhraseCardinality = NewCardinality - PrevCardinality;

            //This happens when we have for ex. 2 bi-grams and a quad-gram but no similar 3-gram. due to bad training
            if(this->Data->HypothesisHolder[PrevCardinality].isEmpty()) {
                //TODO targomanlogwarning
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

                    // generate new coverage vector, containing the new coverage
                    Coverage_t NewCoverage(PrevCoverage);
                    for (size_t i=NewPhraseBeginPos; i<NewPhraseEndPos; ++i)
                        NewCoverage.setBit(i);

                    if (this->conformsIBM1Constraint(NewCoverage) == false){
                        ++PrunedByIBMConstraint;
                        continue;
                    }

                    if (clsSearchGraphBuilder::ReorderingHardJumpLimit.value() &&
                        IsFinal == false &&
                        this->conformsHardJumpConstraint(NewPhraseBeginPos, NewCoverage, NewPhraseEndPos) == false){
                        ++PrunedByHardJumpConstraint;
                        continue;
                    }

                    clsRuleNode& PhraseCandidates =
                            this->Data->PhraseMatchTable[NewPhraseBeginPos][NewPhraseCardinality - 1];

                    //There is no rule defined in rule table for current phrase
                    if (PhraseCandidates.isInvalid())
                        continue; //TODO If there are no more places to fill after this startpos break

                    Cost_t RestCost =  this->calculateRestCost(NewCoverage, NewPhraseEndPos);
                    Cost_t BestCandidateCost = clsSearchGraphBuilder::pPhraseTable->getTargetRuleCost(
                                0,
                                0,
                                PhraseCandidates.targetRules().first());

                    clsLexicalHypothesis& NewLexHypoContainer =
                            this->Data->HypothesisHolder[NewCardinality][NewCoverage];

                    //If best phrase candidate is combined with best previous node but computyed cost is worst than worst node then ignore it
                    if (this->PruneAtStage2.value() && NewLexHypoContainer.mustBePruned(
                                BestCandidateCost + RestCost + PrevLexHypoContainer.getBestCost())){
                        ++PrunedAt2;
                        continue;
                    }

                    foreach (const clsSearchGraphNode& PrevLexHypoNode, PrevLexHypoContainer.nodes()) {
                        // Get reordering distance
                        size_t JumpWidth = qAbs((int)PrevLexHypoNode.sourceRangeEnd() - (int)NewPhraseBeginPos);
                        bool HardJumpViolated = false;
                        if (this->ReorderingHardJumpLimit.value() &&
                            JumpWidth > this->ReorderingMaximumJumpWidth.value()){
                            if (this->Data->HypothesisHolder[NewCardinality].totalSearchGraphNodeCount() > 0){

                                continue;
                            }else
                                HardJumpViolated = true;
                        }

                        Cost_t ReorderingJumpCosts = computeReorderingJumpCost(JumpWidth);

                        Cost_t ScaledReorderingJumpCost = ReorderingJumpCosts *
                                this->ScalingFactorReorderingJump.value();

                        Cost_t HypoBaseCost =
                                RestCost +
                                PrevLexHypoNode.getCost() +
                                ScaledReorderingJumpCost;

                        //If best phrase candidate combined with current HypoNode is worst than worst stored node ignore it
                        if (this->PruneAtStage3.value() &&
                                NewLexHypoContainer.mustBePruned(HypoBaseCost + BestCandidateCost)){
                            ++PrunedAt3;
                            continue;
                        }


                        size_t MaxCandidates = qMin((int)this->ObservationHistogramSize.value(),
                                                    PhraseCandidates.targetRules().size());

                        for(size_t i = 0; i<MaxCandidates; ++i){
                            QScopedPointer<intfLMSentenceScorer> LMScorer(gConfigs.LM.getInstance<intfLMSentenceScorer>());
                            LMScorer->initHistory(PrevLexHypoNode.lmScorer());

                            const clsTargetRule& CurrentPhraseCandidate = PhraseCandidates.targetRules().at(i);
                            Cost_t PhraseCost =
                                    clsSearchGraphBuilder::pPhraseTable->getTargetRuleCost(
                                        0,
                                        0,
                                        CurrentPhraseCandidate);
#if 0
                            std::cout<<"TestLOG-"<<"Cardinality: "<<NewCardinality<<
                                      " PrevCard: "<<PrevCardinality<<
                                      " Cov: "<<bitArray2Str(PrevCoverage).toLatin1().constData()<<
                                      " StartPos: "<<NewPhraseBeginPos<<
                                      " HypoNode: "<<PrevLexHypoNode.targetRule().size()<<
                                      " Candidate: "<<i<<
                                      " PhraseCost: "<<PhraseCost<<
                                      " BaseCost: "<<HypoBaseCost<<
                                      " JumpW: "<<JumpWidth<<
                                      " ScaledJ:"<<ScaledReorderingJumpCost<<std::endl;

#endif
                            //If current phrase candidate combined with current HypoNode is worst than worst stored node ignore it
                            if (this->PruneAtStage4.value() &&
                                    NewLexHypoContainer.mustBePruned(HypoBaseCost + PhraseCost)){
                                ++PrunedAt4;
                                //TODO This must be converted to break!!!! as RuleTable must store TargetPhrases sorted.
                                continue;
                            }

                            Cost_t LMCost = 0;
                            for (size_t j=0; j<CurrentPhraseCandidate.size(); ++j)
                                LMCost -= LMScorer->wordProb(CurrentPhraseCandidate.at(j));

                            Cost_t CurrCost = PrevLexHypoNode.getCost() + PhraseCost + ScaledReorderingJumpCost;

                            Cost_t FinalJumpCost = 0;

                            if (IsFinal){
                                LMCost -= LMScorer->endOfSentenceProb();
                                size_t FinalJumpWidth = qAbs(this->Data->Sentence.size() - NewPhraseEndPos);
                                FinalJumpCost = this->computeReorderingJumpCost(FinalJumpWidth) *
                                        clsSearchGraphBuilder::ScalingFactorReorderingJump.value();
                                //addtoSeparateCosts ignored
                            }

                            CurrCost += LMCost * clsSearchGraphBuilder::ScalingFactorLM.value();

/*                            if (NewLexHypoContainer.mustBePruned(CurrCost + RestCost + PBT_IMMUNITY_BOUND)){
                                ++PrunedAtVerySmall;
                                continue;
                            }*/

                            //Pruning among different reorderings
                            if (this->Data->HypothesisHolder[NewCardinality].mustBePruned(CurrCost + RestCost)){
                                ++PrunedAtReordering;
                                continue;
                            }

                            clsSearchGraphNode* NewHypoNode =
                                    new clsSearchGraphNode(PrevLexHypoNode,
                                                           CurrentPhraseCandidate,
                                                           CurrCost + FinalJumpCost,
                                                           ReorderingJumpCosts,
                                                           RestCost,
                                                           LMCost,
                                                           NewPhraseBeginPos,
                                                           NewPhraseEndPos,
                                                           NewCoverage,
                                                           IsFinal,
                                                           LMScorer.take());

                            if (HardJumpViolated){
                                if (BestHardJumpViolatingNode){
                                    if (BestHardJumpViolatingNode->getTotalCost() >
                                            NewHypoNode->getTotalCost()){
                                        delete BestHardJumpViolatingNode;
                                        BestHardJumpViolatingNode = NewHypoNode;
                                    }else
                                        delete NewHypoNode;
                                }else{
                                    BestHardJumpViolatingNode = NewHypoNode;
                                }
                            }else{
                                this->Data->HypothesisHolder[NewCardinality].insertNewHypothesis(NewCoverage, NewLexHypoContainer, *NewHypoNode);

                                //NewHypoNode will be appended to QList as a new reference so we don't need it anymore
                                delete NewHypoNode;
                            }
                        }
                    }//foreach (const clsSearchGraphNode& PrevLexHypoNode, PrevLexHypoContainer.nodes())
                    if (NewLexHypoContainer.nodes().isEmpty())
                        this->Data->HypothesisHolder[NewCardinality].remove(NewCoverage);

                }//for NewPhraseBeginPos
            }//for PrevCoverageIter
        }//for PrevCardinality

        if (BestHardJumpViolatingNode){
            if (this->Data->HypothesisHolder[NewCardinality].totalSearchGraphNodeCount() == 0){
                Coverage_t BestViolatingNodeCoverage = BestHardJumpViolatingNode->coverage();
                clsLexicalHypothesis& BestViolatingNodeLexHypo =
                        this->Data->HypothesisHolder[NewCardinality][BestViolatingNodeCoverage];
                this->Data->HypothesisHolder[NewCardinality].insertNewHypothesis(
                            BestViolatingNodeCoverage,
                            BestViolatingNodeLexHypo,
                            *BestHardJumpViolatingNode);

            }
            delete BestHardJumpViolatingNode;
        }

#if 0
        quint64 Sum = 0;
        foreach(auto Iter , this->Data->HypothesisHolder[NewCardinality].lexicalHypotheses()){
            Sum+= Iter.nodes().size();
        }
        TargomanDebug(1,"Total Hypo for card="<<NewCardinality<<" is: "<<Sum <<" which must be: "<<this->Data->HypothesisHolder[NewCardinality].totalSearchGraphNodeCount());
        TargomanDebug(1," pruned at IBM(%d) Jump(%d) 2(%d) 3(%d) 4(%d) Reorder(%d): ",
                      PrunedByIBMConstraint,
                      PrunedByHardJumpConstraint,
                      PrunedAt2, PrunedAt3, PrunedAt4,PrunedAtReordering)
#endif
#ifdef DEBUG_PARSE_SENTENCE_CARDINALITY
                if (NewCardinality <= DEBUG_PARSE_SENTENCE_CARDINALITY){
            this->Data->HypothesisHolder[NewCardinality].dump("JANECOMPARE: Card["+
                                                              QString::number(NewCardinality) +
                                                              "] ");
        }
#endif


    }//for NewCardinality

    Coverage_t FullCoverage;
    FullCoverage.fill(1, this->Data->Sentence.size());

    if(this->Data->HypothesisHolder[this->Data->Sentence.size()].lexicalHypotheses().size() > 0 &&
            this->Data->HypothesisHolder[this->Data->Sentence.size()][FullCoverage].nodes().isEmpty() == false)
    {
        this->Data->GoalNode = &this->Data->HypothesisHolder[this->Data->Sentence.size()][FullCoverage].bestNode();

#if 0
        /********************************************************************/
        /// JUST FOR DEBUG
        std::function<QString (const clsSearchGraphNode&)> getNodeString = [&](const clsSearchGraphNode& _node) {
            if(_node.isInvalid())
                return QString();

            QString result = getNodeString(_node.prevNode());

            for(int i=0; i< _node.targetRule().size(); ++i)
                result += gConfigs.EmptyLMScorer->getWordByIndex(_node.targetRule().at(i)) + " ";

            return result;
        };

        QScopedPointer<intfLMSentenceScorer> Scorer(gConfigs.LM.getInstance<intfLMSentenceScorer>());
        foreach(const clsSearchGraphNode& Node, this->Data->HypothesisHolder[this->Data->Sentence.size()][FullCoverage].nodes()){
            std::cerr << "================================================" << std::endl;
            QString NodeSentence = getNodeString(Node);
            std::cout<< NodeSentence.toUtf8().constData()<<std::endl;
            std::cerr<< NodeSentence.toUtf8().constData()<<std::endl;
            LogP_t LMCost = 0;
            Scorer->reset();
            foreach(const QString& Token, NodeSentence.split(" ", QString::SkipEmptyParts))
                LMCost -= Scorer->wordProb(Scorer->getWordIndex(Token));
            LMCost -= Scorer->endOfSentenceProb();
            std::cerr << "Total Cost: " << Node.getTotalCost() << ", LM Cost: " << LMCost << std::endl;
            break;
        }
        /********************************************************************/
#endif
        this->Data->HypothesisHolder[this->Data->Sentence.size()][FullCoverage].finalizeRecombination();

        return true;
    } else {
        TargomanWarn(1,"No translation option");
        std::cout<<"******************************* No translation option"<<std::endl;
        // no translation with full coverage found
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

    QScopedPointer<intfLMSentenceScorer> LMSentenceScorer(gConfigs.LM.getInstance<intfLMSentenceScorer>());

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
                    Cost += FF->getApproximateCost(
                                FirstPosition,
                                FirstPosition + Length,
                                PhraseCandidates.targetRules().at(PhraseCandidateNumber));

                LMSentenceScorer->reset(false);
                const clsTargetRule& TargetRule = PhraseCandidates.targetRules().at(PhraseCandidateNumber);
                Cost_t LMCost = 0;
                for (int i=0; i< TargetRule.size(); ++i)
                    LMCost += LMSentenceScorer->wordProb(TargetRule.at(i));

                BestCost = qMin(BestCost, Cost - (LMCost * clsSearchGraphBuilder::ScalingFactorLM.value()));
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
