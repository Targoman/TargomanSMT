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

#include <QTimer>
#include "appITagger.h"
#include "libTargomanCommon/Macros.h"
#include "libTargomanCommon/Configuration/ConfigManager.h"

const char* LicenseStr =
"%1 Ver: %2 Build %3\n"
"Published under the terms of TCRL(Targoman Community Research License)\n\n"
"%1 [Arguments]\n"
"Arguments: \n";

int main(int _argc, char *_argv[])
{
    Targoman::Common::printLoadedLibs();

    Targoman::appITagger App(_argc, _argv);

    Targoman::Common::Configuration::ConfigManager::instance().init(
                QString(LicenseStr).arg(TARGOMAN_M2STR(PROJ_VERSION)).arg(__DATE__),
                App.arguments()
            );

    QTimer::singleShot(10, &App, SLOT(slotExecute()));

    return App.exec();
}


