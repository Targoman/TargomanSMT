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

#ifndef TARGOMAN_COMMON_CONFIGURATION_INTFRPCEXPORTER_HPP
#define TARGOMAN_COMMON_CONFIGURATION_INTFRPCEXPORTER_HPP

#include <QObject>
#include <QVariantMap>
#include "libTargomanCommon/Types.h"

namespace Targoman {
namespace Common {
namespace Configuration {

struct stuRPCOutput{
    QVariant    DirectResult;
    QVariantMap IndirectResult;

    stuRPCOutput(const QVariant& _direct = QVariant(),
                 const QVariantMap& _indirect = QVariantMap()) :
        DirectResult(_direct),
        IndirectResult(_indirect)
    {}

    stuRPCOutput(quint8 _errCode, const QString& _errString):
        DirectResult(_errCode){
        IndirectResult.insert("err",_errString);
    }
};

class intfRPCExporter : public QObject{
    Q_OBJECT
public:
    intfRPCExporter(QObject *_parent = NULL) :
        QObject(_parent)
    {}

    /**
     * @brief exportMyRPCs will detect and export acceptable method to RPC registry.
     * Acceptable methods must have following conditions:
     *  1- their name must start with rpc or asyncRPC keyword
     *  2- must be defined as an slot
     *  3- their signature must be
     *        stuRPCOutput RPC_NAME(QVariantMap _input)
     * This method must be called in subclasses constructor
     */
    void exportMyRPCs();
};

}
}
}
Q_DECLARE_METATYPE(Targoman::Common::Configuration::stuRPCOutput)


#endif // TARGOMAN_COMMON_CONFIGURATION_INTFRPCEXPORTER_HPP
