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
 */

#ifndef TARGOMAN_COMMON_CONFIGURATION_VALIDATORS_H
#define TARGOMAN_COMMON_CONFIGURATION_VALIDATORS_H

#include "libTargomanCommon/Configuration/intfCrossValidate.hpp"
#include "libTargomanCommon/Constants.h"
#include "intfConfigurable.hpp"

namespace Targoman {
namespace Common {
namespace Configuration {
namespace Validators {

#define ValidatorLambda()[](const Targoman::Common::Configuration::intfConfigurable& _item,QString& _errorMessage)

///////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief The tmplNumericValidator template function can be used for all type of numeric configurables
 */
template <typename Type_t, int _max, int _min>
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
template <Targoman::Common::PathAccess::Options _requiredAccess>
    bool tmplPathAccessValidator (const intfConfigurable& _item,
                             QString& _errorMessage){
        QString Path = _item.toVariant().toString();
        QFileInfo PathInfo(Path);

        if (_requiredAccess.testFlag(Targoman::Common::PathAccess::Dir) && PathInfo.isDir() == false){
            _errorMessage = _item.configPath() + ": <"+Path+"> must be a directory";
            return false;
        }else if (_requiredAccess.testFlag(Targoman::Common::PathAccess::File) && PathInfo.isFile() == false){
            _errorMessage = _item.configPath() + ": <"+Path+"> must be a file";
            return false;
        }
        if (_requiredAccess.testFlag(Targoman::Common::PathAccess::Executable) && PathInfo.isExecutable() == false){
            _errorMessage = _item.configPath() + ": <"+Path+"> must be executable";
            return false;
        }

        if (_requiredAccess.testFlag(Targoman::Common::PathAccess::Readable) && PathInfo.isReadable() == false){
            _errorMessage = _item.configPath() + ": Unable to open <"+Path+"> for READING";
            return false;
        }
        if (_requiredAccess.testFlag(Targoman::Common::PathAccess::Writeatble) && PathInfo.isWritable() == false){
            _errorMessage = _item.configPath() + ": Unable to open <"+Path+"> for WRITING";
            return false;
        }
        _errorMessage.clear();
        return true;
    }

}
}
}
}
#endif // TARGOMAN_COMMON_CONFIGURATION_VALIDATORS_H
