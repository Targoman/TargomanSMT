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

#ifndef TARGOMAN_CORE_PRIVATE_SEARCHGRAPHBUILDER_CLSHYPOTHESISHOLDER_H
#define TARGOMAN_CORE_PRIVATE_SEARCHGRAPHBUILDER_CLSHYPOTHESISHOLDER_H

#include <QVector>
#include "clsCardinality.h"
#include "clsSearchGraphNode.h"
#include "libTargomanCommon/Types.h"

namespace Targoman{
namespace SMT {
namespace Private{
namespace SearchGraph {

class clsHypothesisHolderData : public QSharedData
{
public:
    clsHypothesisHolderData(size_t _sentenceSize){
        this->EmptyCoverage.resize(_sentenceSize);
        this->EmptyCoverage.fill(0);
    }
    clsHypothesisHolderData(const clsHypothesisHolderData& _other):
        QSharedData(_other),
        CardinalityHypotheses(_other.CardinalityHypotheses)
    {}
    ~clsHypothesisHolderData() {}

public:
    QVector<clsCardinalityHypothesisContainer> CardinalityHypotheses;
    Coverage_t EmptyCoverage;
};

class clsHypothesisHolder
{
public:
    clsHypothesisHolder(size_t _sentenceSize) :
        Data(new clsHypothesisHolderData(_sentenceSize))
    {}
    clsHypothesisHolder(){}

    void clear() {
        this->Data->CardinalityHypotheses.clear();
        this->Data->CardinalityHypotheses.append(clsCardinalityHypothesisContainer::rootCoverageContainer(this->Data->EmptyCoverage));
    }
    void resize(size_t _size) {
            this->Data->CardinalityHypotheses.resize(_size);
    }

    inline const clsSearchGraphNode& getRootNode(){
        return this->Data->CardinalityHypotheses[0][this->Data->EmptyCoverage].bestNode();
    }

    inline clsCardinalityHypothesisContainer& operator [](size_t _index){
        return this->Data->CardinalityHypotheses[_index];
    }

    inline size_t size() const{
        return this->Data->CardinalityHypotheses.size();
    }


private:
    QExplicitlySharedDataPointer<clsHypothesisHolderData> Data;

};

}
}
}
}

#endif // TARGOMAN_CORE_PRIVATE_SEARCHGRAPHBUILDER_CLSHYPOTHESISHOLDER_H
