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
        QString      CallUID;
        QVariantMap  Args;
        QString      CallBack;
        bool         SendWait;
        inline stuRequest(const QString& _name = "",
                          const QString& _uuid = "",
                          const QVariantMap& _args = QVariantMap(),
                          bool _sendWait = false,
                          const QString& _callback = ""){
            this->Name = _name;
            this->CallUID = _uuid;
            this->CallBack = _callback;
            this->Args = _args;
            this->SendWait = _sendWait;
        }
    };


    //TODO obey Qt Design pattern to avoid copy
    struct stuResponse
    {
        enum enuResponseTypes{
            Ok,
            Error,
            Pong,
            EnhancedPong
        };

        enuResponseTypes Type;
        QString      CallUID;
        QVariant     Result;
        QVariantMap  Args;

        stuResponse(enuResponseTypes    _type = Error,
                    const QVariant&     _result = QVariant(),
                    const QVariantMap&  _args = QVariantMap(),
                    const QString&      _callUID = ""){
            this->Type = _type;
            this->CallUID = _callUID;
            this->Result = _result;
            this->Args = _args;
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
    static QString preparePing(quint64 _ssid = 0);
    static QString prepareRequest(const stuRequest& _request);

    static stuRequest parseRequest(const QByteArray &_request);
    static stuResponse parseResponse(const QByteArray &_request);

protected:
    static QString variantMap2JSONObject(const QVariantMap& _map);
    static QString variantList2JSONArray(const QVariantList& _map);
    static QString variant2Json(const QVariant& _var);
};


}
}
#endif // TARGOMAN_COMMON_JSONCONVERSATIONPROTOCOL_H
