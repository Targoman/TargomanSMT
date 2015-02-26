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
#ifndef TARGOMAN_CORE_PRIVATE_TYPES_H
#define TARGOMAN_CORE_PRIVATE_TYPES_H

#include <QBitArray>
#include "Types.h"

namespace Targoman {
namespace Core {
namespace Private {

typedef QBitArray Coverage_t;

#ifdef TARGOMAN_SHOW_DEBUG
inline std::ostream& operator << (std::ostream& _outputStream, const Coverage_t& _coverage)
{
    if(_coverage.size() == 0)
        return _outputStream;
    for(int i = 0; i < _coverage.size(); ++i)
        _outputStream << (_coverage.at(i) ? "1" : "0");
    return _outputStream;
}

inline QString bitArray2Str(const Coverage_t& _bits){
    QString Output;
    for(int i=0;i<_bits.size(); ++i)
        Output+=_bits.testBit(i) ? '1' : '0';
    return Output;
}
#endif

}
}
}

inline bool operator<(const Targoman::Core::Private::Coverage_t &_a, const Targoman::Core::Private::Coverage_t &_b)
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
