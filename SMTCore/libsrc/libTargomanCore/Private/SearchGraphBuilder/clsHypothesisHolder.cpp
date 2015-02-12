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

#include "clsHypothesisHolder.h"
#include "libTargomanCommon/exTargomanBase.h"

namespace Targoman{
namespace Core {
namespace Private{
namespace SearchGraphBuilder {

clsHypothesisHolder::clsHypothesisHolder()
{
}

void clsHypothesisHolder::clear()
{
    this->Data->Cardinalities.clear();
    this->Data->Cardinalities.append(clsCardinality::rootCoverageContainer());

    //TODO
}

void clsHypothesisHolder::resize(size_t _size)
{
    this->Data->Cardinalities.resize(_size);
//TODO
    throw Common::exTargomanMustBeImplemented("");
}

}
}
}
}
