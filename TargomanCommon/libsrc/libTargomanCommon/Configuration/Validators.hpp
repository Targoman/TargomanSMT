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
 * @author Behrooz Vedadian <vedadian@targoman.com>
 */

#ifndef TARGOMAN_COMMON_CONFIGURATION_VALIDATORS_H
#define TARGOMAN_COMMON_CONFIGURATION_VALIDATORS_H

#include "libTargomanCommon/Constants.h"
#include "intfConfigurable.hpp"

namespace Targoman {
namespace Common {
namespace Configuration {
namespace Validators {

///////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief The tmplNumericValidator template function can be used for all type of numeric configurables
 */
template <typename Type_t, int _min, int _max>
     bool tmplNumericValidator (const intfConfigurable& _item,
                              QString& _errorMessage){
         Type_t Number = _item.toVariant().value<Type_t>();
         if (Number < _min || Number > _max){
             _errorMessage = _item.configPath() + QString(": must be between %1 - %2").arg(_min).arg(_max);
             return false;
         }
         _errorMessage.clear();
         return true;
     }

///////////////////////////////////////////////////////////////////////////////////////
/**
* @brief The tmplPathAccessValidator template function can be used for path(string) configurables
*/
template <Targoman::Common::enuPathAccess::Type _requiredAccess, bool _required = true>
   bool tmplPathAccessValidator (const intfConfigurable& _item,
                                  QString& _errorMessage){
        if (_required == false &&
            _item.toVariant().toString().isEmpty() &&
            _item.wasConfigured() == false)
            return true;

        QString Path = _item.toVariant().toString();
        QFileInfo PathInfo(Path);

        if (Targoman::Common::testFlag(_requiredAccess, Targoman::Common::enuPathAccess::Dir) &&
                (PathInfo.exists() == false || PathInfo.isDir() == false)){
            _errorMessage = _item.configPath() + ": <"+Path+"> must be a directory";
            return false;
        }else if (Targoman::Common::testFlag(_requiredAccess, Targoman::Common::enuPathAccess::File) &&
                  (PathInfo.exists() == false || PathInfo.isFile() == false)){
            _errorMessage = _item.configPath() + ": <"+Path+"> must be a file";
            return false;
        }
        if (Targoman::Common::testFlag(_requiredAccess, Targoman::Common::enuPathAccess::Executable) &&
                PathInfo.isExecutable() == false){
            _errorMessage = _item.configPath() + ": <"+Path+"> must be executable";
            return false;
        }

        if (Targoman::Common::testFlag(_requiredAccess, Targoman::Common::enuPathAccess::Readable) &&
                PathInfo.isReadable() == false){
            _errorMessage = _item.configPath() + ": Unable to open <"+Path+"> for READING";
            return false;
        }
        if (Targoman::Common::testFlag(_requiredAccess, Targoman::Common::enuPathAccess::Writeatble)){
            if (PathInfo.exists() == false){
                if (QFileInfo(PathInfo.path()).isWritable() == false){
                    _errorMessage = _item.configPath() + ": Unable to create <"+Path+">";
                    return false;
                }
            }else if (PathInfo.isWritable() == false){
                _errorMessage = _item.configPath() + ": Unable to open <"+Path+"> for WRITING";
                return false;
            }
        }
        _errorMessage.clear();

        return true;
    }

#define ConditionalPathValidator(_condition, _requiredAccess)\
   [] (const intfConfigurable& _item, QString& _errorMessage) { \
       if(_condition) \
           return Validators::tmplPathAccessValidator< \
                   (enuPathAccess::Type)(_requiredAccess), true>( \
                       _item, _errorMessage); \
       else \
            return true;\
   }

}
}
}
}
#endif // TARGOMAN_COMMON_CONFIGURATION_VALIDATORS_H
