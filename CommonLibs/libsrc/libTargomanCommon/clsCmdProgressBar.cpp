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

#include "clsCmdProgressBar.h"
#include "libTargomanCommon/CmdIO.h"
#include <iostream>
#include <sys/ioctl.h>

using namespace std;

namespace Targoman {
namespace Common {

clsCmdProgressBar::clsCmdProgressBar(const QString &_message, quint32 _maximum)
{
    this->Maximum = _maximum;
    this->LastProgressValue = -1;
    this->Message = _message;
    this->setValue(0);
}

void clsCmdProgressBar::setValue(quint32 _value)
{

    if (TARGOMAN_IO_SETTINGS.Info.canBeShown(3) == false)
        return;

    struct winsize w;
    ioctl(1, TIOCGWINSZ, &w);
    quint16 ProgressBarMaxWidth = w.ws_col - this->Message.size() - 10;

    if (_value >= this->Maximum){
        cout<<(this->Message + " [100%][" + QString(ProgressBarMaxWidth,'#') + "]").toUtf8().constData()<<endl;
    }else {
        quint16 ProgressVal = ((double)_value / (double)this->Maximum) * 1000;
        if (true || this->LastProgressValue != ProgressVal){
            cout<<(this->Message + QString(" [%1%][%2]\r").arg(
                       ProgressVal/10,3).arg(
                       QString(ProgressVal*ProgressBarMaxWidth/1000, '#'),-ProgressBarMaxWidth)
                   ).toUtf8().constData()<<flush;
            this->LastProgressValue = ProgressVal;
        }
    }
}

}
}
