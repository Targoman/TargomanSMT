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

static QVector<stuPos> getCorrespondence(const QString& _str1, const QString& _str2)
{
    Q_UNUSED(getCorrespondence);
    int Len1 = _str1.size();
    int Len2 = _str2.size();
    QVector<QVector<int>> DistanceMatrix(Len1 + 1, QVector<int>(Len2 + 1));

    DistanceMatrix[0][0] = 0;
    for(int i = 1; i <= Len1; ++i)
        DistanceMatrix[i][0] = i;
    for(int i = 1; i <= Len2; ++i)
        DistanceMatrix[0][i] = i;

    for(int i = 1; i <= Len1; ++i)
        for(int j = 1; j <= Len2; ++j)
                      DistanceMatrix[i][j] = qMin(
                            qMin(DistanceMatrix[i - 1][j] + 1, DistanceMatrix[i][j - 1] + 1),
                            DistanceMatrix[i - 1][j - 1] + (_str1[i - 1] == _str2[j - 1] ? 0 : 1)
                      );

    QVector<stuPos> Mapping(_str2.size());

    int i = Len1, end = Len1;
    int j = Len2;
    while(j > 0 && i > 0) {
        if(DistanceMatrix[i - 1][j - 1] + (_str1[i - 1] == _str2[j - 1] ? 0 : 1) == DistanceMatrix[i][j]) {
            Mapping[j - 1].first  = i - 1;
            Mapping[j - 1].second = end;
            end = i - 1;
            i--;
            j--;
        } else if(DistanceMatrix[i][j - 1] + 1 == DistanceMatrix[i][j]) {
            Mapping[j - 1].first  = -1;
            Mapping[j - 1].second = -1;
            j--;
        } else
            i--;
    }

    while(j > 0) {
        Mapping[j - 1].first  = -1;
        Mapping[j - 1].second = -1;
        j--;
    }

    if(i>0)
        Mapping[0].first = 0;

    return Mapping;
}

}
}
#endif // TARGOMAN_COMMON_HELPERS_HPP
