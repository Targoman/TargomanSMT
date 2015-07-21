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

#ifndef CLSCMDPROGRESSBAR_H
#define CLSCMDPROGRESSBAR_H

#include "libTargomanCommon/CmdIO.h"

namespace Targoman {
namespace Common {

/**
 * @brief This class helps us to show a progress bar in command line.
 */
class clsCmdProgressBar
{
public:
    clsCmdProgressBar();
    clsCmdProgressBar(const QString& _message, quint64 _maximum = 0);
    ~clsCmdProgressBar();
    void setValue(quint32 _value);
    void reset(const QString& _message, qint64 _maximum);
    void finalize(bool _change2full, qint64 _value = -1);

private:
    qint64  Maximum;                /** Maximum length of progress bar */
    QString Message;                /** Message to show beside progress bar */
    quint8   LastProgressValue;     /** Last value of progress bar */
    bool    Reseted;                /** whether progress bar has been reseted or not. */
    bool    WasShown;               /** whether progress bar has been shown or not. */
    QTime   LastShowTime;           /** The time of last show of progress bar. */
};

}
}
#endif // CLSCMDPROGRESSBAR_H
