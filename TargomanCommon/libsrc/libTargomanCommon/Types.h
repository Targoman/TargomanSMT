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

#define OUTPUT
#define INOUT

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

}
}

typedef Targoman::Common::clsFilePath FilePath_t;

#endif // TARGOMAN_COMMON_TYPES_H
