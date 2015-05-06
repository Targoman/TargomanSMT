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

#ifndef TARGOMAN_CORE_PRIVATE_INPUTDECOMPOSER_CLSTOKEN_H
#define TARGOMAN_CORE_PRIVATE_INPUTDECOMPOSER_CLSTOKEN_H


#include <QString>
#include <QHash>
#include "libTargomanCommon/Types.h"
#include "libTargomanCommon/CmdIO.h"

#ifdef TARGOMAN_SHOW_DEBUG
#include <QTextStream>
#endif

namespace Targoman {
namespace SMT {
namespace Private {
namespace InputDecomposer {

/**
 * @brief This class is defined to collecy whole data of a word (token) in integrated structure.
 */

class clsToken
{
public:
    clsToken(const QString& _value,
             const Common::WordIndex_t _wordIndex,
             const QString& _tagStr,
             const QVariantMap& _attrs){
        this->String = _value;
        this->WordIndex = _wordIndex;
        this->TagStr = _tagStr;
        this->Attrs = _attrs;
    }
    ~clsToken(){}

    inline const QString& string() const {return this->String;}
    inline const Common::WordIndex_t& wordIndex() const {return this->WordIndex;}
    inline const QString& tagStr() const {return this->TagStr;}
    inline const QVariantMap& attrs() const {return this->Attrs;}

private:
    QString String;                      /**< String of token. */
    Common::WordIndex_t WordIndex;      /**< Index of token in hash table. */
    QString TagStr;                     /**< If token is in ixml tag, its tag string will be recorded in this variable. */
    QVariantMap         Attrs;          /**< Some ixml tags has attributes. If token is in attributed ixml tag, attribute names and attributes values will be recorded in this variable.  */
};


}
}
}
}

#ifdef TARGOMAN_SHOW_DEBUG
inline QTextStream& operator << (QTextStream& _outputStream, const Targoman::SMT::Private::InputDecomposer::clsToken& _token)
{
    _outputStream << _token.string();
    return _outputStream;
}
#endif


#endif // TARGOMAN_CORE_PRIVATE_INPUTDECOMPOSER_CLSTOKEN_H