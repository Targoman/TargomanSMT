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

#ifndef TARGOMAN_COMMON_JSONCONVERSATIONPROTOCOL_H
#define TARGOMAN_COMMON_JSONCONVERSATIONPROTOCOL_H

#include <QVariantMap>
#include <QVariantList>

#include "libTargomanCommon/exTargomanBase.h"
#include "libTargomanCommon/Types.h"

namespace Targoman {
namespace Common{

TARGOMAN_ADD_EXCEPTION_HANDLER(exJSONConversationProtocol, Targoman::Common::exTargomanBase);

class JSONConversationProtocol
{
public:
    struct stuRequest
    {
        QString      Name;
        QString      CallString;
        QVariantMap  Args;
        QString      CallBack;
        bool         SendWait;
    };

    struct stuPong
    {
        enuStatus::Type Status;
        enuStatus::Type SpecialColor;
        QString Message;

        inline stuPong(enuStatus::Type _status = enuStatus::Unknown,
                       enuStatus::Type _specialColor = enuStatus::Unknown,
                       QString _message = QString()){
            this->Status = _status;
            this->SpecialColor = _specialColor;
            this->Message = _message;
        }
    };

    static QString prepareResult(const QString& _callBack,
                                 const QString& _callString,
                                 const QVariant& _result,
                                 const QVariantMap& _args = QVariantMap());

    static QString prepareWait(const QString& _callBack,
                               const QString& _callString);

    static QString prepareError(const QString& _callBack,
                                const QString& _callString,
                                enuReturnType::Type _type,
                                const QString& _message);

    static QString preparePong(const stuPong &_pong = enuStatus::Unknown);
    static stuRequest parseRequest(const QByteArray &_request);

protected:
    static QString variantMap2JSONObject(const QVariantMap& _map);
    static QString variantList2JSONArray(const QVariantList& _map);
    static QString variant2Json(const QVariant& _var);
};


}
}
#endif // TARGOMAN_COMMON_JSONCONVERSATIONPROTOCOL_H
