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
#include <QDataStream>
#include "libTargomanCommon/Constants.h"
#include "libTargomanCommon/exTargomanBase.h"

namespace Targoman {
namespace Common {
/**
 * @brief The clsOFStreamExtended class is a derivation of std::fstream class to facilitate writing
 * to file.
 */
class clsOFStreamExtended : public std::fstream {
public:

    /**
     * @brief clsOFStreamExtended   Constructor of this class, instantiates base fstream class with
     *                              file path and open file for output.
     * @param _filePath             file path.
     */
    clsOFStreamExtended(const QString& _filePath) :
        std::fstream(_filePath.toUtf8().constData(), std::ios_base::out)
    {}

    /**
     *  @brief This template function, facilitates writing basic data types and can be overloaded for
     *          some specific data types.
     */
    template <class Type_t>
    inline void write(const Type_t _value){
        this->write((char*)&_value, sizeof(_value));
    }

    /**
     * @brief write writes a pointer of char* up to size to file.
     * @param _data input char* data pointer.
     * @param _size size of data.
     * @return returns itself.
     */
    inline __ostream_type& write(const char* _data, size_t _size) {
        std::fstream::write(_data, _size);
        return *this;
    }
};

template <> void clsOFStreamExtended::write(QString _value);

/**
 * @brief The clsIFStreamExtended class is a derivation of std::fstream class to facilitate reading
 * from file.
 */
class clsIFStreamExtended : public std::fstream{
public:
    clsIFStreamExtended()
    { }

    /**
     * @brief clsOFStreamExtended   Constructor of this class, instantiates base fstream class with
     *                              file path and open file for input.
     * @param _filePath             file path.
     */
    clsIFStreamExtended(const QString& _filePath, bool _useBuffer = false) :
        std::fstream(_filePath.toUtf8().constData(), std::ios_base::in),
        BufferStream(&this->Buffer, QIODevice::ReadOnly)
    {
        this->UseBuffer = _useBuffer;
    }

    /**
     *  @brief This template function, facilitates reading basic data types and can be overloaded for
     *          some specific data types.
     */
    template <class Type_t>
    inline Type_t read(){
        Type_t _storage;
        this->read((char*)&_storage, sizeof(_storage));
        return _storage;
    }

    /**
     * @brief read reads up to size from file and stores that in a char* data pointer.
     * @param[out] _data output char* data pointer.
     * @param _size size of data.
     * @return returns itself.
     */
    inline __istream_type& read(char* _data, size_t _size) {
        if (this->UseBuffer){
            if(_size > Common::Constants::MaxFileIOBytes) {
                size_t BytesRead = this->BufferStream.readRawData(_data, _size);
                std::fstream::read(_data + BytesRead, _size - BytesRead);
                this->Buffer.clear();
                this->BufferStream.device()->seek(0);
                this->BufferStream.resetStatus();
            } else {
                size_t BytesToRead = _size;
                do{
                    if(this->BufferStream.atEnd()){
                        this->Buffer.resize(Constants::MaxFileIOBytes);
                        this->BufferStream.device()->seek(0);
                        this->BufferStream.resetStatus();
                        std::fstream::read(this->Buffer.data(), Constants::MaxFileIOBytes);
                    }
                    BytesToRead -= this->BufferStream.readRawData(_data, BytesToRead);
                }while(BytesToRead > 0);
            }
        }else
            std::fstream::read(_data, _size);
        return *this;
    }

    inline void lock() {
        this->ReadLock.lock();
    }

    inline void unlock() {
        this->ReadLock.unlock();
    }

    /**
     * @brief seekg
     * @param _offset
     * @param _dir
     * @return
     */
    inline __istream_type& seekg(qint64 _offset, ios_base::seekdir _dir){
        if (this->UseBuffer){
            throw exTargomanNotImplemented("seekg can not be used in Buffered mode.");
        }else
            return  std::fstream::seekg(_offset, _dir);
    }

    /**
     * @brief tellg
     * @return
     */
    inline quint64 tellg(){
        if (this->UseBuffer){
            throw exTargomanNotImplemented("tellg can not be used in Buffered mode.");
        }else
            return std::fstream::tellg();
    }

private:
    QMutex      ReadLock; /** A Mutex to prevent multiple threads write at the same time.*/
    bool        UseBuffer;
    QByteArray  Buffer;
    QDataStream BufferStream;
};

template <> QString clsIFStreamExtended::read();

}
}

#endif // TARGOMAN_COMMON_FSTREAMEXTENDED_HPP
