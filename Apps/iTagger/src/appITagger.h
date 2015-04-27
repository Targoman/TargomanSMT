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

#ifndef TARGOMAN_APPE4SMT_H
#define TARGOMAN_APPE4SMT_H

#include "libTargomanCommon/clsSafeCoreApplication.h"
#include "libTargomanCommon/CmdIO.h"

namespace Targoman {

TARGOMAN_DEFINE_ENHANCED_ENUM(enuMode,
                              Text2IXML,
                              Normalize,
                              Tokenize,
                              IXML2Text
                              )


class appITagger : public Common::clsSafeCoreApplication
{
public:
    appITagger(int _argc, char **_argv);

public slots:
    void slotExecute();

private:
};

}

#endif // TARGOMAN_APPE4SMT_H
