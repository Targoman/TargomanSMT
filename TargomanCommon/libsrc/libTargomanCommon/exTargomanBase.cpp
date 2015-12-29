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

#include "exTargomanBase.h"
#include <iostream>

namespace Targoman {
namespace Common {

exTargomanBase::exTargomanBase(const QString& _message, quint32 _line) throw(){
    QString Message = _line ? QString::number(_line) + ": " + _message : _message;
    this->Message = Message.toUtf8();
}

exTargomanBase::~exTargomanBase() throw()
{}

void exTargomanBase::raise() const
{
    throw *this;
}

QException *exTargomanBase::clone() const
{
    return new exTargomanBase(*this);
}


QString exTargomanBase::what()
{
    return QString::fromUtf8(this->Message);
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
    //Show error on screen as this exception normally occurs before application startup
    std::cerr<<this->Message.constData()<<std::endl;
}

exTargomanMustBeImplemented::exTargomanMustBeImplemented(const QString& _message, int _line) :
    exTargomanNotImplemented(_message, _line)
{
    this->Message.append(">;exTargomanMustBeImplemented");
}

exTargomanInitialization::exTargomanInitialization(const QString& _message, int _line) :
    exTargomanBase(_message, _line)
{
    this->Message.append(">;exTargomanInitialization");
    //Show error on screen as this exception normally occurs before application startup
    std::cerr<<this->Message.constData()<<std::endl;
}

}
}

