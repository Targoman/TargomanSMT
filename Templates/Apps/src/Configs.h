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
 * @author 
 */

#ifndef TARGOMAN_APPS_APPNAMESPACE_CONFIGS_H
#define TARGOMAN_APPS_APPNAMESPACE_CONFIGS_H

#include "libTargomanCommon/Configuration/tmplConfigurable.h"
#include "libTargomanCommon/Configuration/Validators.hpp"
#include "libTargomanCommon/Macros.h"
#include "libTargomanCommon/exTargomanBase.h"

namespace Targoman {
namespace Apps {

TARGOMAN_ADD_EXCEPTION_HANDLER(exAPP_NAME, Common::exTargomanBase);

class gConfigs{
public:
    static inline QString appConfig(const QString& _name){
        return "App/" + _name;
    }

    static Common::Configuration::tmplConfigurable<TYPE>          CONFIG_VAR_1;
    static Common::Configuration::tmplConfigurable<TYPE>          CONFIG_VAR_2;
    static Common::Configuration::tmplConfigurable<TYPE>          CONFIG_VAR_n;
};

}
}
#endif // TARGOMAN_APPS_APPNAMESPACE_CONFIGS_H
