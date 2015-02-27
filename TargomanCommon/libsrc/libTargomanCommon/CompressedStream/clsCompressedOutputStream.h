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

#ifndef TARGOMAN_COMMON_COMPRESSEDSTREAM_CLSCOMPRESSEDOUTPUTSTREAM_H
#define TARGOMAN_COMMON_COMPRESSEDSTREAM_CLSCOMPRESSEDOUTPUTSTREAM_H

// standard C++ with new header file names and std:: namespace
#include <iostream>
#include <fstream>

namespace Targoman {
namespace Common {
namespace CompressedStream {

// ----------------------------------------------------------------------------
// Internal classes to implement CompressedStream. See below for user classes.
// ----------------------------------------------------------------------------

class clsCompressedStreamBuff;


/**
 * @brief The clsCompressedOutputStream class is analogously to ofstream. They read and write files based on the gz*
 * function interface of the zlib. Files are compatible with gzip compression.
 */

class clsCompressedOutputStream : public std::ostream
{
public:
    clsCompressedOutputStream();
    clsCompressedOutputStream(const std::string &_name, bool _allowFail=false);
    ~clsCompressedOutputStream() { close(); }
    void open(const std::string &_name, bool _allowFail=false);
    void close();
    bool isOpen() const { return Buffer; }

private:
    std::streambuf *Buffer;

};

}
}
}

#endif // TARGOMAN_COMMON_COMPRESSEDSTREAM_CLSCOMPRESSEDOUTPUTSTREAM_H
