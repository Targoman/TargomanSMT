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

#include "clsSafeCoreApplication.h"
#include "exTargomanBase.h"
#include "Debug.h"

namespace Targoman{
namespace Common{

clsSafeCoreApplication::clsSafeCoreApplication(int& _argc, char ** _argv) :
    QCoreApplication(_argc, _argv)
{}

bool clsSafeCoreApplication::notify(QObject * _object, QEvent * _ev)
{
    try{
        return QCoreApplication::notify(_object, _ev);
    } catch (Targoman::Common::exTargomanBase &ex) {
        TargomanError("Fatal Error at Application scope: %s", qPrintable(ex.what()));
    } catch (...) {
        TargomanError("Fatal Unrecognized Exception occured at application scope. Seems like a bug.");
    }

    return false;
}

}
}
