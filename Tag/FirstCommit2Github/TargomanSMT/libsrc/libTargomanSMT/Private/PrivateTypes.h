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

#ifndef TARGOMAN_CORE_PRIVATE_TYPES_H
#define TARGOMAN_CORE_PRIVATE_TYPES_H

#include <QBitArray>
#include "Types.h"
#ifdef TARGOMAN_SHOW_DEBUG
#include <QTextStream>
#endif

namespace Targoman {
namespace SMT {
namespace Private {

typedef QBitArray Coverage_t;

#ifdef TARGOMAN_SHOW_DEBUG
inline bool operator == (const Coverage_t& _coverage, const char* value)
{
    if(strlen(value) != (size_t)_coverage.size())
        return false;
    for(int i = 0; i < _coverage.size(); ++i)
        if((value[i] == '1') ^ _coverage.testBit(i))
            return false;
    return true;
}

inline bool operator == (const char* value, const Coverage_t& _coverage)
{
    return _coverage == value;
}

inline QTextStream& operator << (QTextStream& _outputStream, const Coverage_t& _coverage)
{
    if(_coverage.size() == 0)
        return _outputStream;
    for(int i = 0; i < _coverage.size(); ++i)
        _outputStream << (_coverage.testBit(i) ? "1" : "0");
    return _outputStream;
}

inline QString bitArray2Str(const Coverage_t& _bits){
    QString Output;
    for(int i=0;i<_bits.size(); ++i)
        Output+=_bits.testBit(i) ? '1' : '0';
    return Output;
}

template<typename Type_t>
inline QTextStream& operator << (QTextStream& _outputStream, const QList<Type_t>& _list)
{
    if(_list.size() == 0)
        return _outputStream;
    _outputStream << _list.at(0);
    for(int i = 1; i < _list.size(); ++i)
        _outputStream << " " << _list.at(i);
    return _outputStream;
}

#endif

}
}
}

inline bool operator<(const Targoman::SMT::Private::Coverage_t &_a, const Targoman::SMT::Private::Coverage_t &_b)
{
    Q_ASSERT(_a.size() == _b.size());
    for(int i = 0; i < _a.size(); ++i)
        if(_a[i] < _b[i])
            return false;
        else if(_b[i] < _a[i])
            return true;
    return false;
}

#endif // TYPES_H
