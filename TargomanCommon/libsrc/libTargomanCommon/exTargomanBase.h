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
 */

#ifndef TARGOMAN_COMMON_exTargomanBASE_HPP_
#define TARGOMAN_COMMON_exTargomanBASE_HPP_

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

/**
 * @exception exTargomanBase
 * @brief Base Exception Class. All the classes will raise an exception inherited from this
 */
class exTargomanBase: public QException
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
    QException* clone();
    /**
     * @brief A method to show Exception message
     *
     * @return QString Exception message
     **/
    QString what();

  protected:
    QString Message;
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

#endif /* TARGOMAN_COMMON_EXTARGOMANBASE_HPP_ */
