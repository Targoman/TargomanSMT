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

/**
 * @brief prints a list of loaded system and Targoman libraries.
 */
void printLoadedLibs()
{
    if (TARGOMAN_IO_SETTINGS.Debug.canBeShown(5) == false)
        return;

    dl_iterate_phdr(targomanLinkedLibrariesCallback, NULL);
}

clsIOSettings TARGOMAN_IO_SETTINGS;


}
}


