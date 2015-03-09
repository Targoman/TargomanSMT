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
                PathInfo.isDir() == false){
            _errorMessage = _item.configPath() + ": <"+Path+"> must be a directory";
            return false;
        }else if (Targoman::Common::testFlag(_requiredAccess, Targoman::Common::enuPathAccess::File) &&
                  PathInfo.isFile() == false){
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
        if (Targoman::Common::testFlag(_requiredAccess, Targoman::Common::enuPathAccess::Writeatble) &&
                PathInfo.isWritable() == false){
            _errorMessage = _item.configPath() + ": Unable to open <"+Path+"> for WRITING";
            return false;
        }
        _errorMessage.clear();

        return true;
    }

#define ConditionalPathValidator(_condition, _requiredAccess)\
   [] (const intfConfigurable& _item, QString& _errorMessage) { \
       if(_condition) \
           return Validators::tmplPathAccessValidator< \
                   (enuPathAccess::Type)(enuPathAccess::File | enuPathAccess::Readable), true>( \
                       _item, _errorMessage); \
       else \
           return Validators::tmplPathAccessValidator< \
                   (enuPathAccess::Type)(enuPathAccess::File | enuPathAccess::Readable), false>( \
                       _item, _errorMessage); \
   }

}
}
}
}
#endif // TARGOMAN_COMMON_CONFIGURATION_VALIDATORS_H
