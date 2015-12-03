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

#ifndef TARGOMAN_COMMON_CONFIGURATION_TMPLSMTCONFIGURABLE_HPP
#define TARGOMAN_COMMON_CONFIGURATION_TMPLSMTCONFIGURABLE_HPP

#include "libTargomanCommon/Configuration/tmplConfigurable.h"

namespace Targoman {
namespace Common {
namespace Configuration {

template <class itmplType_t> class tmplSMTConfigurable : public tmplConfigurable<itmplType_t>
{
public:
    tmplConfigurable(const QString&  _configPath,
                     const QString&  _description,
                     const QVariant& _default = QVariant(),
                     const std::function< bool(const intfConfigurable& _item,
                                               QString& _errorMessage) >& _crossValidator = ReturnTrueCrossValidator,
                     const QString&  _shortSwitch = "",
                     const QString&  _shortHelp = "",
                     const QString&  _LongSwitch = "",
                     enuConfigSource::Type _configSources = (enuConfigSource::Type)(
                enuConfigSource::Arg  |
                enuConfigSource::File |
                enuConfigSource::Net ),
                     bool _remoteView = true,
                     const std::function< void(const intfConfigurable& _item) >& _finalizer = VoidFinalizer
                     ) :
        intfConfigurable(enuConfigType::Normal,
                         _configPath,
                         _description,
                         _shortSwitch,
                         _shortHelp,
                         _LongSwitch,
                         _configSources,
                         _remoteView,
                         _finalizer)
    {
        try{
            QString ErrorMessage;
            if (!this->validate(_default, ErrorMessage)){
                throw exTargomanInitialization("Invalid default value for: " + _configPath + ": " + ErrorMessage);
            }
            this->setFromVariant(_default);
            if (this->ShortHelp.size()){
                this->Description.append(" ( default= '" + this->toVariant().toString() + "' )");
            }
            this->CrossValidator = _crossValidator;
        }catch(exTargomanBase &e){
            TargomanError(e.what());
            throw;
        }
    }
}

}
}
}

#endif // TARGOMAN_COMMON_CONFIGURATION_TMPLSMTCONFIGURABLE_HPP
