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
 * @author Behrooz Vedadian <vedadian@targoman.com>
 */

#ifndef TARGOMAN_COMMON_EXTARGOMANBASE_HPP
#define TARGOMAN_COMMON_EXTARGOMANBASE_HPP

#include <QException>
#include <QString>

#include "libTargomanCommon/CmdIO.h"

namespace Targoman {
namespace Common {

/**
 * @brief A Macro to add a new exception handler. This will insert exception name at end of the message
 **/
#define TARGOMAN_ADD_EXCEPTION_HANDLER(_name,_base) \
    class _name : public _base{\
    public: _name (const QString& _message = "", int _line = 0) : \
            _base (_message, _line){ \
    this->Message.append(" >;" TARGOMAN_M2STR(_name));\
    }}

class exTargomanStdOverrider : public QException {
public:
    const char* what() const _GLIBCXX_USE_NOEXCEPT {
        return this->Message.constData();
    }

protected:
  QByteArray Message;
};

/**
 * @exception exTargomanBase
 * @brief Base Exception Class. All the classes will raise an exception inherited from this
 */
class exTargomanBase: public exTargomanStdOverrider
{
  public:
    /**
     * @brief Base Exception constructor.
     *
     * @param _message Message to be shown when calling what()
     * @param _line Line Number where the exception occured Defaults to 0.
     **/
    exTargomanBase(const QString& _message = "", quint32 _line = 0) throw ();
    ~exTargomanBase() throw ();

    void raise() const;
    QException* clone() const;
    /**
     * @brief A method to show Exception message
     * @note this method must be defined as const but it will collide with std::exception
     * @return QString Exception message
     **/
    QString what();
};

/**
 * @exception exTargomanInvalidParameter
 * @brief Base Exception on invalid parameters
 */
class exTargomanInvalidParameter: public exTargomanBase
{
  public:
    exTargomanInvalidParameter(const QString& _message = "", int _line = 0);
};

/**
 * @exception exTargomanNotEnoughMemory
 * @brief Base Exception on not enough memory errors
 */
class exTargomanNotEnoughMemory: public exTargomanBase
{
  public:
    exTargomanNotEnoughMemory(const QString& _message, int _line = 0);
};

/**
 * @exception exTargomanNotImplemented
 * @brief Exception on not implemented methods. This exception must not be subclassed
 */
class exTargomanNotImplemented: public exTargomanBase
{
  public:
    exTargomanNotImplemented(const QString& _message = "", int _line = 0);
};

/**
 * @exception exTargomanMustBeImplemented
 * @brief Exception on virtual methods that must be implemented in subclasses. This exception must not be subclassed
 */
class exTargomanMustBeImplemented: public exTargomanNotImplemented
{
  public:
    exTargomanMustBeImplemented(const QString& _message = "", int _line = 0);
};

/**
 * @exception exTargomanMustBeImplemented
 * @brief Exception on Initialization of static members before Application start. This exception must not be subclassed
 */
class exTargomanInitialization: public exTargomanBase
{
  public:
    exTargomanInitialization(const QString& _message = "", int _line = 0);
};

}
}

#endif /* TARGOMAN_COMMON_EXTARGOMANBASE_HPP */
