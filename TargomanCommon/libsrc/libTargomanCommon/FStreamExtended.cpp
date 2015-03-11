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

#include "FStreamExtended.h"

namespace Targoman {
namespace Common {

template <> void clsOFStreamExtended::write(QString _value){
    QByteArray StringData = _value.toUtf8();
    this->write(StringData.size());
    this->write(StringData.constData(), StringData.size());
}

template <>  QString clsIFStreamExtended::read(){
    QByteArray Data(this->read<int>(),Qt::Uninitialized);
    this->read(Data.data(), Data.size());
    return QString::fromUtf8(Data);
}

}
}
