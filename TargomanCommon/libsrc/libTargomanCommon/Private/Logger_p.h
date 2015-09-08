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

#ifndef TARGOMAN_COMMON_PRIVATE_LOGGER_P_H
#define TARGOMAN_COMMON_PRIVATE_LOGGER_P_H

#include <QFile>
#include <QHash>
#include <QMutex>

#include "../Logger.h"

namespace Targoman {
namespace Common {
namespace Private {
/**
 * @brief The pointer of this class is used as "pPrivate" data variable of Logger class.
 * This class is used as an integral place for registering all users by their UUIDs.
 * This class also helps to write outout logs safely.
 */
class LoggerPrivate{
public:

    /**
     * @brief Generates an array of clsLogSettings for different types of log settings such as
     * "Info", "Warning", "Error", "Debug" and "Happy".
     */
    LoggerPrivate();

    /**
     * @brief Opens logs file in appending mode.
     * @return returns true if succesfull, returns false if file is not writable.
     */
    bool open();

    /**
     * @brief Zips log file if size of log file is bigger MaxFileSize
     * @exception Throws exception if OS type is not linux
     */
    void rotateLog();

public:
    QFile   LogFile;
    quint64 MaxFileSize;
    char GlobalSettings;            /**< first bit of this boolean defines whether it is active or not and second bit defines whether it is visible or not*/
    QMutex  mxLog;                  /**< a mutex for insuring unique access for logging outputs for safty */
    QString ActorUUID;
    QHash<QString, QString> Actors; /**< a map for registring actors. Key: actor_name, Value: actor_UUID */

    /**
     * @todo add configuration settings
     */
};

}
}
}

#endif // TARGOMAN_COMMON_PRIVATE_LOGGER_P_H
