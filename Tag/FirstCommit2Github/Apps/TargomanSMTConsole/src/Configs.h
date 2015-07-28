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

#ifndef TARGOMAN_APPS_TARGOMANSMTCONSOLE_CONFIGS_H
#define TARGOMAN_APPS_TARGOMANSMTCONSOLE_CONFIGS_H

#include "libTargomanCommon/Configuration/tmplConfigurable.h"
#include "libTargomanCommon/Configuration/Validators.hpp"
#include "libTargomanCommon/Macros.h"
#include "libTargomanCommon/exTargomanBase.h"

namespace Targoman {
namespace Apps {

TARGOMAN_ADD_EXCEPTION_HANDLER(exTargomanSMTConsole, Common::exTargomanBase);

TARGOMAN_DEFINE_ENHANCED_ENUM(enuAppMode,
                              Translation,
                              Training,
                              MakeBinary
                              );
}
}

ENUM_CONFIGURABLE(Targoman::Apps::enuAppMode);

namespace Targoman {
namespace Apps {

class gConfigs{
public:
    static inline QString appConfig(const QString& _name){
        return "App/" + _name;
    }

    static Common::Configuration::tmplConfigurable<enuAppMode::Type>    Mode;
    static Common::Configuration::tmplConfigurable<QString>             InputFile;
    static Common::Configuration::tmplConfigurable<QString>             InputText;
    static Common::Configuration::tmplConfigurable<QString>             OutputFile;
    static Common::Configuration::tmplConfigurable<quint8>              MaxThreads;
};

}
}
#endif // TARGOMAN_APPS_TARGOMANSMTCONSOLE_CONFIGS_H
