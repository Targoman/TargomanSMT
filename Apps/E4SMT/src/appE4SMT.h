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

#ifndef TARGOMAN_APPS_APPE4SMT_H
#define TARGOMAN_APPS_APPE4SMT_H

#include "libTargomanCommon/clsSafeCoreApplication.h"
#include "libTargomanCommon/CmdIO.h"
#include "libTargomanCommon/Configuration/intfRPCExporter.hpp"
#include "libTargomanCommon/JSONConversationProtocol.h"

namespace Targoman {
namespace Apps {


class appE4SMT : public Common::Configuration::intfRPCExporter
{
    Q_OBJECT
public:
    appE4SMT(){
        this->exportMyRPCs();
    }


public slots:
    void slotExecute();

private slots:
    void slotValidateAgent(INOUT QString&        _user,
                          const QString&        _pass,
                          const QString&        _ip,
                          OUTPUT bool&          _canView,
                          OUTPUT bool&          _canChange);

    void slotPong(Targoman::Common::stuPong& _pong);

private slots:
    Common::Configuration::stuRPCOutput rpcNormalize(const QVariantMap&_args);
    Common::Configuration::stuRPCOutput rpcText2IXML(const QVariantMap&);
    Common::Configuration::stuRPCOutput rpcIXML2Text(const QVariantMap&);
    Common::Configuration::stuRPCOutput rpcTokenize(const QVariantMap&);

private:
    void processDir(const QString& _dir, const QString &_basePath);
    void processFile(const QString &_inputFile, const QString& _outFile);
    QStringList retrieveFileItems(const QString& _filePath);
};

}
}
#endif // TARGOMAN_APPS_APPE4SMT_H
