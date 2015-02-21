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

#ifndef TARGOMAN_CORE_PRIVATE_OUTPUTCOMPOSER_CLSOUTPUTCOMPOSER_H
#define TARGOMAN_CORE_PRIVATE_OUTPUTCOMPOSER_CLSOUTPUTCOMPOSER_H

#include "libTargomanCommon/CmdIO.h"

namespace Targoman{
namespace Core {
namespace Private{
namespace OutputComposer {

class clsOutputComposer
{
public:
    clsOutputComposer();
    ~clsOutputComposer(){}

    static void init();
};

}
}
}
}

#endif // TARGOMAN_CORE_PRIVATE_OUTPUTCOMPOSER_CLSOUTPUTCOMPOSER_H
