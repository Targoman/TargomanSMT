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
    clsCmdProgressBar(const QString& _message, quint32 _maximum);
    void setValue(quint32 _value);
    void reset(const QString& _message, quint32 _maximum);

private:
    quint32 Maximum;
    QString Message;
    qint8   LastProgressValue;
    bool    Reseted;
};

}
}
#endif // CLSCMDPROGRESSBAR_H
