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
 @author Behrooz Vedadian <vedadian@gmail.com>
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
