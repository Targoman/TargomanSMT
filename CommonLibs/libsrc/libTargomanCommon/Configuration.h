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

#ifndef TARGOMAN_COMMON_CONFIGURATION_H
#define TARGOMAN_COMMON_CONFIGURATION_H

#include <cfloat>
#include "libTargomanCommon/exTargomanBase.h"
#include "libTargomanCommon/Constants.h"

namespace Targoman {
namespace Common {

namespace Private {
class clsConfigurationPrivate;
}

TARGOMAN_ADD_EXCEPTION_HANDLER(exConfiguration, exTargomanBase);


class clsConfigurableAbstract;
/***************************************************************************************/
class clsCrossValidateAbstract{
public:
    clsCrossValidateAbstract(const clsConfigurableAbstract& _item):
        Item(_item)
    {}

    virtual bool validate(QString& _errorMessage) = 0;

protected:
    const clsConfigurableAbstract& Item;
};

/***************************************************************************************/
class clsConfigurableAbstract
{
public:
    clsConfigurableAbstract(const QString&  _configPath,
                            const QString&  _description,
                            const QString&  _shortSwitch = "",
                            const QString&  _shortHelp = "",
                            const QString&  _longSwitch = "");

    virtual void        setFromVariant(const QVariant& _value) = 0;
    virtual QVariant    toVariant() const = 0 ;
    virtual bool        validate(const QVariant& _value, QString& _errorMessage) const = 0 ;
    virtual bool        crossValidate(QString& _errorMessage) const = 0;

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

/***************************************************************************************/
/**
 * @brief The clsFileBasedConfig class is used when there are more optional configs stored in configuration file
 * this optional configs will not be stored and monitored by configuration manager.
 */
class clsFileBasedConfig : public clsConfigurableAbstract{
public:
    clsFileBasedConfig(const QString&  _configPath) :
        clsConfigurableAbstract(_configPath, "OPTIONAL_CONFIGS_IN_FILE"){
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
};

/***************************************************************************************/
/**
 * @brief The clsConfigurable template is used to store and validate different configurable items
 */
template <class Type_t> class clsConfigurable : public clsConfigurableAbstract
{
public:
    clsConfigurable(const QString&  _configPath,
                    const QString&  _description,
                    const QVariant& _default,
                    clsCrossValidateAbstract* _crossValidator = NULL,
                    const QString&  _shortSwitch = "",
                    const QString&  _shortHelp = "",
                    const QString&  _LongSwitch = ""):
        clsConfigurableAbstract(_configPath,
                                _description,
                                _shortSwitch,
                                _shortHelp,
                                _LongSwitch){
        QString ErrorMessage;
        if (!this->validate(_default, ErrorMessage))
            throw exTargomanInitialization("Invalid default value for: " + _configPath + ": " + ErrorMessage);
        this->setFromVariant(_default);
        this->CrossValidator = _crossValidator;
    }

    virtual inline void setFromVariant(const QVariant& _value){
        throw exTargomanMustBeImplemented("setFromVariant for "+this->ConfigPath+" Not Implemented");
    }

    virtual inline QVariant    toVariant() const{
        return QVariant(this->Value);
    }

    virtual inline bool        validate(const QVariant& _value, QString& _errorMessage) const{
        return true;
    }

    virtual inline bool        crossValidate(QString& _errorMessage) const{
        return Q_LIKELY(this->CrossValidator) ? this->CrossValidator->validate(_errorMessage) : true;
    }

    inline Type_t  value(){return this->Value;}

private:
    Type_t  Value;
    clsCrossValidateAbstract* CrossValidator;
};

/***************************************************************************************/
#define _SPECIAL_CONFIGURABLE(_type) \
    template <> bool clsConfigurable<_type>::validate(const QVariant& _value, QString& _errorMessage) const ;\
    template <> void clsConfigurable<_type>::setFromVariant(const QVariant& _value);

/***************************************************************************************/
_SPECIAL_CONFIGURABLE(qint8)
_SPECIAL_CONFIGURABLE(qint16)
_SPECIAL_CONFIGURABLE(qint32)
_SPECIAL_CONFIGURABLE(qint64)
_SPECIAL_CONFIGURABLE(quint8)
_SPECIAL_CONFIGURABLE(quint16)
_SPECIAL_CONFIGURABLE(quint32)
_SPECIAL_CONFIGURABLE(quint64)
_SPECIAL_CONFIGURABLE(double)
_SPECIAL_CONFIGURABLE(float)
_SPECIAL_CONFIGURABLE(QString)
_SPECIAL_CONFIGURABLE(bool)
/***************************************************************************************/
namespace Validators {
    class clsPathValidator : public clsCrossValidateAbstract{
    public:
        clsPathValidator(const clsConfigurableAbstract& _item,
                         PathAccess _requiredAccess);
        bool validate(QString &_errorMessage);
    private:
        PathAccess RequiredAccess;
    };
    ///////////////////////////////////////////////////////////////////////////////////////
    class clsIntValidator : public clsCrossValidateAbstract{
    public:
        clsIntValidator(const clsConfigurableAbstract& _item,
                        qint64 _min, qint64 _max);
        bool validate(QString &_errorMessage);
    private:
        qint64 Min,Max;
    };
    ///////////////////////////////////////////////////////////////////////////////////////
    class clsUIntValidator : public clsCrossValidateAbstract{
    public:
        clsUIntValidator(const clsConfigurableAbstract& _item,
                         quint64 _min, quint64 _max);
        bool validate(QString &_errorMessage);
    private:
        quint64 Min,Max;
    };
    ///////////////////////////////////////////////////////////////////////////////////////
    class clsDoubleValidator : public clsCrossValidateAbstract{
    public:
        clsDoubleValidator(const clsConfigurableAbstract& _item,
                           double _min, double _max);
        bool validate(QString &_errorMessage);
    private:
        double Max,Min;
    };
}

/***************************************************************************************/
/**
 * @brief The Configuration class is the base configuration manager class.
 * Currently it will just manage Arguments and config file
 */
class Configuration
{
public:
    ~Configuration();
    static inline Configuration& instance(){
        return *(Q_LIKELY(Instance) ? Instance : (Instance = new Configuration));}

    void init(const QStringList &_arguments, const QString &_license);
    void save2File(const QString&  _fileName, bool _backup);
    void addConfig(const QString _path, clsConfigurableAbstract* _item);
    QVariant getConfig(const QString& _path, const QVariant &_default = QVariant());

private:
    Configuration();
    Q_DISABLE_COPY(Configuration)

private:
    static Configuration* Instance;
    QScopedPointer<Private::clsConfigurationPrivate> pPrivate;
};

}
}
#endif // TARGOMAN_COMMON_CONFIGURATION_H
