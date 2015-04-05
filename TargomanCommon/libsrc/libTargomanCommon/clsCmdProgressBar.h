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
