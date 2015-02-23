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
namespace Core {
namespace Private{
namespace SearchGraphBuilder {

class clsHypothesisHolderData : public QSharedData
{
public:
    clsHypothesisHolderData(size_t _sentenceSize){
        this->EmptyCoverage.resize(_sentenceSize);
        this->EmptyCoverage.fill(0);
    }
    clsHypothesisHolderData(const clsHypothesisHolderData& _other):
        QSharedData(_other),
        Cardinalities(_other.Cardinalities)
    {}
    ~clsHypothesisHolderData() {}

public:
    QVector<clsCardinality> Cardinalities;
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
        this->Data->Cardinalities.clear();
        this->Data->Cardinalities.append(clsCardinality::rootCoverageContainer(this->Data->EmptyCoverage));
    }
    void resize(size_t _size) {
            this->Data->Cardinalities.resize(_size);
    }

    inline const clsSearchGraphNode& getRootNode(){
        return this->Data->Cardinalities[0][this->Data->EmptyCoverage].bestNode();
    }

    inline clsCardinality& operator [](size_t _index){
        return this->Data->Cardinalities[_index];
    }

    /*inline const QVector<clsCoverageContainer>& cardinalityContainer(){
        return this->CardinalityContainer;
    }*/

private:
    QExplicitlySharedDataPointer<clsHypothesisHolderData> Data;

};

}
}
}
}

#endif // TARGOMAN_CORE_PRIVATE_SEARCHGRAPHBUILDER_CLSHYPOTHESISHOLDER_H
