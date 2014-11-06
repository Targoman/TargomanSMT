/*************************************************************************
 * Copyright © 2012-2014, Targoman.com
 *
 * Published under the terms of TCRL(Targoman Community Research License)
 * You can find a copy of the license file with distributed source or
 * download it from http://targoman.com/License.txt
 * 
 *************************************************************************/
/**
 @author S. Mohammad M. Ziabary <smm@ziabary.com>
 */

#ifndef TARGOMAN_NLPLIBS_PRIVATE_CLSNGRAM_H
#define TARGOMAN_NLPLIBS_PRIVATE_CLSNGRAM_H
#include <QHash>
#include "../Definitions.h"

namespace Targoman {
namespace NLPLibs {
namespace Private {

typedef QList<WordIndex_t> NGram_t;

}
}
}

uint qHash(const Targoman::NLPLibs::Private::NGram_t & _key);

#endif // TARGOMAN_NLPLIBS_PRIVATE_CLSNGRAM_H
