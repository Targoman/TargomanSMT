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

#include "clsCompressedStreamBuff.h"

namespace Targoman {
namespace Common {
namespace CompressedStream {

clsCompressedStreamBuff::clsCompressedStreamBuff():
    Opened(0){
    setp( buffer, buffer + (bufferSize-1));
    setg( buffer + 4,     // beginning of putback area
          buffer + 4,     // read position
          buffer + 4);    // end position
    // ASSERT: both input & output capabilities will not be used together
}

bool clsCompressedStreamBuff::open(const std::string &_name, std::ios_base::openmode _openMode)
{
    // no append nor read/write mode
    /*    require(!(mode & std::ios::ate));
    require(!(mode & std::ios::app));
    require(!((mode & std::ios::in) && (mode & std::ios::out)));
*/
    if (this->isOpen())
        return false;

    char  Fmode[10];
    char* Fmodeptr = Fmode;
    if (_openMode & std::ios::in)
        *Fmodeptr++ = 'r';
    if (_openMode & std::ios::out)
        *Fmodeptr++ = 'w';
    *Fmodeptr++ = 'b';
    *Fmodeptr = '\0';

    this->File = gzopen(_name.c_str(), Fmode);
    if (this->File == NULL)
        return false;

    this->Opened = 1;
    this->Mode = _openMode;
    return true;
}

bool clsCompressedStreamBuff::close()
{
    if (this->isOpen()) {
        this->sync();
        this->Opened = 0;
        if (gzclose(this->File) == Z_OK)
            return true;
    }
    return false;
}

clsCompressedStreamBuff::~clsCompressedStreamBuff()
{
    this->close();
}

int clsCompressedStreamBuff::overflow(int _chars)
{
    if ( ! ( this->Mode & std::ios::out) || ! this->Opened)
        return EOF;
    if (_chars != EOF) {
        *pptr() = _chars;
        pbump(1);
    }
    if ( this->flushBuffer() == EOF)
        return EOF;
    return _chars;
}

int clsCompressedStreamBuff::underflow()
{
    if ( gptr() && ( gptr() < egptr()))
        return * reinterpret_cast<unsigned char *>( gptr());

    if ( ! (this->Mode & std::ios::in) || ! this->Opened)
        return EOF;
    // Josuttis' implementation of inbuf
    int NumPutback = gptr() - eback();
    if ( NumPutback > 4)
        NumPutback = 4;
    memcpy( buffer + (4 - NumPutback), gptr() - NumPutback, NumPutback);

    int NumRead = gzread( this->File, buffer+4, bufferSize-4);
    if (NumRead <= 0) // ERROR or EOF
        return EOF;

    // reset buffer pointers
    setg( buffer + (4 - NumPutback),   // beginning of putback area
          buffer + 4,                 // read position
          buffer + 4 + NumRead);          // end of buffer

    // return next character
    return * reinterpret_cast<unsigned char *>( gptr());
}

int clsCompressedStreamBuff::sync()
{
    // Changed to use flush_buffer() instead of overflow( EOF)
    // which caused improper behavior with std::endl and flush(),
    // bug reported by Vincent Ricard.
    if ( pptr() && pptr() > pbase()) {
        if ( this->flushBuffer() == EOF)
            return -1;
    }
    return 0;
}

int clsCompressedStreamBuff::flushBuffer()
{
    // Separate the writing of the buffer from overflow() and
    // sync() operation.
    int Written = pptr() - pbase();
    if ( gzwrite( this->File, pbase(), Written) != Written)
        return EOF;
    pbump( -Written);
    return Written;
}

}
}
}

