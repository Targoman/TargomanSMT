#include "OperationalSequenceModel.h"
#include "libTargomanCommon/Configuration/Validators.hpp"
#include <iostream>
namespace Targoman {
namespace SMT {
namespace Private {
namespace FeatureFunction{

using namespace Common;
using namespace Common::Configuration;
using namespace SearchGraphBuilder;

TARGOMAN_REGISTER_SINGLETON_MODULE(OperationalSequenceModel);


tmplConfigurable<bool>      OperationalSequenceModel::JustUseOSMProbability(
        MAKE_CONFIG_PATH("JustUseOSMProbability"),
        "Configs number of features used for OSM model",
        true);

tmplConfigurable<double>    OperationalSequenceModel::ScalingFactors[] = {
    tmplConfigurable<double>(
    MAKE_CONFIG_PATH("ScalingFactors/OSMProbability"),
    "Scaling factor for OSMProbability score",
    1),
    tmplConfigurable<double>(
    MAKE_CONFIG_PATH("ScalingFactors/GapWidth"),
    "Scaling factor for GapWidth score",
    1),
    tmplConfigurable<double>(
    MAKE_CONFIG_PATH("ScalingFactors/GapCount"),
    "Scaling factor for GapCount score",
    1),
    tmplConfigurable<double>(
    MAKE_CONFIG_PATH("ScalingFactors/OpenGapCount"),
    "Scaling factor for OpenGapCount score",
    1),
    tmplConfigurable<double>(
    MAKE_CONFIG_PATH("ScalingFactors/DeletionCount"),
    "Scaling factor for DeletionCount score",
    1)
};

tmplConfigurable<FilePath_t> OperationalSequenceModel::FilePath(
        MAKE_CONFIG_PATH("FilePath"),
        "OSM model file path. Relative to config file path if not specified as absolute.",
        ""
        "",
        Validators::tmplPathAccessValidator<
        (enuPathAccess::Type)(enuPathAccess::File | enuPathAccess::Readable)>
        );

QScopedPointer<OSMLM> OperationalSequenceModel::OSM;
double OperationalSequenceModel::UnkOperationProbability;

/**
 * @brief The clsOperationalSequenceModelFeatureData class is a derviation of intfFeatureFunctionData class.
 */
class clsOperationalSequenceModelFeatureData : public intfFeatureFunctionData{
public:

    explicit clsOperationalSequenceModelFeatureData(size_t _costElementsSize):
        intfFeatureFunctionData(_costElementsSize)
    { state.reset(new OSMState()); }

