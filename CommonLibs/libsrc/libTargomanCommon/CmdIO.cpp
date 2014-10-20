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

#include <QtGlobal>
#include <QString>
#include <link.h>

#include "Macros.h"
#include "CmdIO.h"

typedef std::string (*delGetTargomanLibVersion)();

namespace Targoman {
namespace Common {

int targomanLinkedLibrariesCallback(struct dl_phdr_info *_info, size_t _size, void *_data)
{
    Q_UNUSED(_size)
    Q_UNUSED(_data)
    delGetTargomanLibVersion getTargomanLibVersion;

    QString LibName = _info->dlpi_name;
    if (LibName.isEmpty())
        return 0;

    void* Handle = dlopen (_info->dlpi_name, RTLD_LAZY);
    dlerror();    /* Clear any existing error */
    *(void**) (&getTargomanLibVersion) = dlsym (Handle, "getTargomanLibVersion");

    if (dlerror() != NULL)
        qDebug("System Lib: %s",_info->dlpi_name);
    else if (getTargomanLibVersion)
        qDebug("  Targoman  Lib: %s : %s",_info->dlpi_name, getTargomanLibVersion().c_str());
    else
        qDebug("Loaded System Lib: %s",_info->dlpi_name);

    dlclose(Handle);
    return 0;
}

void dummyPrintLoadedLibs()
{
}

void printLoadedLibs()
{
    dl_iterate_phdr(targomanLinkedLibrariesCallback, NULL);
}

void silent()
{
    OUTPUT_SETTINGS_DEBUG.setLevel(0);
    OUTPUT_SETTINGS_HAPPY.setLevel(0);
    OUTPUT_SETTINGS_INFO.setLevel(0);
    OUTPUT_SETTINGS_WARNING.setLevel(0);
    OUTPUT_SETTINGS_NORMAL.setLevel(0);

//    OUTPUT_SETTINGS_ERROR.setLevel(0); //Error outputs must not be silented
}

bool OUTPUT_SETTINGS_SHOWCOLORED = false;
bool OUTPUT_SETTINGS_ALSO_LOG = false;

clsOutputSettings OUTPUT_SETTINGS_DEBUG;
clsOutputSettings OUTPUT_SETTINGS_INFO;
clsOutputSettings OUTPUT_SETTINGS_WARNING;
clsOutputSettings OUTPUT_SETTINGS_HAPPY;
clsOutputSettings OUTPUT_SETTINGS_ERROR;
clsOutputSettings OUTPUT_SETTINGS_NORMAL;


}
}


