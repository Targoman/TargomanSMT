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
    qint64  Maximum;
    QString Message;
    quint8   LastProgressValue;
    bool    Reseted;
    bool    WasShown;
    QTime   LastShowTime;
};

}
}
#endif // CLSCMDPROGRESSBAR_H
