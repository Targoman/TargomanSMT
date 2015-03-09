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

#ifndef TARGOMAN_APPS_APPTARGOMANSMTCONSOLE_H
#define TARGOMAN_APPS_APPTARGOMANSMTCONSOLE_H

#include "libTargomanCommon/clsSafeCoreApplication.h"

namespace Targoman {
namespace Apps{

class appTargomanSMTConsole : public QObject
{
    Q_OBJECT
public:
    appTargomanSMTConsole() {}

public slots:
    void slotExecute();

private:
};

}
}

#endif // TARGOMAN_APPS_APPTARGOMANSMTCONSOLE_H
