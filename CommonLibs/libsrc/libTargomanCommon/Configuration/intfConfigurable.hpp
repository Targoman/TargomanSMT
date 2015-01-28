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

class intfConfigurable
{
public:
    intfConfigurable(const QString&  _configPath,
                            const QString&  _description,
                            const QString&  _shortSwitch = "",
                            const QString&  _shortHelp = "",
                            const QString&  _longSwitch = "");

    virtual void        setFromVariant(const QVariant& _value) = 0;
    virtual QVariant    toVariant() const = 0 ;
    virtual bool        validate(const QVariant& _value, QString& _errorMessage) const = 0 ;
    virtual bool        crossValidate(QString& _errorMessage) const {Q_UNUSED(_errorMessage) return true;}
    virtual void        finalizeConfig() {}

    inline const QString& description()const{return this->Description;}
    inline const QString& shortSwitch()const{return this->ShortSwitch;}
    inline const QString& shortHelp()const{return this->ShortHelp;}
    inline const QString& longSwitch()const{return this->LongSwitch;}
    inline qint8 argCount()const{return this->ArgCount;}
    inline const QString& configPath()const{return this->ConfigPath;}

protected:
    QString ConfigPath;
    QString Description;
    QString ShortSwitch;
    QString ShortHelp;
    QString LongSwitch;
    qint8   ArgCount;
};

}
}
}

#endif // TARGOMAN_COMMON_CONFIGURATION_INTFCONFIGURABLE_HPP
