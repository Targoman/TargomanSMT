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
 @author Behrooz Vedadian <vedadian@gmail.com>
 */

#ifndef TARGOMAN_COMMON_CONFIGURATION_INTFCONFIGURABLE_HPP
#define TARGOMAN_COMMON_CONFIGURATION_INTFCONFIGURABLE_HPP

#include <QString>
#include <QVariant>

namespace Targoman {
namespace Common {
namespace Configuration {
/**
 * @brief Private section of Configuration module where obfuscated classes are defined
 */
namespace Private {
class intfConfigurablePrivate;
}

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
    intfConfigurable(const QString&  _configPath,
                     const QString&  _description,
                     const QString&  _shortSwitch = "",
                     const QString&  _shortHelp = "",
                     const QString&  _longSwitch = "");

    intfConfigurable(const intfConfigurable& _other);

    ~intfConfigurable();

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
    virtual void        finalizeConfig() {}

    inline const QString& description()const{return this->Description;}
    inline const QString& shortSwitch()const{return this->ShortSwitch;}
    inline const QString& shortHelp()const{return this->ShortHelp;}
    inline const QString& longSwitch()const{return this->LongSwitch;}
    inline bool  canBemanaged() {return this->ArgCount >= 0;}
    inline qint8 argCount()const{return this->ArgCount;}
    inline const QString& configPath()const{return this->ConfigPath;}
    inline void  setIsConfigured() { this->WasConfigured = true; }
    inline bool  wasConfigured() const {return this->WasConfigured; }

protected:
    QString ConfigPath;     /**< Config path of the configurable item. Path is used to access Item by name or via configuration file */
    QString Description;    /**< Description of the configurable item. this will also be used as Long Help String*/
    QString ShortSwitch;    /**< (Optional) Short switch to configure item via arguments */
    QString ShortHelp;      /**< (Optional) Argument structure to be shown when printing help*/
    QString LongSwitch;     /**< (Optional) Long switch to be used to configure item via program arguments*/
    qint8   ArgCount;       /**< Number of arguments for this configurable */
    bool    WasConfigured;      /**< Indicates that value has been set by Config sources not default */

private:
    QScopedPointer<Private::intfConfigurablePrivate> pPrivate;
};

}
}
}

#endif // TARGOMAN_COMMON_CONFIGURATION_INTFCONFIGURABLE_HPP
