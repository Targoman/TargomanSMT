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

#ifndef TARGOMAN_COMMON_CONFIGURATION_INTFCONFIGURABLE_HPP
#define TARGOMAN_COMMON_CONFIGURATION_INTFCONFIGURABLE_HPP

#include <QString>
#include <QVariant>

#include "libTargomanCommon/Macros.h"

namespace Targoman {
namespace Common {
namespace Configuration {
/**
 * @brief Private section of Configuration module where obfuscated classes are defined
 */
namespace Private {
class intfConfigurablePrivate;
}

TARGOMAN_DEFINE_ENUM(enuConfigSource,
                     Arg      = 0x01,
                     File     = 0x02,
                     ReadOnly = 0x04,
                     Net      = 0x08,
                     Invalid  = 0x00)

TARGOMAN_DEFINE_ENHANCED_ENUM(enuConfigType,
                              Normal,
                              FileBased,
                              Array,
                              Module);

class intfConfigurable;
/// @brief A predefined lambda function used which always returns true used when there
/// is no further crossvalidation condition.
static std::function<void(const intfConfigurable&)> VoidFinalizer = [] (const intfConfigurable&) {};

/**
 * @brief This class is an interface for all kinds of configurables. Configurables are options and configurations of programs.
 *
 * This class and derivations of this class manage and holds informations of configurables.
 * This class and its derivations is used as value of Configs Map of clsConfigurationPrivate class.
 */
class intfConfigurable
{
public:
    /**
     * @brief intfConfigurable base interface on all configurable items. Constructor has been implmented in ConfigManager.cpp
     * @param _configPath Config path of the configurable item. Path is used to access Item by name or via configuration file
     * @param _description Description of the configurable item. this will also be used as Long Help String
     * @param _shortSwitch (Optional) Short switch to configure item via arguments
     * @param _shortHelp (Optional) Argument structure to be shown when printing help
     * @param _longSwitch (Optional) Long switch to be used to configure item via program arguments
     */
    intfConfigurable(enuConfigType::Type _configType,
                     const QString&  _configPath,
                     const QString&  _description,
                     const QString&  _shortSwitch = "",
                     const QString&  _shortHelp = "",
                     const QString&  _longSwitch = "",
                     enuConfigSource::Type _configSources =
                        (enuConfigSource::Type)(
                            enuConfigSource::Arg  |
                            enuConfigSource::File |
                            enuConfigSource::Net ),
                     bool _remoteView = true,
                     const std::function< void(const intfConfigurable& _item) >& _finalizer = VoidFinalizer
                     );

    intfConfigurable(const intfConfigurable& _other);

    virtual ~intfConfigurable();

    /**
     * @brief setFromVariant Pure virtual function to set configurable item by a variant.
     * @param _value variant value to be set on configurable item.
     */
    virtual void        setFromVariant(const QVariant& _value) = 0;
    /**
     * @brief toVariant Pure virtual method to convert item to a variant in order to be stored in file
     * @return A valid QVariant
     */
    virtual QVariant    toVariant() const = 0 ;
    /**
     * @brief validate Pure virtual method used to validate configurable items. This method will be called on first
     * alloation and change
     * @param[in] _value Value to be validated
     * @param[out] _errorMessage An storage to report why it was invalid
     * @return true if _value is validated else false
     */
    virtual bool        validate(const QVariant& _value, QString& _errorMessage) const = 0 ;
    /**
     * @brief crossValidate Pure virtual method used to post validate configurable item. this method will be called after all
     *                      Configuration items are filled.
     * @param[out] _errorMessage  An storage to report why it was invalid
     * @return true if vilidated else false
     */
    virtual bool        crossValidate(QString& _errorMessage) const {Q_UNUSED(_errorMessage) return true;}
    /**
     * @brief finalizeConfig method called on those configurable items which need an extra post-configuration method
     */
    virtual void        finalizeConfig();

    virtual QString     typeString() const = 0;
    virtual QString     validValues() const = 0;

    inline const QString& description()const{return this->Description;}
    inline const QString& shortSwitch()const{return this->ShortSwitch;}
    inline const QString& shortHelp()const{return this->ShortHelp;}
    inline const QString& longSwitch()const{return this->LongSwitch;}
    inline bool  canBemanaged() { return this->ConfigType == enuConfigType::Normal ||
                                         this->ConfigType == enuConfigType::Module; }
    inline bool  canBeConfigured(enuConfigSource::Type _source) const { return testFlag(this->ConfigSources, _source) ;}
    inline qint8 argCount()const{return this->ArgCount;}
    inline const QString& configPath()const{return this->ConfigPath;}
    inline void  setIsConfigured() { this->WasConfigured = true; }
    inline bool  wasConfigured() const {return this->WasConfigured; }
    inline enuConfigSource::Type configSources() { return this->ConfigSources; }
    inline bool remoteView() const { return this->RemoteViewAllowed; }
    inline enuConfigType::Type configType(){return this->ConfigType;}

protected:
    enuConfigType::Type  ConfigType; /**< Represent type of configuration item*/
    QString ConfigPath;     /**< Config path of the configurable item. Path is used to access Item by name or via configuration file */
    QString Description;    /**< Description of the configurable item. this will also be used as Long Help String*/
    QString ShortSwitch;    /**< (Optional) Short switch to configure item via arguments */
    QString ShortHelp;      /**< (Optional) Argument structure to be shown when printing help*/
    QString LongSwitch;     /**< (Optional) Long switch to be used to configure item via program arguments*/
    qint8   ArgCount;       /**< Number of arguments for this configurable */
    bool    WasConfigured;      /**< Indicates that value has been set by Config sources not default */
    enuConfigSource::Type  ConfigSources; /**< Indicates that from which source this item can be configured */
    bool    RemoteViewAllowed;   /** Indicates wheter this item can be monitored remotely or not **/
private:
    QScopedPointer<Private::intfConfigurablePrivate> pPrivate;
};

typedef std::function<bool(const intfConfigurable& _item, QString& _errorMessage)> Validator_t;

}
}
}

#endif // TARGOMAN_COMMON_CONFIGURATION_INTFCONFIGURABLE_HPP
