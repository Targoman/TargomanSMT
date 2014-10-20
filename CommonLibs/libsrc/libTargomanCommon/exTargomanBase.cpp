/*************************************************************************
 * Copyright © 2012-2014, Targoman.com
 *
 * Published under the terms of TCRL(Targoman Community Research License)
 * You can find a copy of the license file with distributed source or
 * download it from http://targoman.com/License.txt
 *
 *************************************************************************/
/**
 @author S. Mohammad M. Ziabary <smm@ziabary.com>
 */

#include "exTargomanBase.h"

namespace Targoman {
namespace Common {

exTargomanBase::exTargomanBase(const QString& _message, quint32 _line) throw(){
    this->Message = _line ? QString::number(_line) + ": " + _message : _message;
}

exTargomanBase::~exTargomanBase() throw()
{}

void exTargomanBase::raise() const
{
    throw *this;
}

QtConcurrent::Exception* exTargomanBase::clone()
{
    return new exTargomanBase(*this);
}


QString exTargomanBase::what()
{
    return this->Message;
}

exTargomanInvalidParameter::exTargomanInvalidParameter(const QString& _message, int _line):
    exTargomanBase(_message, _line)
{
    /// Class name not inserted in message to be hidden in subclass messages
}

exTargomanNotEnoughMemory::exTargomanNotEnoughMemory(const QString& _message, int _line) :
    exTargomanBase(_message, _line)
{
    /// Class name not inserted in message to be hidden in subclass messages
}

exTargomanNotImplemented::exTargomanNotImplemented(const QString& _message, int _line) :
    exTargomanBase(_message, _line)
{
    this->Message.append(">;exTargomanNotImplemented");
}

exTargomanMustBeImplemented::exTargomanMustBeImplemented(const QString& _message, int _line) :
    exTargomanNotImplemented(_message, _line)
{
    this->Message.append(">;exTargomanMustBeImplemented");
}

}
}

