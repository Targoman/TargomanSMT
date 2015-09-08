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

#ifndef TARGOMAN_COMMON_CONSTANTS_H
#define TARGOMAN_COMMON_CONSTANTS_H

#include "libTargomanCommon/Types.h"
#include "libTargomanCommon/Macros.h"

namespace Targoman {
namespace Common {

struct Constants
{
    static const LogP_t         LogP_Zero;                  /** log(0) = -Infinity */
    static const LogP_t         LogP_Inf;                   /** log(Inf) = Infinity */
    static const LogP_t         LogP_One;                   /** log(1) = 0 */
    static const quint32        MaxFileIOBytes;             /** 8388608 */
    static const WordIndex_t    SrcVocabUnkWordIndex;       /** 0 */
};

TARGOMAN_DEFINE_ENUM(enuPathAccess,
                     Readable   = 0x01,
                     Writeatble = 0x02,
                     Executable = 0x04,
                     File       = 0x08,
                     Dir        = 0x10
                    );

}
}

#endif // TARGOMAN_COMMON_CONSTANTS_H
