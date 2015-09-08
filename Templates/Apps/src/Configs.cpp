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

#include "Configs.h"

namespace Targoman {
namespace Apps {

using namespace Common;
using namespace Common::Configuration;

tmplConfigurable<TYPE>     gConfigs::CONFIG_VAR_1(
        gConfigs::appConfig("CONFIG_VAR_1"),
        "CONFIG_VAR_1 description",
        "CONFIG_VAR_1 default value",
        CONFIG_VAR_1 Validator lambda,
        "CONFIG_VAR_1 Short switch",
        "CONFIG_VAR_1 PARAMNAME",
        "CONFIG_VAR_1 Long switch",
        CONFIG_VAR_1 configuration sources,
        CONFIG_VAR_1 can be remote viewed,
        CONFIG_VAR_1 finalization lambda);

tmplConfigurable<TYPE>     gConfigs::CONFIG_VAR_2(
        gConfigs::appConfig("CONFIG_VAR_2"),
        "CONFIG_VAR_2 description",
        "CONFIG_VAR_2 default value",
        CONFIG_VAR_2 Validator lambda,
        "CONFIG_VAR_2 Short switch",
        "CONFIG_VAR_2 PARAMNAME",
        "CONFIG_VAR_2 Long switch",
        CONFIG_VAR_2 configuration sources,
        CONFIG_VAR_2 can be remote viewed,
        CONFIG_VAR_2 finalization lambda);

tmplConfigurable<TYPE>     gConfigs::CONFIG_VAR_n(
        gConfigs::appConfig("CONFIG_VAR_n"),
        "CONFIG_VAR_n description",
        "CONFIG_VAR_n default value",
        CONFIG_VAR_n Validator lambda,
        "CONFIG_VAR_n Short switch",
        "CONFIG_VAR_n PARAMNAME",
        "CONFIG_VAR_n Long switch",
        CONFIG_VAR_n configuration sources,
        CONFIG_VAR_n can be remote viewed,
        CONFIG_VAR_n finalization lambda);

}
}
