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

#include "clsCmdProgressBar.h"
#include "libTargomanCommon/CmdIO.h"
#include <iostream>
#include <sys/ioctl.h>

using namespace std;

namespace Targoman {
namespace Common {

/**
 * @brief Default constructor of this class makes #WasShown false and sets #Message to
 * "NOT-INITIALIZED" and set #Maximum to -1.
 */
Targoman::Common::clsCmdProgressBar::clsCmdProgressBar()
{
    this->WasShown = false;
    this->reset("NOT-INITIALIZED", -1);
}

/**
 * @brief This constructor of class sets #Message and #Maximum from input argument and makes #WasShown
 *  true.
 * @param _message Message string.
 * @param _maximum Maximum value.
 */
clsCmdProgressBar::clsCmdProgressBar(const QString &_message, quint64 _maximum)
{
    this->WasShown = false;
    this->reset(_message, _maximum);
}

/**
 * @brief destructor of this class calls finalize() function of this class.
 */
clsCmdProgressBar::~clsCmdProgressBar()
{
    this->finalize(false);
}

/**
 * @brief This function sets value of progress bar.
 *
 * If the #Maximum value of progress bar is zero, it should show marquee. in marquee mode it shows
 * an orbiting line.
 * If the #Maximum value of progress bar is greater than zero it shows a progress bar which have some
 * number of "#" proportional to input _value and command window size.
 *
 * @param _value
 */
void clsCmdProgressBar::setValue(quint32 _value)
{
    if (TARGOMAN_IO_SETTINGS.Info.canBeShown(6) == false)
        return;

    if (this->Maximum == 0){//Show marquee
        if (this->LastShowTime.elapsed() >= 200){
            char ProgressIndicator;
            switch(this->LastProgressValue % 8){
            case 0: ProgressIndicator = '|';break;
            case 1: ProgressIndicator = '/';break;
            case 2: ProgressIndicator = '-';break;
            case 3: ProgressIndicator = '\\';break;
            case 4: ProgressIndicator = '|';break;
            case 5: ProgressIndicator = '/';break;
            case 6: ProgressIndicator = '-';break;
            case 7: ProgressIndicator = '\\';break;
            default:ProgressIndicator = '|';break;
            }
            struct winsize w;
            ioctl(1, TIOCGWINSZ, &w);

            QString ProgressPrefix = this->Message + QString(" [%1][%2][").arg(_value).arg(ProgressIndicator);
            quint16 ProgressBarMaxWidth = w.ws_col - ProgressPrefix.size() - 10;
            cerr<<QString("%1%2]\r").arg(ProgressPrefix).arg(
                   QString(this->LastProgressValue % (ProgressBarMaxWidth - 3), ' ')+"###",-ProgressBarMaxWidth).toUtf8().constData()<<flush;
            ++this->LastProgressValue;
            this->LastShowTime.restart();
            this->WasShown = true;
        }

        return;
    }


    if(this->Maximum < 0)
        return;

    if (this->Reseted){
        this->finalize(false);
        this->Reseted = false;
    }

    if (_value >= this->Maximum)
        this->finalize(true);
    else {
        quint16 ProgressVal = ((double)_value / (double)this->Maximum) * 100;
        if (this->LastProgressValue != ProgressVal &&
            this->LastShowTime.elapsed() >= 200
                ){
            struct winsize w;
            ioctl(1, TIOCGWINSZ, &w);
            QString ProgressPrefix = this->Message + QString(" [%1][%2%][").arg(_value).arg(ProgressVal,3);
            quint16 ProgressBarMaxWidth = w.ws_col - ProgressPrefix.size() - 10;
            cerr<<QString("%1%2]\r").arg(ProgressPrefix).arg(
                   QString((ProgressVal*ProgressBarMaxWidth)/100, '#'),-ProgressBarMaxWidth).toUtf8().constData()<<flush;
            this->LastProgressValue = ProgressVal;
            this->LastShowTime.restart();
        }
        this->WasShown = true;
    }
}


/**
 * @brief Resets #Message value and #Maximum value of class.
 * @param _message  message string.
 * @param _maximum  maximum value.
 */
void clsCmdProgressBar::reset(const QString &_message, qint64 _maximum)
{
    this->Reseted = true;
    this->Maximum = _maximum;
    this->LastProgressValue = 0;
    this->Message = _message;
    this->LastShowTime.restart();
    this->setValue(0);
}


/**
 * @brief finalizes progress bar and show _finalValue% progress.
 * @param _change2full whether show _finalValue% progress or not
 * @param _finalValue final value of progress bar.
 */
void clsCmdProgressBar::finalize(bool _change2full, qint64 _finalValue)
{
    if (this->WasShown == false)
        return;
    if (_change2full){
        struct winsize w;
        ioctl(1, TIOCGWINSZ, &w);
        QString ProgressPrefix = this->Message + QString(" [%1][100%][").arg(_finalValue == -1 ? this->Maximum : _finalValue);
        quint16 ProgressBarMaxWidth = w.ws_col - ProgressPrefix.size() - 10;
        cerr<<(ProgressPrefix +
               QString(ProgressBarMaxWidth, '#') + "]").toUtf8().constData()<<flush;
    }

    cerr<<endl;
    this->WasShown = false;
}

}
}
