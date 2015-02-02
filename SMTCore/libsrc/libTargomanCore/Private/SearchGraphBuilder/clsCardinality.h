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

class clsCardinality
{
public:
    clsCardinality();

    inline const clsLexicalHypothesis& operator [] (const Coverage_t& _coverage){
        return this->LexicalHypothesisContainer.value(_coverage/*, TODO default*/);
    }

    inline size_t isEmpty(){
        return this->LexicalHypothesisContainer.isEmpty();
    }

    inline const LexicalHypothesisContainer_t& lexicalHypotheses(){
        return this->LexicalHypothesisContainer;
    }

    inline static clsCardinality rootCoverageContainer(){
        clsCardinality CoverageContainer;
        CoverageContainer.LexicalHypothesisContainer.insert(clsCardinality::EmptyCoverage,
                                                            clsLexicalHypothesis::rootLexicalHypothesis());
        return CoverageContainer;

        //TODO OJO
    }

    void remove(Coverage_t _coverage){
        this->LexicalHypothesisContainer.remove(_coverage);
    }

    void performReorderingPruning();
    bool mustBePruned(Common::Cost_t _cost) const;
    clsLexicalHypothesis& getOrCreateLexicalHypothesisContainer(Coverage_t _coverage){
        return this->LexicalHypothesisContainer[_coverage];
    }

public:
    static Coverage_t EmptyCoverage;

private:
    LexicalHypothesisContainer_t LexicalHypothesisContainer;

    //LexicalHypothesisContainerMap lexicalHypothesisContainerMap_;
  //  size_t ReorderingHistogramSize;
  //  size_t LexicalHistogramSize;
};

}
}
}
}

#endif // TARGOMAN_CORE_PRIVATE_SEARCHGRAPHBUILDER_CLSCARDINALITY_H
