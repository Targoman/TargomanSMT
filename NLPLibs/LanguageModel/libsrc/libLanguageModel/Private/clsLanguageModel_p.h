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

#ifndef TARGOMAN_NLPLIBS_PRIVATE_CLSLANGUAGEMODEL_P_H
#define TARGOMAN_NLPLIBS_PRIVATE_CLSLANGUAGEMODEL_P_H

#include "libTargomanCommon/Macros.h"
#include "../clsLanguageModel.h"
#include "intfBaseModel.hpp"

namespace Targoman {
namespace NLPLibs {
namespace Private {

class clsLanguageModelPrivate
{
public:
    clsLanguageModelPrivate();
    bool isBinary(const QString& _file);

public:
    quint8                Order;     /**< Order of NGram */
    intfBaseModel*        Model;    /**< instance of NGram Model */
    bool                  Initialized;
};


}
}
}
#endif // TARGOMAN_NLPLIBS_PRIVATE_CLSLANGUAGEMODEL_P_H
