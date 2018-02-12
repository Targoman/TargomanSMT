#include "OSMState.h"
#include "OperationSequenceModel.h"
#include<iostream>

namespace Targoman {
namespace SMT {
namespace Private {
namespace FeatureFunction{

using namespace Common;

OSMState::OSMState(size_t _last, size_t _right, QMap<size_t, bool> _gaps, KenState &_LMState)
{
    LastGeneratedSourceIndex = _last;
    RightmostGeneratedSourceIndex = _right;
    Gaps = _gaps;
    LMState = _LMState;

}

OSMState::OSMState(){
    LastGeneratedSourceIndex = 0;
    RightmostGeneratedSourceIndex = 0;
}

int OSMState::compareState(const OSMState &_otherOSMState) const
{
    if(LastGeneratedSourceIndex != _otherOSMState.getLastGeneratedSourceIndex())
        return (LastGeneratedSourceIndex < _otherOSMState.getLastGeneratedSourceIndex()) ? -1 : 1;

    if(RightmostGeneratedSourceIndex != _otherOSMState.getRightmostGeneratedSourceIndex())
        return (RightmostGeneratedSourceIndex < _otherOSMState.getRightmostGeneratedSourceIndex()) ? -1 : 1;

    if(Gaps != _otherOSMState.getGaps())
        return (Gaps.toStdMap() < _otherOSMState.getGaps().toStdMap()) ? -1 : 1;

    if(LMState.length < _otherOSMState.getLMState().length)
        return -1;
    if(LMState.length > _otherOSMState.getLMState().length)
        return 1;

    return 0;

}

QList<Cept_t> OSMScorer::createCepts(unsigned _sourceStart, unsigned _sourceEnd,
                                                    const RuleTable::clsTargetRule& _targetRule){

    QMap<int, int> SourceToTargetAlignments;
    QMap<int, int> TargetToSourceAlignments= _targetRule.getAllWordLevelAlignments();
    for(QMap<int, int>::iterator it = TargetToSourceAlignments.begin(); it != TargetToSourceAlignments.end(); it++){
        SourceToTargetAlignments.insertMulti(it.value(), it.key());
    }

    for(unsigned i = _sourceStart; i < _sourceEnd; i++)
        if(SourceToTargetAlignments.find(i - _sourceStart) == SourceToTargetAlignments.end())
            UnalignedSourceWords.insert(i);

    for(unsigned i = 0; i < _targetRule.size(); i++)
        if(TargetToSourceAlignments.find(i) == TargetToSourceAlignments.end())
            UnalignedTargetWords.insert(i);

    QList<int> SourceSideWords, TargetSideWords;

    QQueue<std::pair<int, int > > WordsQueue;  // the first item is a flag which shows whether this node
                                               // is from source words(0) or target words (1)

    QList< Cept_t > CeptsInPhrase;

    while (!TargetToSourceAlignments.empty()) {

        WordsQueue.append(std::pair<int, int>(1, TargetToSourceAlignments.begin().key()));

        SourceSideWords.clear();
        TargetSideWords.clear();

        while(!WordsQueue.empty()){
            std::pair<int, int> p = WordsQueue.first();
            WordsQueue.removeFirst();
            if(p.first == 0){
                SourceSideWords.append(p.second);
                for(int target: SourceToTargetAlignments.values(p.second)){
                    if(TargetToSourceAlignments.find(target) != TargetToSourceAlignments.end())
                        WordsQueue.append(std::pair<int, int>(1, target));
                }
                SourceToTargetAlignments.remove(p.second);

            }else{
                TargetSideWords.append(p.second);
                for(int source: TargetToSourceAlignments.values(p.second)){
                    if(SourceToTargetAlignments.find(source) != SourceToTargetAlignments.end())
                        WordsQueue.append(std::pair<int, int>(0, source));
                }
                TargetToSourceAlignments.remove(p.second);
            }
      }

      qSort(SourceSideWords);
      qSort(TargetSideWords);
      CeptsInPhrase.append(Cept_t(SourceSideWords, TargetSideWords));

    }
    return CeptsInPhrase;
}


void OSMScorer::computeOSM(unsigned _sourceStart, Coverage_t & _coverage, const QList< Cept_t > &CeptsInPhrase){

    int TargetIndex = 0;
    size_t LastGeneratedSourceIndex = 0;
    QSet<int> GeneratedTargetIndexes;

    if(UnalignedSourceWords.find(_sourceStart) != UnalignedSourceWords.end()){
        LastGeneratedSourceIndex = _sourceStart;
        QString TargetString = "_INS_";
        generateOperation(_sourceStart, LastGeneratedSourceIndex, 2,
                          _coverage, SourcePhrase[LastGeneratedSourceIndex - _sourceStart],
                            TargetString);
    }

    if(UnalignedTargetWords.find(TargetIndex) != UnalignedTargetWords.end()){
        generateDeleteOperation(TargetIndex, GeneratedTargetIndexes);
    }

    for(Cept_t cept : CeptsInPhrase){
        QList<int> SourceSide = cept.first;
        QList<int> TargetSide = cept.second;
        QString SourceString = "";
        QString TargetString = "";

        for(int i = 0; i < SourceSide.size(); i++){
            if(SourceString.length() > 0)
                SourceString += "^_^";
            SourceString += SourcePhrase[SourceSide[i]];
        }

        TargetIndex = TargetSide[0];
        for(int i = 0; i < TargetSide.size(); i++){
            if(TargetString.length() > 0){
                TargetString += "^_^";
                if(TargetIndex + 1 == TargetSide[i])
                    TargetIndex++;
                else
                    GeneratedTargetIndexes.insert(TargetSide[i]);
            }

            TargetString += TargetPhrase[TargetSide[i]];
        }

        LastGeneratedSourceIndex = _sourceStart + SourceSide[0];
        generateOperation(_sourceStart, LastGeneratedSourceIndex, 0,
                          _coverage, SourceString, TargetString);

        for(int i = 1; i < SourceSide.size(); i++){
            LastGeneratedSourceIndex = _sourceStart + SourceSide[i];
            generateOperation(_sourceStart, LastGeneratedSourceIndex, 1,
                              _coverage, SourceString, TargetString);
        }

        TargetIndex++;
        while(GeneratedTargetIndexes.find(TargetIndex) != GeneratedTargetIndexes.end())
            TargetIndex++;

        if(UnalignedTargetWords.find(TargetIndex) != UnalignedTargetWords.end()){
            generateDeleteOperation(TargetIndex, GeneratedTargetIndexes);
        }

    }

}


void OSMScorer::generateOperation(unsigned _sourceStart, size_t _sourceIndexToGenerate, int _operationType,
                                  Coverage_t & _coverage, QString &_sourceString, QString &_targetString){
    int GapFlag = 0;
    int GapNumber = 0;

    if(LastGeneratedSourceIndex < _sourceIndexToGenerate){
        if(_coverage.testBit(LastGeneratedSourceIndex) == 0){
            OperationsSquence.append("_INS_GAP_");
            GapFlag++;
            Gaps.insert(LastGeneratedSourceIndex, true);
        }
        if(LastGeneratedSourceIndex == RightmostGeneratedSourceIndex)
            LastGeneratedSourceIndex = _sourceIndexToGenerate;
        else{
            OperationsSquence.append("_JMP_FWD_");
            LastGeneratedSourceIndex = RightmostGeneratedSourceIndex;
        }
    }

    if(LastGeneratedSourceIndex > _sourceIndexToGenerate){
        if(LastGeneratedSourceIndex < RightmostGeneratedSourceIndex
                && _coverage.testBit(LastGeneratedSourceIndex) == 0){
            OperationsSquence.append("_INS_GAP_");
            GapFlag++;
            Gaps.insert(LastGeneratedSourceIndex, true);
        }

        LastGeneratedSourceIndex = getClosestGap(_sourceIndexToGenerate, GapNumber);
        OperationsSquence.append(QString("_JMP_BCK_") + QString::number(GapNumber));

        if(LastGeneratedSourceIndex == _sourceIndexToGenerate)
            Gaps.remove(LastGeneratedSourceIndex);
    }

    if(LastGeneratedSourceIndex < _sourceIndexToGenerate){
        OperationsSquence.append("_INS_GAP_");
        Gaps.insert(LastGeneratedSourceIndex, true);
        GapFlag++;
        LastGeneratedSourceIndex = _sourceIndexToGenerate;
    }

    if(_operationType == 0){  // the first word in a cept -> TRANS

        if(_targetString == "_TRANS_SLF_")
            OperationsSquence.push_back("_TRANS_SLF_");
        else
            OperationsSquence.push_back(QString("_TRANS_") + _targetString + QString("_TO_") + _sourceString);

        int FirstGap = getFirstGap(_coverage);
        if(FirstGap != -1)
            GapWidth += LastGeneratedSourceIndex - FirstGap;

    }else if(_operationType == 2){
        OperationsSquence.push_back("_INS_" + _sourceString);
        int FirstGap = getFirstGap(_coverage);
        if(FirstGap != -1)
            GapWidth += LastGeneratedSourceIndex - FirstGap;
        DeletionCount++;

    }else{
        OperationsSquence.push_back("_CONT_CEPT_");
    }

    _coverage.setBit(LastGeneratedSourceIndex);
    LastGeneratedSourceIndex++;

    if(RightmostGeneratedSourceIndex < LastGeneratedSourceIndex)
        RightmostGeneratedSourceIndex = LastGeneratedSourceIndex;
    if(GapFlag > 0)
        GapCount++;
    OpenGapCount += Gaps.size();
    
    if(LastGeneratedSourceIndex < (size_t)_coverage.size()){
        if(_coverage.testBit(LastGeneratedSourceIndex) == 0 &&
                UnalignedSourceWords.find(LastGeneratedSourceIndex) != UnalignedSourceWords.end()){
            QString TargetString = "_INS_";
            generateOperation(_sourceStart, LastGeneratedSourceIndex, 2, _coverage,
                              SourcePhrase[LastGeneratedSourceIndex - _sourceStart], TargetString);
        }
    }

}


void OSMScorer::generateDeleteOperation(int _targetIndex, QSet<int> & _generatedTargetIndexes ){

    while(UnalignedTargetWords.find(_targetIndex) != UnalignedTargetWords.end()){
        OperationsSquence.append("_DEL_" + TargetPhrase[_targetIndex]);
        _targetIndex++;
        while(_generatedTargetIndexes.find(_targetIndex) != _generatedTargetIndexes.end())
            _targetIndex++;
    }
}

int OSMScorer::getClosestGap(size_t _sourceIndexToGenerate, int & _gapCount){
    _gapCount = 0;
    QMapIterator<size_t, bool> gapIter(Gaps);
    gapIter.toBack();
    while(gapIter.hasPrevious()){
        gapIter.previous();
        _gapCount++;
        if(_sourceIndexToGenerate >= gapIter.key())
            return gapIter.key();
    }
    _gapCount = 0;
    return -1;
}

int OSMScorer::getFirstGap(Coverage_t _coverage){

    for(int pos = 0; pos < _coverage.size(); pos++)
        if(!_coverage.testBit(pos))
            return pos;
    return -1;
}

QList<double> OSMScorer::getOSMScores(int _numberOfFeatures){

    double OperationsProbability = 0;
    KenState CurrentState = LMState;
    KenState TempState;
    for (int i = 0; i < OperationsSquence.size(); i++) {
      TempState = CurrentState;
      OperationsProbability += OperationSequenceModel::OSM->Score(TempState, OperationsSquence[i], CurrentState);
    }
    LMState = CurrentState;

    QList<double> Scores;

    Scores.append(-OperationsProbability);



//    for (int i = 0; i< OperationsSquence.size(); i++) {
//        std::cout <<OperationsSquence[i].toStdString()<<" ";

//      }

//      std::cout<<std::endl<<std::endl;

//      std::cout<<"Operation Probability "<<OperationsProbability<<std::endl;
//      std::cout<<"Gap Count "<<GapCount<<std::endl;
//      std::cout<<"Open Gap Count "<<OpenGapCount<<std::endl;
//      std::cout<<"Gap Width "<<GapWidth<<std::endl;
//      std::cout<<"Deletion Count "<<DeletionCount<<std::endl;

//      std::cout<<"____________________________"<<std::endl;

    if (_numberOfFeatures == 1)
      return Scores;

    Scores.append(-GapWidth);
    Scores.append(-GapCount);
    Scores.append(-OpenGapCount);
    Scores.append(-DeletionCount);
    return Scores;

}


}
}
}
}
