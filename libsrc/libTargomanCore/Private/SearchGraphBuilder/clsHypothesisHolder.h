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
 */

#ifndef TARGOMAN_CORE_PRIVATE_SEARCHGRAPHBUILDER_CLSHYPOTHESISHOLDER_H
#define TARGOMAN_CORE_PRIVATE_SEARCHGRAPHBUILDER_CLSHYPOTHESISHOLDER_H

#include <QList>
#include "clsCoverageContainer.h"

namespace Targoman{
namespace Core {
namespace Private{
namespace SearchGraphBuilder {

class clsHypothesisHolder
{
public:
    clsHypothesisHolder();

private:
    QList<clsCoverageContainer> CardinalityContainer;
};

}
}
}
}

#endif // TARGOMAN_CORE_PRIVATE_SEARCHGRAPHBUILDER_CLSHYPOTHESISHOLDER_H