    intfFeatureFunctionData* copy() const {
        clsOperationalSequenceModelFeatureData* Copy = new clsOperationalSequenceModelFeatureData(this->CostElements.size());
        for(int i = 0; i < this->CostElements.size(); ++i)
            Copy->CostElements[i] = this->CostElements[i];
        return Copy;
    }
    QScopedPointer<OSMState> state;

};


void OperationalSequenceModel::initRootNode(clsSearchGraphNode &_rootNode)
{
    _rootNode.setFeatureFunctionData(this->DataIndex,
                                     new clsOperationalSequenceModelFeatureData(
                                         this->JustUseOSMProbability.value() ? 1 : 5));
}

int OperationalSequenceModel::compareStates(const SearchGraphBuilder::clsSearchGraphNode &_first,
                        const SearchGraphBuilder::clsSearchGraphNode &_second) const{

    const clsOperationalSequenceModelFeatureData* FirstNodeData =
            static_cast<const clsOperationalSequenceModelFeatureData*> (_first.featureFunctionDataAt(this->DataIndex));
    const clsOperationalSequenceModelFeatureData* SecondNodeData =
            static_cast<const clsOperationalSequenceModelFeatureData*> (_second.featureFunctionDataAt(this->DataIndex));

    return FirstNodeData->state->compareState(*SecondNodeData->state);
}


Common::Cost_t OperationalSequenceModel::getApproximateCost(unsigned _sourceStart,
                                  unsigned _sourceEnd,
                                  const InputDecomposer::Sentence_t& _input,
                                  const RuleTable::clsTargetRule& _targetRule) const{
    QList<QString> SourcePhrase;
    for(size_t i = _sourceStart; i < _sourceEnd; i++){

        /// TODO: Use tagstr for some input words
        SourcePhrase.append(_input[i].string());
    }
    QList<QString> TargetPhrase = _targetRule.toStr().split(" ");
    for(int i = 0; i < TargetPhrase.size(); i++){
        if(_targetRule.isUnknownWord())
            TargetPhrase[i] = "_TRANS_SLF_";
    }

    if(!_targetRule.alignmentDataAvailable())
        TargomanError(" OSM Model can not be used without alignment data in the phrase-table");

    KenState PreviousState = OSM->NullContextState();
    Coverage_t Coverage(_sourceEnd - _sourceStart);
    OSMScorer Scorer(SourcePhrase, TargetPhrase, PreviousState);
    QList<Cept_t> CeptsInPhrase = Scorer.createCepts(_sourceStart, _sourceEnd, _targetRule);
    Scorer.computeOSM(0, Coverage, CeptsInPhrase);
    int NumberOfFeatures = (JustUseOSMProbability.value() == true ? 1 : 5);

    QList<double> Scores = Scorer.getOSMScores(NumberOfFeatures);

    Cost_t Cost = 0;
    for(int i = 0; i < NumberOfFeatures; i++){
        Cost += (Scores[i] * ScalingFactors[i].value());
    }

    return Cost;

}

Common::Cost_t OperationalSequenceModel::scoreSearchGraphNodeAndUpdateFutureHash(SearchGraphBuilder::clsSearchGraphNode& _newHypothesisNode,
                                                                                 const InputDecomposer::Sentence_t& _input,
                                                                                 QCryptographicHash& _hash) const{
    Q_UNUSED(_hash)

    QList<QString> SourcePhrase;
    QList<QString> TargetPhrase = _newHypothesisNode.targetRule().toStr().split(" ");
     /// TODO : change OOV target words to "_TRANS_SLF_"

    for(int i = 0; i < TargetPhrase.size(); i++){
        if(_newHypothesisNode.targetRule().isUnknownWord())
            TargetPhrase[i] = "_TRANS_SLF_";
    }
    Coverage_t Coverage = _newHypothesisNode.coverage();

    for(size_t i = _newHypothesisNode.sourceRangeBegin(); i < _newHypothesisNode.sourceRangeEnd(); i++){

        /// TODO: Use tagstr for some input words
        SourcePhrase.append(_input[i].string());
        Coverage.clearBit(i);
    }

    int NumberOfFeatures = (JustUseOSMProbability.value() == true ? 1 : 5);

    const clsOperationalSequenceModelFeatureData* PrevNodeData =
            static_cast<const clsOperationalSequenceModelFeatureData*>
            (_newHypothesisNode.prevNode().featureFunctionDataAt(this->DataIndex));
    KenState PrevState = OSM->BeginSentenceState();
    if(!_newHypothesisNode.prevNode().isInvalid())
        PrevState = PrevNodeData->state->getLMState();
    clsOperationalSequenceModelFeatureData* Data = new clsOperationalSequenceModelFeatureData(NumberOfFeatures);


    OSMScorer Scorer(SourcePhrase, TargetPhrase, *PrevNodeData->state, PrevState);
    QList<Cept_t> CeptsInPhrase = Scorer.createCepts(_newHypothesisNode.sourceRangeBegin(),_newHypothesisNode.sourceRangeEnd(),
                                                     _newHypothesisNode.targetRule());
    Scorer.computeOSM(_newHypothesisNode.sourceRangeBegin(), Coverage, CeptsInPhrase);
    QList<double> Scores = Scorer.getOSMScores(NumberOfFeatures);

    Cost_t Cost = 0;
    for(int i = 0; i < NumberOfFeatures; i++){
        Cost += (Scores[i] * ScalingFactors[i].value());
    }

    Data->setCostElements(QVector<Cost_t>::fromList(Scores));
    Data->state.reset(Scorer.getState());
    _newHypothesisNode.setFeatureFunctionData(this->DataIndex, Data);

    return Cost;


}


}
}
}
}
