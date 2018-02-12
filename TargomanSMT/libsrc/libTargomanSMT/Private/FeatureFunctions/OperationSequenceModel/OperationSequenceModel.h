#ifndef OperationSeqUENCEMODEL_H
#define OperationSeqUENCEMODEL_H

#include "libTargomanCommon/Types.h"
#include "libTargomanCommon/Configuration/tmplConfigurable.h"
#include "Private/FeatureFunctions/intfFeatureFunction.hpp"
#include "OSMState.h"

namespace Targoman {
namespace SMT {
namespace Private {
namespace FeatureFunction {

TARGOMAN_DEFINE_ENHANCED_ENUM(enuOSMFields,
                              OSMProbability,
                              GapWidth,
                              GapCount,
                              OpenGapCount,
                              DeletionCount
                              )

TARGOMAN_ADD_EXCEPTION_HANDLER(exOperationSequenceModel, exFeatureFunction);

using namespace Common;
class OperationSequenceModel : public intfFeatureFunction
{
public:

    ~OperationSequenceModel(){}

    void initialize(QSharedPointer<QSettings>){
        ConstructOSMLM(FilePath.value());

        QString UnkOperation = "_TRANS_SLF_";
        KenState startState = OSM->NullContextState();
        KenState endState;
        UnkOperationProbability = OSM->Score(startState, UnkOperation, endState);
    }

    Common::Cost_t scoreSearchGraphNodeAndUpdateFutureHash(SearchGraphBuilder::clsSearchGraphNode& _newHypothesisNode,
                                                           const InputDecomposer::Sentence_t& _input, QCryptographicHash& _hash) const;

    Common::Cost_t getRestCostForPosition(const Coverage_t& _coverage, size_t _beginPos, size_t endPos) const {
        Q_UNUSED(_coverage);
        Q_UNUSED(_beginPos);
        Q_UNUSED(endPos);
        return 0;
    }

    /**
     * @brief Computes approximate cost to the future cost heuristic
     * @note Either getRestCostForPosition or this function must return 0
     */
    Common::Cost_t getApproximateCost(unsigned _sourceStart,
                                      unsigned _sourceEnd,
                                      const InputDecomposer::Sentence_t& _input,
                                      const RuleTable::clsTargetRule& _targetRule) const;

    int compareStates(const SearchGraphBuilder::clsSearchGraphNode &_first,
                            const SearchGraphBuilder::clsSearchGraphNode &_second) const;


    inline QStringList columnNames() const{
        return OperationSequenceModel::JustUseOSMProbability.value() ?
                    enuOSMFields::options().mid(0, 1) :
                    enuOSMFields::options();
    }

    inline QList<double> getScalingFactors() const{
        QList<double> res;
        res.push_back(this->ScalingFactors[0].value());
        if(this->JustUseOSMProbability.value())
            for(int i = 1; i < 5; i++)
                res.push_back(this->ScalingFactors[i].value());
        return res;
    }

    void initRootNode(SearchGraphBuilder::clsSearchGraphNode &_rootNode);

    void ConstructOSMLM(const FilePath_t &_filePath)
    {
      lm::ngram::ModelType modelType;
      if (lm::ngram::RecognizeBinary(_filePath.toStdString().c_str(), modelType)) {

        switch(modelType) {
        case lm::ngram::PROBING:
          OSM.reset(new tmplOSMLM<lm::ngram::ProbingModel>(_filePath.toStdString().c_str()));
            break;
        case lm::ngram::REST_PROBING:
          OSM.reset(new tmplOSMLM<lm::ngram::RestProbingModel>(_filePath.toStdString().c_str()));
            break;
        case lm::ngram::TRIE:
          OSM.reset(new tmplOSMLM<lm::ngram::TrieModel>(_filePath.toStdString().c_str()));
            break;
        case lm::ngram::QUANT_TRIE:
          OSM.reset(new tmplOSMLM<lm::ngram::QuantTrieModel>(_filePath.toStdString().c_str()));
            break;
        case lm::ngram::ARRAY_TRIE:
          OSM.reset(new tmplOSMLM<lm::ngram::ArrayTrieModel>(_filePath.toStdString().c_str()));
            break;
        case lm::ngram::QUANT_ARRAY_TRIE:
          OSM.reset(new tmplOSMLM<lm::ngram::QuantArrayTrieModel>(_filePath.toStdString().c_str()));
            break;
        default:
          TargomanError("Unrecognized OSM model type " << modelType);
        }
      } else {
        OSM.reset(new tmplOSMLM<lm::ngram::ProbingModel>(_filePath.toStdString().c_str()));
      }
    }

public:

    static QScopedPointer<OSMLM> OSM;

private:
    static  double UnkOperationProbability;

    static Targoman::Common::Configuration::tmplConfigurable<FilePath_t> FilePath;
    static Common::Configuration::tmplConfigurable<bool>      JustUseOSMProbability;      /**< Whether to use just OSM probability
                                                                                           feature or all the 5 OSM related features:
                                                                                           gapCount, gapWidth, openGapCount, deletionCount.*/
    static Common::Configuration::tmplConfigurable<double>    ScalingFactors[5];       /**< Scale factor of OSM costs.*/

    TARGOMAN_SMT_DEFINE_FEATUREFUNCTION(OperationSequenceModel, false)

};

}
}
}
}
#endif // OperationSeqUENCEMODEL_H
