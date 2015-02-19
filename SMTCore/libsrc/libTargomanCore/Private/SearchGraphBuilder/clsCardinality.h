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


namespace Targoman{
namespace Core {
namespace Private{
namespace SearchGraphBuilder {

typedef QBitArray Coverage_t;
typedef QHash<Coverage_t, clsLexicalHypothesis> LexicalHypothesisContainer_t;

class clsCardinalityData : public QSharedData
{
    struct stuWorstCosSearchGraphNode{
        LexicalHypothesisContainer_t::Iterator ConatinerIter;
        QList<clsSearchGraphNode>::Iterator    NodeIter;
        Common::Cost_t                         Cost;
    };

public:
    clsCardinalityData(){
        this->WorstNode.Cost = INFINITY;
        this->TotalSearchGraphNodeCount = 0;
    }

    clsCardinalityData(const clsCardinalityData& _other) :
        QSharedData(_other),
        LexicalHypothesisContainer(_other.LexicalHypothesisContainer)
    {}
    ~clsCardinalityData(){TargomanDebugLine}

public:
    LexicalHypothesisContainer_t LexicalHypothesisContainer;
    size_t                       TotalSearchGraphNodeCount;
    stuWorstCosSearchGraphNode   WorstNode;
};

class clsCardinality
{
public:
    clsCardinality();
    ~clsCardinality(){TargomanDebugLine}

    inline clsLexicalHypothesis& operator [] (const Coverage_t& _coverage){
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
    }

    bool mustBePruned(Common::Cost_t _cost);
    void updateWorstNode(const Coverage_t& _coverage, const clsSearchGraphNode& _node);

public:
    static Common::Configuration::tmplConfigurable<quint8> ReorderingHistogramSize;

private:
    QExplicitlySharedDataPointer<clsCardinalityData> Data;
};

}
}
}
}

#endif // TARGOMAN_CORE_PRIVATE_SEARCHGRAPHBUILDER_CLSCARDINALITY_H
