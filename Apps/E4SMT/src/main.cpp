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
 @author S. Mohammad M. Ziabary <ziabary@targoman.com>
 */

#include <QCoreApplication>
#include <QTimer>
#include "libTargomanCommon/Macros.h"
#include "libTargomanCommon/Configuration/ConfigManager.h"

#include "appE4SMT.h"

const char* LicenseStr =
"%1 Ver: %2 Build %3\n"
"Published under the terms of GNU Lesser General Public License version 3\n\n"
"%1 [Arguments]\n"
"Arguments: \n";

using namespace Targoman;
using namespace Targoman::Common;
using namespace Targoman::Apps;

int main(int _argc, char *_argv[])
{
    try{
        QCoreApplication App(_argc, _argv);

        Configuration::ConfigManager::instance().init(
                    QString(LicenseStr).arg(_argv[0]).arg(TARGOMAN_M2STR(PROJ_VERSION)).arg(__DATE__),
                    App.arguments().mid(1)
                    );

        QTimer::singleShot(10, new appE4SMT, SLOT(slotExecute()));

        return App.exec();
    }catch(exTargomanBase& e){
        TargomanError(e.what());
    }catch (std::exception &e){
        TargomanError(e.what());
    }catch(...){
        TargomanError("FATAL Unrecognized exception");
    }
    return -1;
}


