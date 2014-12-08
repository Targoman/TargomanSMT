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

#ifndef TARGOMAN_COMMON_LOGGER_P_H
#define TARGOMAN_COMMON_LOGGER_P_H

#include <QFile>
#include <QHash>
#include <QMutex>

#include "../Logger.h"

namespace Targoman {
namespace Common {
namespace Private {

class LoggerPrivate{
public:
    LoggerPrivate();
    bool open();
    void rotateLog();

public:
    QFile   LogFile;
    clsLogSettings* LogSettings;
    quint64 MaxFileSize;
    bool GlobalSettings;
    QMutex  mxLog;
    QString ActorUUID;
    QHash<QString, QString> Actors;
};

}
}
}

#endif // TARGOMAN_COMMON_LOGGER_P_H
