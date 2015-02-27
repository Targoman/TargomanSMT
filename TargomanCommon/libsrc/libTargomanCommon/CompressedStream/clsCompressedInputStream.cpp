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
