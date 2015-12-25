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

namespace Proxies {
namespace NamedEntityRecognition {
class intfNamedEntityRecognizer;
}
}

namespace InputDecomposer {

/**
 * @brief This class is defined to collecy whole data of a word (token) in integrated structure.
 */

class clsToken
{
public:
    struct stuInfo{
        QString Str;
        QString TagStr;
        QVariantMap Attrs;
        stuInfo(const QString &_str = "",
                     const QString &_tagStr = "",
                     const QVariantMap &_attrs = QVariantMap()):
            Str(_str),
            TagStr(_tagStr),
            Attrs(_attrs)
        {}
    };

public:

    clsToken(const stuInfo& _info,
             const QList<Common::WordIndex_t>& _wordIndexes) {
        this->Info = _info;
        this->WordIndexes = _wordIndexes;
    }

    clsToken(const stuInfo& _info,
             const Common::WordIndex_t _wordIndexes)
        : clsToken(_info, QList<Common::WordIndex_t>() << _wordIndexes)
    { }

    inline const QString& string() const {return this->Info.Str;}
    inline const QList<Common::WordIndex_t>& wordIndexes() const {return this->WordIndexes;}
    inline const QString& tagStr() const {return this->Info.TagStr;}
    inline const QVariantMap& attrs() const {return this->Info.Attrs;}

private:
    stuInfo Info;
    QList<Common::WordIndex_t> WordIndexes;     /**< Indexes of token in hash table. */

    friend class Proxies::NamedEntityRecognition::intfNamedEntityRecognizer;
};


}
}
}
}

inline QTextStream& operator << (QTextStream& _outputStream, const Targoman::SMT::Private::InputDecomposer::clsToken& _token)
{
    _outputStream << _token.string();
    return _outputStream;
}

#endif // TARGOMAN_CORE_PRIVATE_INPUTDECOMPOSER_CLSTOKEN_H
