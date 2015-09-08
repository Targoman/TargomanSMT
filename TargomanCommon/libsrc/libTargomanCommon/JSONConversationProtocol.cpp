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

#include "JSONConversationProtocol.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

namespace Targoman {
namespace Common{

QString JSONConversationProtocol::variantMap2JSONObject(const QVariantMap& _map)
{
    QString JSONObject = "{";

    for (QVariantMap::ConstIterator MapIter = _map.constBegin();
         MapIter != _map.constEnd();
         MapIter++)
    {
        if (MapIter.value().type() == QVariant::Map)
            JSONObject+= "\"" + MapIter.key() + "\":" + variantMap2JSONObject(MapIter.value().toMap());
        else if (MapIter.value().type() == QVariant::List)
            JSONObject+="\"" + MapIter.key() + "\":" + variantList2JSONArray(MapIter.value().toList());
        else
            JSONObject+="\"" + MapIter.key() + "\":" + variant2Json(MapIter.value());

        if (MapIter + 1 != _map.constEnd())
            JSONObject+=",";
    }

    return JSONObject + "}";
}

QString JSONConversationProtocol::variantList2JSONArray(const QVariantList& _list)
{
    QString JSONArray = "[";

    for (QVariantList::ConstIterator ListIter = _list.constBegin();
         ListIter != _list.constEnd();
         ListIter++)
    {
        if (ListIter->type() == QVariant::Map)
            JSONArray += variantMap2JSONObject(ListIter->toMap());
        if (ListIter->type() == QVariant::List)
            JSONArray += variantList2JSONArray(ListIter->toList());
        else
            JSONArray += variant2Json(*ListIter);

        if (ListIter + 1 != _list.constEnd())
            JSONArray+=",";
    }

    return JSONArray + "]";
}

QString JSONConversationProtocol::prepareError(const QString& _callBack,
                                               const QString& _callString,
                                               enuReturnType::Type _type,
                                               const QString& _message)
{
    QString   ReturnStr;
    if (_callBack.size())
        ReturnStr = _callBack + "(";
    else
        ReturnStr = "";

    ReturnStr+=QString("{\"e\":{\"q\":\"%1\",\"c\":%2,\"s\":\"%3:%4\"").arg(
                _callString).arg(
                _type).arg(
                _type).arg(
                _message);

    if (_callBack.size())
        return ReturnStr+="}})";
    else
        return ReturnStr+="}}";
}

QString JSONConversationProtocol::preparePong(const stuPong& _pong)
{
    if (_pong.Status == enuStatus::Unknown)
        return "[2]";
    else
        return QString("[3,\"%1\",\"%2\",\"%3\"]").arg(
                    (char)_pong.Status).arg(
                    _pong.SpecialColor == enuStatus::Unknown ?
                        (char)_pong.Status : (char)_pong.SpecialColor).arg(
                    _pong.Message);
}

QString JSONConversationProtocol::preparePing(quint64 _ssid)
{
    if (_ssid)
        return QString("[2,%1]").arg(_ssid);
    else
        return "[1]";
}

QString JSONConversationProtocol::prepareRequest(const JSONConversationProtocol::stuRequest &_request)
{
    QString Request = "{\"r\":{";
    if (_request.CallBack.size())
        Request.append("\"b\":\"" + _request.CallBack + "\",");
    if (_request.CallUID.size())
        Request.append("\"q\":\"" + _request.CallUID + "\",");
    if (_request.SendWait)
        Request.append("\"w\":1,");
    Request.append("\"" + _request.Name + "\":");
    if (_request.Args.isEmpty())
        Request.append("{}");
    else
        Request.append(variant2Json(_request.Args));

    return Request + "}}";
}

QString JSONConversationProtocol::prepareResult(const QString &_callBack,
                                                const QString &_callString,
                                                const QVariant &_result,
                                                const QVariantMap &_args)
{
    QString   ReturnStr;

    if (_callBack.size())
        ReturnStr = _callBack + "({\"r\":{";
    else
        ReturnStr = "{\"r\":{";

    if (_callString.size())
        ReturnStr+=QString("\"q\":\"%1\",").arg(_callString);

    ReturnStr+="\"r\":[";

    ReturnStr += variant2Json(_result);

    if (_args.size())
    {
        ReturnStr+=",{";
        for (QVariantMap::ConstIterator ArgIter = _args.constBegin();
             ArgIter != _args.constEnd();
             ArgIter++
             )
        {
            ReturnStr += "\"" + ArgIter.key() + "\":";
            ReturnStr += variant2Json(ArgIter.value());
            ReturnStr += ((ArgIter + 1 != _args.constEnd()) ? "," : "");
        }
        ReturnStr+="}";
    }


    if (_callBack.size())
        return ReturnStr+="]}})";
    else
        return ReturnStr+="]}}";
}

QString JSONConversationProtocol::prepareWait(const QString &_callBack,
                                              const QString& _callString)
{
    QString   ReturnStr;

    if (_callBack.size())
        ReturnStr = _callBack + "({\"r\":{";
    else
        ReturnStr = "{\"r\":{";

    ReturnStr+=QString("\"q\":\"%1\",").arg(_callString);

    ReturnStr+="\"w\":1";


    if (_callBack.size())
        return ReturnStr+="}})";
    else
        return ReturnStr+="}}";
}

JSONConversationProtocol::stuRequest JSONConversationProtocol::parseRequest(const QByteArray& _request)
{
    JSONConversationProtocol::stuRequest Request;

    QJsonParseError Error;
    QJsonDocument Doc = QJsonDocument::fromJson(_request, &Error);
    if (Error.error != QJsonParseError::NoError)
        throw exJSONConversationProtocol(Error.errorString() +": " + QString::number(Error.offset));

    if (Doc.isArray()) {
        if (Doc.array().first().toVariant().toInt() == 1)
            Request.Name = "simplePing";
        else if (Doc.array().first().toVariant().toInt() == 2) {
            Request.Name = "ssidPing";
            if (Doc.array().size() > 1)
                Request.Args.insert("ssid", Doc.array().last().toVariant().toULongLong());
            else
                throw exJSONConversationProtocol("No SSID defined for ping type 2");
        }
    } else if (Doc.isObject()) {
        if (Doc.object().begin().key() == "r") {
            QJsonObject RPCObject = Doc.object().value("r").toObject();
            if (RPCObject.isEmpty()){
                throw exJSONConversationProtocol("Invalid JSON Request");
            } else {
                Request.SendWait = true;
                for (QJsonObject::const_iterator RPCObjectIter = RPCObject.constBegin();
                     RPCObjectIter != RPCObject.constEnd();
                     RPCObjectIter++)
                {
                    if (RPCObjectIter.key() == "b" &&
                            RPCObjectIter.value().isString())
                        Request.CallBack = RPCObjectIter.value().toString();
                    else if (RPCObjectIter.key() == "q" &&
                             RPCObjectIter.value().isString())
                        Request.CallUID = RPCObjectIter.value().toString();
                    else if (RPCObjectIter.key() == "w" &&
                             RPCObjectIter.value().isBool())
                        Request.SendWait = RPCObjectIter.value().toBool();
                    else {
                        Request.Name = RPCObjectIter.key();
                        Request.Args = RPCObjectIter.value().toVariant().toMap();
                    }
                }
            }
        } else
            throw exJSONConversationProtocol("Invalid request JSON");
    }
    else
        throw exJSONConversationProtocol("Doc must be started by object");
    return Request;
}

JSONConversationProtocol::stuResponse JSONConversationProtocol::parseResponse(const QByteArray &_response)
{
    stuResponse Response;

    QJsonParseError Error;
    QJsonDocument Doc = QJsonDocument::fromJson(_response, &Error);
    if (Error.error != QJsonParseError::NoError)
        throw exJSONConversationProtocol(Error.errorString() +": " + QString::number(Error.offset));

    if (Doc.isArray()) {
        if (Doc.array().first().toVariant().toInt() == 2)
            Response.Type = stuResponse::Pong;
        else if (Doc.array().first().toVariant().toInt() == 3) {
            if (Doc.array().size() < 4)
                throw exJSONConversationProtocol("Invalid count of arguments on enhanced pong");
            Response.Result = Doc.array().at(1).toVariant().toChar();
            Response.Args.insert("Color", Doc.array().at(1).toVariant().toChar());
            Response.Args.insert("Message", Doc.array().at(1).toVariant().toString());
            Response.Type = stuResponse::EnhancedPong;
        }
    } else if (Doc.isObject()) {
        if (Doc.object().begin().key() == "e") {
            QJsonObject RPCObject = Doc.object().value("e").toObject();
            Response.Type       = stuResponse::Error;
            Response.CallUID = RPCObject.value("q").toString();
            Response.Result  = RPCObject.value("c");
            Response.Args.insert("Code", RPCObject.value("s").toString(":").split(":").first());
            Response.Args.insert("Message", RPCObject.value("s").toString(":").split(":").last());
        }else if (Doc.object().begin().key() == "r"){
            QJsonObject RPCObject = Doc.object().value("r").toObject();
            Response.Type       = stuResponse::Ok;
            Response.CallUID = RPCObject.value("q").toString();
            if (RPCObject.value("r").isArray() == false)
                throw exJSONConversationProtocol("Invalid response without array");

            QJsonArray ResultArray = RPCObject.value("r").toArray();
            Response.Result = ResultArray.first();
            if (ResultArray.size() > 1 && ResultArray.at(1).isObject()){
                Response.Args = ResultArray.at(1).toObject().toVariantMap();
                /*for(QVariantMap::Iterator ArgIter = Response.Args.begin();
                    ArgIter != Response.Args.end();
                    ArgIter++) {
                    QVariantMap::Iterator ToDeleteIter = ArgIter;
                    if (ArgIter.value().toString() == "\127\127\127") {
                        Response.Args.erase(ToDeleteIter);
                        TargomanWarn(1,"Parameter: %s ignored", ArgIter.key().toUtf8().constData())
                    }
                }*/
            }

        }else
            throw exJSONConversationProtocol("Unrecognized protocol command");
    }
    return Response;
}

QString JSONConversationProtocol::variant2Json(const QVariant& _var)
{
    switch(_var.type())
    {
    case QVariant::Bool:
    case QVariant::Int:
    case QVariant::LongLong:
        return QString::number(_var.toLongLong());
        break;

    case QVariant::Double:
        return QString::number(_var.toReal());
        break;

    case QVariant::UInt:
    case QVariant::ULongLong:
        return QString::number(_var.toULongLong());
        break;

    case QVariant::String:
    case QVariant::Char:
    case QVariant::Date:
    case QVariant::DateTime:
    case QVariant::Time:
    case QVariant::Url:
        return "\"" + _var.toString().replace("\"", "\\\"").replace("\n","\\n") + "\"";
        break;

    case QVariant::List:
        return variantList2JSONArray(_var.toList());
        break;

    case QVariant::Map:
        return variantMap2JSONObject(_var.toMap());
        break;


    default:
        Q_ASSERT_X(false, "Formating Response to JSON", "Invalid Object Item unconvertible to String");
    }
    return "!!!!ERROR!!!!";
}
}
}
