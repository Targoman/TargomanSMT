/*************************************************************************
 * Copyright © 2012-2014, Targoman.com
 *
 * Published under the terms of TCRL(Targoman Community Research License)
 * You can find a copy of the license file with distributed source or
 * download it from http://targoman.com/License.txt
 *
 *************************************************************************/
/**
 @author S. Mohammad M. Ziabary <smm@ziabary.com>
 */

#include "Configurations.h"
#include "Private/clsConfigurations_p.h"

namespace Targoman {
namespace Core {

Configurations* Configurations::Instance = NULL;
Configurations& gConfigs = Configurations::instance();

Configurations::Configurations() :
    pPrivate(new Private::clsConfigurationPrivate)
{
    this->pPrivate->Initialized = false;
}

Configurations::~Configurations()
{
    //Defined just to suppress compiler error on QScoppedPointer
}

void Configurations::init(quint8 _argc, const char **_argv)
{

    this->pPrivate->Initialized = true;
}

QVariant Configurations::getConfig(const QString &_key, const QVariant& _default) const
{
    if (this->pPrivate->Initialized == false)
        throw exConfiguration("Configuration is not initialized yet.");

    Private::clsConfigurationPrivate::stuConfigItem Item= this->pPrivate->Configs.value(_key);
    if (Item.Value.isValid() == false)
        return Item.Value;
    else
        return _default;
}

void Configurations::addConfig(const QString &_key, QVariant::Type _type, const QVariant &_defaultValue)
{

}

}
}

