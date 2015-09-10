/*******************************************************************************
 * Targoman: A robust Statistical Machine Translation framework                *
 *                                                                             *
 * Copyright 2014-2015 by ITRC <http://itrc.ac.ir>                           *
 *                                                                             *
 * This file is part of Targoman.                                              *
 *                                                                             *
 * Targoman is free software: you can redistribute it and/or modify            *
 * it under the terms of the GNU Lesser General Public License as published by *
 * the Free Software Foundation, either version 3 of the License, or           *
 * (at your option) any later version.                                         *
 *                                                                             *
 * Targoman is distributed in the hope that it will be useful,                 *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of              *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the               *
 * GNU Lesser General Public License for more details.                         *
 * You should have received a copy of the GNU Lesser General Public License    *
 * along with Targoman. If not, see <http://www.gnu.org/licenses/>.            *
 *                                                                             *
 *******************************************************************************/
/**
 @author S. Mohammad M. Ziabary <ziabary@targoman.com>
 */

#ifdef WITH_QJsonRPC

#include "Private/clsConfigByJsonRPC.h"
#include "libQJsonRPC/qjsonrpchttpserver.h"
#include "libQJsonRPC/qjsonrpctcpserver.h"

namespace Targoman {
namespace Common {
namespace Configuration {
namespace Private {

clsConfigByJsonRPC::clsConfigByJsonRPC(enuType _type) :
    intfConfigManagerOverNet(_type == TCP ?
                                 (QTcpServer*)new QJsonRpcTcpServer :
                                 ( QTcpServer*)new QJsonRpcHttpServer)
{}

clsConfigByJsonRPC::~clsConfigByJsonRPC()
{
    //Just to suppress compiler error using QScopedPointer
}

}
}
}
}
#endif //WITH_QJsonRPC
