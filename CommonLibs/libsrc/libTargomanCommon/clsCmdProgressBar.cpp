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

Targoman::Common::clsCmdProgressBar::clsCmdProgressBar()
{
    this->reset("NOT-INITIALIZED", 1);
}

clsCmdProgressBar::clsCmdProgressBar(const QString &_message, quint32 _maximum)
{
    this->reset(_message, _maximum);
}

void clsCmdProgressBar::setValue(quint32 _value)
{
    if (TARGOMAN_IO_SETTINGS.Info.canBeShown(3) == false)
        return;

    if (this->Reseted){
        this->Reseted = false;
        cout<<endl;
    }

    if (_value >= this->Maximum){
        struct winsize w;
        ioctl(1, TIOCGWINSZ, &w);
        quint16 ProgressBarMaxWidth = w.ws_col - this->Message.size() - 20;
        cout<<(this->Message + QString(" [%1][100%][").arg(_value) +
               QString(ProgressBarMaxWidth,'#') + "]").toUtf8().constData()<<endl;
    }else {
        quint16 ProgressVal = ((double)_value / (double)this->Maximum) * 100;
        if (this->LastProgressValue != ProgressVal){
            struct winsize w;
            ioctl(1, TIOCGWINSZ, &w);
            quint16 ProgressBarMaxWidth = w.ws_col - this->Message.size() - 20;
            cout<<(this->Message + QString(" [%1][%2%][%3]\r").arg(_value).arg(
                       ProgressVal,3).arg(
                       QString(ProgressVal*(ProgressBarMaxWidth)/100, '#'),-ProgressBarMaxWidth)
                   ).toUtf8().constData()<<flush;
            this->LastProgressValue = ProgressVal;
        }
    }
}

void clsCmdProgressBar::reset(const QString &_message, quint32 _maximum)
{
    this->Reseted = true;
    this->Maximum = _maximum;
    this->LastProgressValue = -1;
    this->Message = _message;
    this->setValue(0);
}

}
}