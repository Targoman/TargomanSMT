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

#ifndef SAMPLEAGENT_HPP
#define SAMPLEAGENT_HPP

#include <QObject>
#include "libTargomanCommon/Types.h"
#include "libTargomanCommon/JSONConversationProtocol.h"
#include "libTargomanCommon/Configuration/ConfigManager.h"

using namespace Targoman::Common;
class clsSampleAgent : public QObject
{
    Q_OBJECT
public:
    clsSampleAgent(){
        connect(&Configuration::ConfigManager::instance(),
                SIGNAL(sigPing(JSONConversationProtocol::stuPong&)),
                this,
                SLOT(slotPong(JSONConversationProtocol::stuPong&)),
                Qt::DirectConnection);
        connect(&Configuration::ConfigManager::instance(),
                SIGNAL(sigValidateAgent(QString&,QString,QString,bool&,bool&)),
                this,
                SLOT(slotValidateAgent(QString&,QString,QString,bool&,bool&)),
                Qt::DirectConnection);
        connect(&Configuration::ConfigManager::instance(),
                SIGNAL(sigRPC(QString,QVariantMap&,QVariant&)),
                this,
                SLOT(slotRPC(QString,QVariantMap&,QVariant&)),
                Qt::DirectConnection);
    }

public slots:
    void slotValidateAgent(INOUT QString&        _user,
                          const QString&        _pass,
                          const QString&        _ip,
                          OUTPUT bool&          _canView,
                          OUTPUT bool&          _canChange){
        Q_UNUSED(_pass);
        Q_UNUSED(_ip);
        _user = "ali";
        _canView = true;
        _canChange = true;
    }

    void slotRPC(const QString&            _funcName,
                INOUT  QVariantMap&       _arguments,
                OUTPUT QVariant&          _returnVal){
        if (_funcName == "rpcTest"){
            _returnVal = "Hello";
        }
        Q_UNUSED(_arguments)
    }

    void slotPong(JSONConversationProtocol::stuPong& _pong){
        _pong.Message =" Hey Pong";
        _pong.SpecialColor = Targoman::Common::enuStatus::Error;
        _pong.Status = Targoman::Common::enuStatus::Exclamation;
    }
};

#endif // SAMPLEAGENT_HPP


