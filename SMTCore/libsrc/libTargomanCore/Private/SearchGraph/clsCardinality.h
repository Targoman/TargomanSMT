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

#ifndef TARGOMAN_CORE_PRIVATE_SEARCHGRAPHBUILDER_CLSCARDINALITY_H
#define TARGOMAN_CORE_PRIVATE_SEARCHGRAPHBUILDER_CLSCARDINALITY_H

#include <QHash>
#include <QBitArray>
#include "clsLexicalHypothesis.h"
#include "Private/PrivateTypes.h"

namespace Targoman{
namespace Core {
namespace Private{
namespace SearchGraph {

typedef QMap<Coverage_t, clsLexicalHypothesisContainer> CoverageLexicalHypothesisMap_t;

class clsCardinalityHypothesisContainerData : public QSharedData
{
public:
    clsCardinalityHypothesisContainerData(){
        this->WorstLexicalHypothesis = NULL;
        this->TotalSearchGraphNodeCount = 0;
    }

    clsCardinalityHypothesisContainerData(const clsCardinalityHypothesisContainerData& _other) :
        QSharedData(_other),
        LexicalHypothesisContainer(_other.LexicalHypothesisContainer)
    {}
    ~clsCardinalityHypothesisContainerData(){}

public:
    CoverageLexicalHypothesisMap_t LexicalHypothesisContainer;
    size_t                       TotalSearchGraphNodeCount;
    clsLexicalHypothesisContainer*        WorstLexicalHypothesis;
    Coverage_t                   WorstCoverage;
};

class clsCardinalityHypothesisContainer
{
public:
    clsCardinalityHypothesisContainer();
    ~clsCardinalityHypothesisContainer(){}

    inline clsLexicalHypothesisContainer& operator [] (const Coverage_t& _coverage){
        /*if(this->Data->LexicalHypothesisContainer.contains(_coverage) == false){
            TargomanDebug(1,"Creating new LexHypo for coverage = " + bitArray2Str(_coverage));
        }*/
        return this->Data->LexicalHypothesisContainer[_coverage];
    }

    void dump(const QString &_prefix);

    inline size_t isEmpty(){
        return this->Data->LexicalHypothesisContainer.isEmpty();
    }

    inline CoverageLexicalHypothesisMap_t& lexicalHypotheses(){
        return this->Data->LexicalHypothesisContainer;
    }

    inline static clsCardinalityHypothesisContainer rootCoverageContainer(Coverage_t _emptyCoverage){
        clsCardinalityHypothesisContainer CoverageContainer;
        CoverageContainer.Data->LexicalHypothesisContainer.insert(_emptyCoverage,
                                                            clsLexicalHypothesisContainer::rootLexicalHypothesis());
        return CoverageContainer;
    }

    void remove(Coverage_t _coverage){
        this->Data->LexicalHypothesisContainer.remove(_coverage);
        if (this->Data->WorstLexicalHypothesis != NULL && this->Data->WorstCoverage == _coverage){
            this->updateWorstNode();
        }
    }

    bool insertNewHypothesis(const Coverage_t& _coverage, clsLexicalHypothesisContainer& _container, clsSearchGraphNode& _node);

    bool mustBePruned(Common::Cost_t _cost) const;
    void pruneAndUpdateWorstNode(const Coverage_t& _coverage, clsLexicalHypothesisContainer& _lexicalHypo, const clsSearchGraphNode &_node);
    size_t totalSearchGraphNodeCount() const{return this->Data->TotalSearchGraphNodeCount;}

public:
    void updateWorstNode();

private:
    QExplicitlySharedDataPointer<clsCardinalityHypothesisContainerData> Data;
    static Common::Configuration::tmplConfigurable<quint8> ReorderingHistogramSize;
};

}
}
}
}

#endif // TARGOMAN_CORE_PRIVATE_SEARCHGRAPHBUILDER_CLSCARDINALITY_H
