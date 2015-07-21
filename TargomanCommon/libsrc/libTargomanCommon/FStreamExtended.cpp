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
 * @author Behrooz Vedadian <vedadian@targoman.com>
 */

#include "FStreamExtended.h"

namespace Targoman {
namespace Common {

/**
 * @brief   This is a specific overloading of write function for QString which first writes string size
 *          and then writes the string.
 */
template <> void clsOFStreamExtended::write(QString _value){
    QByteArray StringData = _value.toUtf8();
    this->write(StringData.size());
    this->write(StringData.constData(), StringData.size());
}


/**
 * @brief   This is a specific overloading of read function for QString which first reads string size
 *          and then reads the string.
 */
template <>  QString clsIFStreamExtended::read(){
    QByteArray Data(this->read<int>(),Qt::Uninitialized);
    this->read(Data.data(), Data.size());
    return QString::fromUtf8(Data);
}

}
}
