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

class clsHypothesisHolder
{
public:
    clsHypothesisHolder();
    void clear();
    void resize(size_t _size);

    inline const clsSearchGraphNode& getRootNode() const{
        return this->Cardinalities[0][clsCardinality::EmptyCoverage].bestNode();
    }

    inline clsCardinality& operator [](size_t _index){
        return this->Cardinalities[_index];
    }

    /*inline const QVector<clsCoverageContainer>& cardinalityContainer(){
        return this->CardinalityContainer;
    }*/

private:
    QVector<clsCardinality> Cardinalities;

};

}
}
}
}

#endif // TARGOMAN_CORE_PRIVATE_SEARCHGRAPHBUILDER_CLSHYPOTHESISHOLDER_H
