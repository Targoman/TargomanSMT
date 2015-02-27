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

#include "clsSafeCoreApplication.h"
#include "exTargomanBase.h"
#include "CmdIO.h"

namespace Targoman{
namespace Common{

clsSafeCoreApplication::clsSafeCoreApplication(int& _argc, char ** _argv) :
    QCoreApplication(_argc, _argv)
{}

/**
 * @brief Overriding Qt notify function. this function catches unhandled thrown exceptions.
 */
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

/**
check assertion file from http://www.dev.voxforge.org/projects/Main/browser/Tags/AudioSegmentation/AudioBook/input_files/g2p/source/Assertions.hh

 */

}
}
