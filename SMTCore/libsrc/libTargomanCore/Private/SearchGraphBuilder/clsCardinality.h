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

bool operator<(const QBitArray& a, const QBitArray& b);

namespace Targoman{
namespace Core {
namespace Private{
namespace SearchGraphBuilder {

typedef QBitArray Coverage_t;
typedef QMap<Coverage_t, clsLexicalHypothesis> LexicalHypothesisContainer_t;

class clsCardinalityData : public QSharedData
{
public:
    clsCardinalityData(){
        this->WorstLexicalHypothesis = NULL;
        this->TotalSearchGraphNodeCount = 0;
    }

    clsCardinalityData(const clsCardinalityData& _other) :
        QSharedData(_other),
        LexicalHypothesisContainer(_other.LexicalHypothesisContainer)
    {}
    ~clsCardinalityData(){}

public:
    LexicalHypothesisContainer_t LexicalHypothesisContainer;
    size_t                       TotalSearchGraphNodeCount;
    clsLexicalHypothesis*        WorstLexicalHypothesis;
    Coverage_t                   WorstCoverage;
};

class clsCardinality
{
public:
    clsCardinality();
    ~clsCardinality(){}

    inline clsLexicalHypothesis& operator [] (const Coverage_t& _coverage){
        /*if(this->Data->LexicalHypothesisContainer.contains(_coverage) == false){
            TargomanDebug(1,"Creating new LexHypo for coverage = " + bitArray2Str(_coverage));
        }*/
        return this->Data->LexicalHypothesisContainer[_coverage];
    }

    inline size_t isEmpty(){
        return this->Data->LexicalHypothesisContainer.isEmpty();
    }

    inline LexicalHypothesisContainer_t& lexicalHypotheses(){
        return this->Data->LexicalHypothesisContainer;
    }

    inline static clsCardinality rootCoverageContainer(Coverage_t _emptyCoverage){
        clsCardinality CoverageContainer;
        CoverageContainer.Data->LexicalHypothesisContainer.insert(_emptyCoverage,
                                                            clsLexicalHypothesis::rootLexicalHypothesis());
        return CoverageContainer;
    }

    void remove(Coverage_t _coverage){
        this->Data->LexicalHypothesisContainer.remove(_coverage);
        if (this->Data->WorstLexicalHypothesis != NULL && this->Data->WorstCoverage == _coverage){
            this->updateWorstNode();
        }
    }

    void insertNewHypothesis(const Coverage_t& _coverage, clsLexicalHypothesis& _container, clsSearchGraphNode& _node);

    bool mustBePruned(Common::Cost_t _cost) const;
    void pruneAndUpdateWorstNode(const Coverage_t& _coverage, clsLexicalHypothesis& _lexicalHypo, const clsSearchGraphNode &_node);
    size_t totalSearchGraphNodeCount() const{return this->Data->TotalSearchGraphNodeCount;}

public:
    static Common::Configuration::tmplConfigurable<quint8> ReorderingHistogramSize;

    void updateWorstNode();
private:
    QExplicitlySharedDataPointer<clsCardinalityData> Data;
};

}
}
}
}

#endif // TARGOMAN_CORE_PRIVATE_SEARCHGRAPHBUILDER_CLSCARDINALITY_H
