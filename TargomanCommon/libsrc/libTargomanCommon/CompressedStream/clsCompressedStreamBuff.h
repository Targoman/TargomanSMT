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

#ifndef TARGOMAN_COMMON_COMPRESSEDSTREAM_CLSCOMPRESSEDSTREAMBUFF_H
#define TARGOMAN_COMMON_COMPRESSEDSTREAM_CLSCOMPRESSEDSTREAMBUFF_H

#include <iostream>
#include <cstring>
#include <zlib.h>
#include <streambuf>

namespace Targoman {
namespace Common {
namespace CompressedStream {

class clsCompressedStreamBuff : public std::streambuf
{
public:
    clsCompressedStreamBuff();
    int isOpen() { return Opened; }
    bool open(const std::string &_name, std::ios_base::openmode _openMode);
    bool close();
    virtual ~clsCompressedStreamBuff();

    virtual int overflow(int _chars = EOF);
    virtual int underflow();
    virtual int sync();

private:
    int flushBuffer();

private:
    static const int bufferSize = 47+256;   /**< Size of data buff (totals 512 bytes under g++ for CompressedInputStream at the end.)*/
    gzFile           File;                  /**< File handle for compressed file */
    char             buffer[bufferSize];    /**< Data buffer */
    char             Opened;                /**< Open/close state of stream */
    int              Mode;                  /**< I/O mode */


};

}
}
}

#endif // TARGOMAN_COMMON_COMPRESSEDSTREAM_CLSCOMPRESSEDSTREAMBUFF_H
