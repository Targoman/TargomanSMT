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

#ifndef TARGOMAN_COMMON_COMPRESSEDSTREAM_CLSCOMPRESSEDINPUTSTREAM_H
#define TARGOMAN_COMMON_COMPRESSEDSTREAM_CLSCOMPRESSEDINPUTSTREAM_H

// standard C++ with new header file names and std:: namespace
#include <iostream>
#include <fstream>

namespace Targoman {
namespace Common {
/**
 * @brief A usefull module to read and write gzip files as stream
 */
namespace CompressedStream {

// ----------------------------------------------------------------------------
// Internal classes to implement CompressedStream. See below for user classes.
// ----------------------------------------------------------------------------

class clsCompressedStreamBuff;

/**
 * @brief The clsCompressedInputStream class is analogously to ifstream. They read and write files based on the gz*
 * function interface of the zlib. Files are compatible with gzip compression.
 */
class clsCompressedInputStream : public std::istream
{
public:
    clsCompressedInputStream();
    clsCompressedInputStream(const std::string &_name, bool _allowFail=false);
    ~clsCompressedInputStream() { close(); }
    void open(const std::string &_name, bool _allowFail=false);
    void close();
    bool isOpen() const { return Buffer; }

private:
std::streambuf *Buffer;

};

}
}
}

#endif // TARGOMAN_COMMON_COMPRESSEDSTREAM_CLSCOMPRESSEDINPUTSTREAM_H
