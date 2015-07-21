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

#include "libTargomanCommon/Macros.h"
#include "libTargomanCommon/CmdIO.h"
#include "libTargomanCommon/Types.h"
#include "libTargomanCommon/FastOperations.hpp"
#include "libTargomanCommon/clsCmdProgressBar.h"
#include "libTargomanCommon/HashFunctions.hpp"
#include "libTargomanCommon/PrefixTree/tmplPrefixTree.h"
#include "libTargomanCommon/tmplBoundedCache.hpp"
//Above files are included to be checked by compiler

//Macro used to export Library information
TARGOMAN_LIBRARY_VERSION

