#ifndef OSMSTATE_H
#define OSMSTATE_H

#define KENLM_MAX_ORDER 6

#include "libTargomanCommon/Types.h"
#include "libTargomanCommon/Configuration/intfConfigurable.hpp"
#include "libTargomanCommon/exTargomanBase.h"
#include "libTargomanCommon/Configuration/intfConfigurableModule.hpp"
#include "Private/FeatureFunctions/intfFeatureFunction.hpp"
#include "libKenLM/lm/model.hh"

namespace Targoman {
namespace SMT {
namespace Private {
namespace FeatureFunction {

using namespace Common;

typedef std::pair<QList<int>,QList<int>> Cept_t;
typedef lm::ngram::State KenState;

class OSMState
{
public:
    OSMState(size_t _last, size_t _right, QMap<size_t, bool> _gaps, KenState &_LMState);
    OSMState();

    ~OSMState(){}
    int compareState(const OSMState &_otherOSMState) const;

    inline size_t getLastGeneratedSourceIndex() const{ return LastGeneratedSourceIndex; }
    inline size_t getRightmostGeneratedSourceIndex() const { return RightmostGeneratedSourceIndex; }
    inline QMap<size_t, bool> getGaps() const { return Gaps; }
    inline KenState getLMState() const { return LMState; }

    inline void setGap(size_t pos){
        Gaps[pos] = true;
    }
    inline void removeGap(size_t pos){
        Gaps.remove(pos);
    }

    size_t LastGeneratedSourceIndex;
    size_t RightmostGeneratedSourceIndex;
    QMap<size_t, bool> Gaps;
    KenState LMState;
};


class OSMLM{
public:
    virtual float Score(const KenState&, const QString&, KenState&) const = 0;

    virtual const KenState &BeginSentenceState() const = 0;

    virtual const KenState &NullContextState() const = 0;
};

template<class itmplModel_t> class tmplOSMLM : public OSMLM{
public:
    tmplOSMLM(const std::string& file)
      : kenLM(new itmplModel_t(file.c_str())) {}

    virtual float Score(const KenState &in_state,
                        const QString& word,
                        KenState &out_state) const {
      return kenLM->Score(in_state, kenLM->GetVocabulary().Index(word.toStdString()),
                            out_state);
    }

    virtual const KenState &BeginSentenceState() const {
      return kenLM->BeginSentenceState();
    }

    virtual const KenState &NullContextState() const {
      return kenLM->NullContextState();
    }

private:
    QScopedPointer<itmplModel_t> kenLM;
};

class OSMScorer{

public:
    OSMScorer(QList<QString> _src, QList<QString> _trg, const OSMState &_prevState, const KenState &_LMState){
        SourcePhrase = _src;
        TargetPhrase = _trg;
        LMState = _LMState;
        LastGeneratedSourceIndex = _prevState.getLastGeneratedSourceIndex();
        RightmostGeneratedSourceIndex = _prevState.getRightmostGeneratedSourceIndex();
        Gaps = _prevState.getGaps();
        GapWidth = GapCount = DeletionCount = OpenGapCount = 0;

    }

    OSMScorer(QList<QString> _src, QList<QString> _trg, const KenState &_LMState){
        SourcePhrase = _src;
        TargetPhrase = _trg;
        LMState = _LMState;
        LastGeneratedSourceIndex = 0;
        RightmostGeneratedSourceIndex = 0;
        GapWidth = GapCount = DeletionCount = OpenGapCount = 0;


    }

    QList<Cept_t> createCepts(unsigned _sourceStart, unsigned _sourceEnd, const RuleTable::clsTargetRule& _targetRule);
    void computeOSM(unsigned _sourceStart, Coverage_t & _coverage, const QList< Cept_t > &CeptsInPhrase);
    QList<double> getOSMScores(int _numberOfFeatures);
    void generateOperation(unsigned _sourceStart, size_t _sourceIndexToGenerate, int _operationType,
                                      Coverage_t & _coverage, QString &_sourceString, QString &_targetString);

    void generateDeleteOperation(int _targetIndex, QSet<int> &_generatedTargetIndexes);

    int getClosestGap(size_t _sourceIndexToGenerate, int & _gapCount);
    int getFirstGap(Coverage_t _coverage);

    inline OSMState* getState(){
        return new OSMState(LastGeneratedSourceIndex, RightmostGeneratedSourceIndex, Gaps, LMState);
    }

private:
    QSet<int> UnalignedSourceWords;
    QSet<int> UnalignedTargetWords;
    QList<QString> SourcePhrase;
    QList<QString> TargetPhrase;
    QList<QString> OperationsSquence;
    int LastGeneratedSourceIndex;
    int RightmostGeneratedSourceIndex;
    QMap<size_t, bool> Gaps;
    KenState LMState;
    int GapWidth, GapCount, DeletionCount, OpenGapCount;


};

}
}
}
}

#endif // OSMSTATE_H
