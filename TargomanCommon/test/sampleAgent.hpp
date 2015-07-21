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

#ifndef SAMPLEAGENT_HPP
#define SAMPLEAGENT_HPP

#include <QObject>
#include "libTargomanCommon/Types.h"
#include "libTargomanCommon/Configuration/ConfigManager.h"

using namespace Targoman::Common;
class clsSampleAgent : public QObject
{
    Q_OBJECT
public:
    clsSampleAgent(){
        connect(&Configuration::ConfigManager::instance(),
                SIGNAL(sigPing(Targoman::Common::stuPong&)),
                this,
                SLOT(slotPong(Targoman::Common::stuPong&)),
                Qt::DirectConnection);
        connect(&Configuration::ConfigManager::instance(),
                SIGNAL(sigValidateAgent(QString&,QString,QString,bool&,bool&)),
                this,
                SLOT(slotValidateAgent(QString&,QString,QString,bool&,bool&)),
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

    void slotPong(Targoman::Common::stuPong& _pong){
        _pong.Message =" Hey Pong";
        _pong.SpecialColor = Targoman::Common::enuStatus::Error;
        _pong.Status = Targoman::Common::enuStatus::Exclamation;
    }
};

#endif // SAMPLEAGENT_HPP


