/******************************************************************************
 * Targoman: A robust Statistical Machine Translation framework               *
 *                                                                            *
 * Copyright 2014-2015 by ITRC <http://itrc.ac.ir>                            *
 *                                                                            *
 * This file is part of Targoman.                                             *
 *                                                                            *
 * Targoman is free software: you can redistribute it and/or modify           *
 * it under the terms of the GNU Lesser General Public License as published   *
 * by the Free Software Foundation, either version 3 of the License, or       *
 * (at your option) any later version.                                        *
 *                                                                            *
 * Targoman is distributed in the hope that it will be useful,                *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of             *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              *
 * GNU Lesser General Public License for more details.                        *
 * You should have received a copy of the GNU Lesser General Public License   *
 * along with Targoman. If not, see <http://www.gnu.org/licenses/>.           *
 *                                                                            *
 ******************************************************************************/
/**
 * @author S. Mohammad M. Ziabary <ziabary@targoman.com>
 */

#include "clsSafeCoreApplication.h"
#include "exTargomanBase.h"
#include "CmdIO.h"

namespace Targoman{
namespace Common{

clsSafeCoreApplication::clsSafeCoreApplication(int _argc, char ** _argv) :
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
