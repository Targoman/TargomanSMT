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

#ifndef TARGOMAN_COMMON_HELPERS_HPP
#define TARGOMAN_COMMON_HELPERS_HPP

#include <typeinfo>
#include <cxxabi.h>
#include <QVector>
#include <QPair>
#include "libTargomanCommon/Types.h"

namespace Targoman {
namespace Common {

template<typename T> QString getTypeStr(T _type){
    char * Name = NULL;
    QString ReturnVal;
    int Status;
    Name = abi::__cxa_demangle(typeid(_type).name(), 0, 0, &Status);
    if (Name != NULL)
        ReturnVal =  Name;
    else
        ReturnVal =  typeid(_type).name();
    free(Name);
    return ReturnVal;
}

inline int minofVec(const QVector<int>& _values)
{
    int Min = _values.at(0);
    for(int i = 1; i < _values.size(); ++i)
        if(Min > _values.at(i))
            Min = _values.at(i);
    return Min;
}

static QVector<stuPos> getCorrespondence(const QString& _str1, const QString& _str2)
{
#define DELETION     0
#define INSERTION    1
#define SUBSTITUTION 2

    Q_UNUSED(getCorrespondence);
    const int L1 = _str1.size();
    const int L2 = _str2.size();
    const int S = L1 + 1;

    QVector<int> D((L1 + 1) * (L2 + 1), 0);
    QVector<int> Dij(3, 0);

    for(int i = 1; i <= L1; ++i)
        D[i] = 4 * i;
    for(int i = 1; i <= L2; ++i)
        D[i * S] = 4 * i;
    for(int i = 1; i <= L1; ++i) {
        for(int j = 1; j <= L2; ++j) {
            Dij[DELETION] = D[j * S + (i - 1)] + 1;
            Dij[INSERTION] = D[(j - 1) * S + i] + 1;
            Dij[SUBSTITUTION] = D[(j - 1) * S + (i - 1)];
            if(_str1.at(i - 1) != _str2.at(j - 1))
                Dij[SUBSTITUTION] += 1;
            D[j * S + i] = minofVec(Dij);
        }
    }

    QVector<stuPos> Correspondence(_str2.size());

    int i = L1;
    int j = L2;
    int End = L1;
    while(i > 0 && j > 0) {
        Dij[DELETION] = D[j * S + (i - 1)] + 1;
        Dij[INSERTION] = D[(j - 1) * S + i] + 1;
        Dij[SUBSTITUTION] = D[(j - 1) * S + (i - 1)];
        if(_str1.at(i - 1) != _str2.at(j - 1))
            Dij[SUBSTITUTION] += 1;

        int Winner;
        if((_str1.at(i - 1) == ' ') != (_str2.at(j - 1) == ' ')) {
            if(_str1.at(i - 1) == ' ')
                Winner = DELETION;
            else
                Winner = INSERTION;
        } else {
            Winner = DELETION;
            if(Dij[Winner] > Dij[INSERTION])
                Winner = INSERTION;
            if(Dij[Winner] > Dij[SUBSTITUTION])
                Winner = SUBSTITUTION;
        }

        switch(Winner) {
        case DELETION:
            --i;
            break;
        case INSERTION:
            Correspondence[j - 1] = stuPos();
            --j;
            break;
        case SUBSTITUTION:
            Correspondence[j - 1] = stuPos(i - 1, End);
            End = i - 1;
            --i;
            --j;
            break;
        }
    }
    while(j > 0) {
        Correspondence[j - 1] = stuPos();
        --j;
    }

    return Correspondence;
}

}
}
#endif // TARGOMAN_COMMON_HELPERS_HPP
