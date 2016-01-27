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
 * @author Saeed Torabzadeh <saeed.torabzadeh@targoman.com>
 */

#ifndef TARGOMAN_COMMON_TYPES_H
#define TARGOMAN_COMMON_TYPES_H

#include <QtCore>
#include "limits.h"
#include "Macros.h"
/**
 * @brief Base namespace surrounding all other namespaces
 */
namespace Targoman {
/**
 * @brief Namespace containing classes, macros and usefull functions used all over the
 *        project without any other project dependency.Lo
 */
namespace Common {

typedef float LogP_t;
typedef double Cost_t;

typedef unsigned WordIndex_t;
typedef union { float AsFloat; quint32 AsUInt32; } FloatEncoded_t;

TARGOMAN_DEFINE_ENUM(enuStatus,
                     Ok              = 'o',
                     Error           = 'e',
                     Exclude         = 'd',
                     Info            = 'i',
                     Exclamation     = 'x',
                     NetConnected    = 'c',
                     NetDisconnected = 'n',
                     Stop            = 's',
                     Question        = 'q',
                     Warning         = 'w',
                     Unknown         = 'u')

TARGOMAN_DEFINE_ENHANCED_ENUM(enuReturnType,
                              Result,
                              Applied,
                              InvalidData,
                              ObjectNotFound,
                              InvalidAction,
                              InvalidUpdateSource,
                              InvalidStream,
                              InvalidLogin,
                              Undefined)

struct stuPong
{
    enuStatus::Type Status;
    enuStatus::Type SpecialColor;
    QString Message;

    inline stuPong(enuStatus::Type _status = enuStatus::Unknown,
                   enuStatus::Type _specialColor = enuStatus::Unknown,
                   const QString& _message = QString()){
        this->Status = _status;
        this->SpecialColor = _specialColor;
        this->Message = _message;
    }
};


class clsFilePath : public QString{
public:
    clsFilePath& operator = (const QString& _str){
        QString::operator =(_str);
        return *this;
    }
};

struct stuPos : public QPair<qint32, qint32>{
    stuPos(qint32 _start = -1, qint32 _end = -1) :
       QPair<qint32, qint32>(_start, _end) {
        Q_ASSERT((_start < _end) || (_start == -1 && _end == -1));
    }

    size_t start()const         {return this->first;}
    size_t end()const           {return this->second;}
    bool   isValid() const      {return this->first >= 0;}
    bool   hasSingleItem() const {return this->second - this->first == 1; }
};


}
}

class QWildCard : public QRegExp{
public:
    QWildCard():
        QRegExp()
    {}
    explicit QWildCard(const QString &pattern,
                       Qt::CaseSensitivity cs = Qt::CaseSensitive,
                       PatternSyntax syntax = RegExp):
        QRegExp(pattern, cs, syntax)
    { }

    QWildCard(const QRegExp &rx):
        QRegExp(rx)
    { }
};

//qRegisterMetaType<Targoman::Common::stuPong>("Targoman::Common::stuPong");
Q_DECLARE_METATYPE(Targoman::Common::stuPong)
typedef Targoman::Common::clsFilePath FilePath_t;

#endif // TARGOMAN_COMMON_TYPES_H
