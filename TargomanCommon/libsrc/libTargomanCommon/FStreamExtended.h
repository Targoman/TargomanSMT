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

#ifndef TARGOMAN_COMMON_FSTREAMEXTENDED_HPP
#define TARGOMAN_COMMON_FSTREAMEXTENDED_HPP

#include <fstream>
#include <QString>
#include <QMutex>

namespace Targoman {
namespace Common {

class clsOFStreamExtended : public std::fstream {
public:
    clsOFStreamExtended(const QString& _filePath) :
        std::fstream(_filePath.toUtf8().constData(), std::ios_base::out)
    {}

    template <class Type_t>
    inline void write(const Type_t _value){
        this->write((char*)&_value, sizeof(_value));
    }
    inline __ostream_type& write(const char* _data, size_t _size) {
        std::fstream::write(_data, _size);
        return *this;
    }
};

template <> void clsOFStreamExtended::write(QString _value);

class clsIFStreamExtended : public std::fstream{
public:
    clsIFStreamExtended()
    { }

    clsIFStreamExtended(const QString& _filePath) :
        std::fstream(_filePath.toUtf8().constData(), std::ios_base::in)
    {}

    template <class Type_t>
    inline Type_t read(){
        Type_t _storage;
        this->read((char*)&_storage, sizeof(_storage));
        return _storage;
    }
    inline __istream_type& read(char* _data, size_t _size) {
        std::fstream::read(_data, _size);
        return *this;
    }
    inline void lock() {
        this->ReadLock.lock();
    }
    inline void unlock() {
        this->ReadLock.unlock();
    }
private:
    QMutex ReadLock;
};

template <> QString clsIFStreamExtended::read();

}
}

#endif // TARGOMAN_COMMON_FSTREAMEXTENDED_HPP
