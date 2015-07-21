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

#ifndef TARGOMAN_COMMON_CONFIGURATION_CLSFILEBASEDCONFIG_HPP
#define TARGOMAN_COMMON_CONFIGURATION_CLSFILEBASEDCONFIG_HPP

#include "libTargomanCommon/Configuration/intfConfigurable.hpp"

namespace Targoman {
namespace Common {
namespace Configuration {

/**
 * @brief The clsFileBasedConfig class is used when there are more optional configs stored in configuration file
 * this optional configs will not be stored and monitored by configuration manager.
 */
class clsFileBasedConfig : public intfConfigurable{
public:
    clsFileBasedConfig(const QString&  _configPath) :
        intfConfigurable(enuConfigType::FileBased,
                         _configPath,
                         "OPTIONAL_CONFIGS_IN_FILE",
                         "",
                         "",
                         "",
                         enuConfigSource::Invalid){
        this->RemoteViewAllowed = false;
        this->ArgCount = -1;
    }

    virtual inline void setFromVariant(const QVariant& ){
        throw exTargomanNotImplemented(this->ConfigPath + " is Abstract");
    }

    virtual inline QVariant    toVariant() const{
        throw exTargomanNotImplemented(this->ConfigPath + " is Abstract");
    }

    virtual inline bool        validate(const QVariant&, QString&) const{
        return true;
    }

    virtual inline bool        crossValidate(QString& ) const{
        return true;
    }

    virtual QString typeString() const{
        return "NULL";
    }

    virtual QString validValues() const{
        return "";
    }
};

}
}
}

#endif // TARGOMAN_COMMON_CONFIGURATION_CLSFILEBASEDCONFIG_HPP
