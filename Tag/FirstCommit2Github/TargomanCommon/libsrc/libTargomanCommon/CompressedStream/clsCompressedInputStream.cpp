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

#include "clsCompressedInputStream.h"
#include "clsCompressedStreamBuff.h"
#include "../exTargomanBase.h"

namespace Targoman {
namespace Common {
namespace CompressedStream {

clsCompressedInputStream::clsCompressedInputStream() : std::istream(0), Buffer(0) {}

clsCompressedInputStream::clsCompressedInputStream(const std::string &_name, bool _allowFail):
    std::istream(0), Buffer(0)
{
    open(_name, _allowFail);
}

/**
 * @param _name         File address
 * @param _allowFail    Is it ok to  throw exception if it could not open file or not.
 * @exception throws exception if it could not open file and _allowFail is not true.
 */

void clsCompressedInputStream::open(const std::string &_name, bool _allowFail)
{
    if (this->Buffer) delete this->Buffer;
    if (_name == "-") {
        this->Buffer = std::cin.rdbuf();
    } else {
        clsCompressedStreamBuff *CompressedBuffer = new clsCompressedStreamBuff();
        if (!CompressedBuffer->open(_name, std::ios::in)) {
            if (!_allowFail)
                throw exTargomanBase("Could not open \"" + QString::fromStdString(_name) + "\" for reading");
            setstate(std::ios::failbit);
        }
        this->Buffer = CompressedBuffer;
    }
    this->rdbuf(this->Buffer);
}
void clsCompressedInputStream::close()
{
    if (this->Buffer) {
        if (this->Buffer != std::cin.rdbuf()) delete this->Buffer;
        this->Buffer = 0;
        rdbuf(0);
    }
}
}
}
}
