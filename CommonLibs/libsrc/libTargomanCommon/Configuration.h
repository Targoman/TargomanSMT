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


class intfConfigurable;
/***************************************************************************************/
/**
 * @brief This class is an interface for all kinds of validators.
 * This class and derivations of this class checks the validity of value of a configurable in comparing with itself and other configurables.
 */
class intfCrossValidate{
public:
    intfCrossValidate(){}
    /**
     * @brief checks validity of input configurable.
     * @param[in] configurable item which is need to be a derivation of intfConfigurable class.
     * @param[in] _errorMessage error message for showing in case of not validity of configurable item.
     */
    virtual bool validate(const intfConfigurable& _item,
                          QString& _errorMessage) = 0;
};

/***************************************************************************************/
/**
 * @brief This class is an interface for all kinds of configurables. Configurables are options and configurations of programs.
 * This class and derivations of this class manage and holds informations of configurables.
 * This class and its derivations is used as value of pPrivate Map of clsConfigurationPrivate class.
 */
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
    virtual bool        crossValidate(QString& _errorMessage) const = 0;

    inline const QString& description()const{return this->Description;}
    inline const QString& shortSwitch()const{return this->ShortSwitch;}
    inline const QString& shortHelp()const{return this->ShortHelp;}
    inline const QString& longSwitch()const{return this->LongSwitch;}
    inline qint8 argCount()const{return this->ArgCount;}
    inline const QString& configPath()const{return this->ConfigPath;}

protected:
    QString ConfigPath;  /**< path of Configuration file */
    QString Description; /**< long descriptions of configurable  */
    QString ShortSwitch; /**< short switch of configurable i.e. "-i" option */
    QString ShortHelp;   /**< short descriptions of configurable */
    QString LongSwitch;  /**< long switch of configurable i.e. "--input" option */
    qint8   ArgCount;    /**< number of arguments for this configurable */
};

/***************************************************************************************/
/**
 * @brief The clsFileBasedConfig class is used when there are more optional configs stored in configuration file
 * this optional configs will not be stored and monitored by configuration manager.
 */
class clsFileBasedConfig : public intfConfigurable{
public:
    clsFileBasedConfig(const QString&  _configPath) :
        intfConfigurable(_configPath, "OPTIONAL_CONFIGS_IN_FILE"){
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
template <class Type_t> class clsConfigurable : public intfConfigurable
{
public:
    clsConfigurable(const QString&  _configPath,
                    const QString&  _description,
                    const QVariant& _default = QVariant(),
                    intfCrossValidate* _crossValidator = NULL,
                    const QString&  _shortSwitch = "",
                    const QString&  _shortHelp = "",
                    const QString&  _LongSwitch = ""):
        intfConfigurable(_configPath,
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

    /**
     * @brief This function will be overloaded for every type (such as int, float ,...).
     * This function converts input value from QVariant to a specific type based on overloaded implementation.
     */
    virtual inline void setFromVariant(const QVariant& _value){
        throw exTargomanMustBeImplemented("setFromVariant for "+this->ConfigPath+" Not Implemented");
    }

    /**
     * @brief convert #value variable to QVariant type.
     */
    virtual inline QVariant    toVariant() const{
        return QVariant(this->Value);
    }

    /**
     * @brief This function will be overloaded for every type (such as int, float ,...)
     * This function validate value of input value.
     */
    virtual inline bool        validate(const QVariant& _value, QString& _errorMessage) const{
        return true;
    }
    /**
     * @brief if cross validator function is prepared that function will be called, else returns true.
     */
    virtual inline bool        crossValidate(QString& _errorMessage) const{
        return Q_LIKELY(this->CrossValidator) ? this->CrossValidator->validate(*this, _errorMessage) : true;
    }

    inline Type_t  value(){return this->Value;}
    inline Type_t  operator ()(){return this->Value;}

private:
    Type_t  Value;
    intfCrossValidate* CrossValidator;
};

/***************************************************************************************/
/**
 * @def type specific validate and setFromVariant functions signiture.
 */
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
_SPECIAL_CONFIGURABLE(QList<quint8>)
/***************************************************************************************/
namespace Validators {
    /**
     * @brief The clsPathValidator class overloads intfCrossValidate that can be used for path(string) configurables
     */
    class clsPathValidator : public intfCrossValidate{
    public:
        clsPathValidator(PathAccess::Options _requiredAccess);
        bool validate(const intfConfigurable& _item,
                      QString &_errorMessage);
    private:
        PathAccess::Options RequiredAccess;
    };
    ///////////////////////////////////////////////////////////////////////////////////////
    /**
     * @brief The clsIntValidator class overloads intfCrossValidate that can be used for int configurables
     */
    class clsIntValidator : public intfCrossValidate{
    public:
        clsIntValidator(qint64 _min, qint64 _max);
        bool validate(const intfConfigurable& _item,
                      QString &_errorMessage);
    private:
        qint64 Max,Min;
    };
    ///////////////////////////////////////////////////////////////////////////////////////
    /**
     * @brief The clsUIntValidator class overloads intfCrossValidate that can be used for uint configurables
     */
    class clsUIntValidator : public intfCrossValidate{
    public:
        clsUIntValidator(quint64 _min, quint64 _max);
        bool validate(const intfConfigurable& _item,
                      QString &_errorMessage);
    private:
        quint64 Max,Min;
    };
    ///////////////////////////////////////////////////////////////////////////////////////
    /**
     * @brief The clsDoubleValidator class overloads intfCrossValidate that can be used for double configurables
     */
    class clsDoubleValidator : public intfCrossValidate{
    public:
        clsDoubleValidator(double _min, double _max);
        bool validate(const intfConfigurable& _item,
                      QString &_errorMessage);
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
    void addConfig(const QString _path, intfConfigurable* _item);
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
