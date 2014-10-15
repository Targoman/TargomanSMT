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

#ifdef TARGOMAN_DEFAULT_DEBUG_LEVEL
quint8  TARGOMAN_DEBUG_LEVEL=TARGOMAN_DEFAULT_DEBUG_LEVEL;
#else
quint8  TARGOMAN_DEBUG_LEVEL=10;
#endif

#ifdef TARGOMAN_DEFAULT_ERROR_LEVEL
quint8  TARGOMAN_ERROR_LEVEL=TARGOMAN_DEFAULT_ERROR_LEVEL;
#else
quint8  TARGOMAN_ERROR_LEVEL=10;
#endif
#ifdef TARGOMAN_DEFAULT_WARNINIG_LEVEL
quint8  TARGOMAN_WARNING_LEVEL=TARGOMAN_DEFAULT_WARNINIG_LEVEL;
#else
quint8  TARGOMAN_WARNING_LEVEL=10;
#endif
#ifdef TARGOMAN_DEFAULT_INFO_LEVEL
quint8  TARGOMAN_INFO_LEVEL=TARGOMAN_DEFAULT_INFO_LEVEL;
#else
quint8  TARGOMAN_INFO_LEVEL=10;
#endif

}
}


