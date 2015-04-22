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
                enuReturnType::toStr(_type)).arg(
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
    stuRequest Request;

    QJsonParseError Error;
    QJsonDocument Doc = QJsonDocument::fromJson(_request, &Error);
    if (Error.error != QJsonParseError::NoError)
        throw exJSONConversationProtocol(Error.errorString() + QString::number(Error.offset));

    if (Doc.isArray()) {
        if (Doc.array().first().toVariant().toInt() == 1)
            Request.Name = "simplePing";
        else if (Doc.array().first().toVariant().toInt() == 2) {
            Request.Name = "ssidPing";
            if (Doc.array().size() > 1)
                Request.Args.insert("ssid", Doc.array().last().toVariant().toULongLong());
        }
    } else if (Doc.isObject()) {
        QString FirstObjectName =  Doc.object().begin().key();
        if (FirstObjectName == "r") {
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
                        Request.CallString = RPCObjectIter.value().toString();
                    else if (RPCObjectIter.key() == "w" &&
                             RPCObjectIter.value().isBool())
                        Request.SendWait = RPCObjectIter.value().toBool();
                    else {
                        Request.Name = RPCObjectIter.key();
                        Request.Args = RPCObjectIter.value().toVariant().toMap();
                        for(QVariantMap::Iterator ArgIter = Request.Args.begin();
                            ArgIter != Request.Args.end();) {
                            QVariantMap::Iterator ToDeleteIter = ArgIter;
                            ArgIter++;
                            if (ArgIter.value().toString() == "\127\127\127") {
                                Request.Args.erase(ToDeleteIter);
                                TargomanWarn(1,"Parameter: %s ignored", ArgIter.key().toStdString().c_str())
                            }
                        }
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
        return "\"" + _var.toString().replace("\"", "\\\"") + "\"";
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
