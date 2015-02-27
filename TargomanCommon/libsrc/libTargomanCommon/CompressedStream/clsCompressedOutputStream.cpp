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

#include "clsCompressedOutputStream.h"
#include "clsCompressedStreamBuff.h"
#include "../exTargomanBase.h"

namespace Targoman {
namespace Common {
namespace CompressedStream {

clsCompressedOutputStream::clsCompressedOutputStream() : std::ostream(0), Buffer(0) {}

clsCompressedOutputStream::clsCompressedOutputStream(const std::string &_name, bool _allowFail):
    std::ostream(0), Buffer(0)
{
    this->open(_name, _allowFail);
}


/**
 * @param _name         File address
 * @param _allowFail    Is it ok to  throw exception if it could not open file or not.
 * @exception throws exception if it could not open file and _allowFail is not true.
 */

void clsCompressedOutputStream::open(const std::string &_name, bool _allowFail)
{
    if (this->Buffer)
        delete this->Buffer;
    if ((_name.rfind(".gz") == _name.length() - 3) || (_name.rfind(".Z") == _name.length() - 3)) {
        clsCompressedStreamBuff *CompressedBuff = new clsCompressedStreamBuff();
        if (!CompressedBuff->open(_name, std::ios::out)) {
            if (!_allowFail)
                throw exTargomanBase("Could not open " + QString::fromStdString(_name) + " for writing");
            setstate(std::ios::failbit);
        }
        this->Buffer = CompressedBuff;
    } else if (_name == "-") {
        this->Buffer = std::cout.rdbuf();
    } else {
        std::filebuf *FileBbuf = new std::filebuf();
        if (!FileBbuf->open(_name.c_str(), std::ios::out)) setstate(std::ios::failbit);
        this->Buffer = FileBbuf;
    }
    rdbuf(Buffer);
}

void clsCompressedOutputStream::close()
{
    if (this->Buffer) {
        if (this->Buffer != std::cout.rdbuf())
            delete this->Buffer;
        this->Buffer = 0;
        this->rdbuf(0);
    }
}

}
}
}
