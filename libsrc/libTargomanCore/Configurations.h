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

#ifndef TARGOMAN_CORE_CONFIGURATIONS_H
#define TARGOMAN_CORE_CONFIGURATIONS_H

#include "libTargomanCommon/exTargomanBase.h"

TARGOMAN_ADD_EXCEPTION_HANDLER(exTargomanCore, Targoman::Common::exTargomanBase);
TARGOMAN_ADD_EXCEPTION_HANDLER(exConfiguration, exTargomanCore);

namespace Targoman {
namespace Core {

namespace Private {
class clsConfigurationPrivate;
}

const size_t DEFAULT_MAX_TRANS_OPT_CACHE_SIZE = 10000;


class Configurations
{
public:
    ~Configurations();
    static inline Configurations& instance(){
        return *(Q_LIKELY(Instance) ? Instance : (Instance = new Configurations));}

    void init(quint8 _argc, const char** _argv);
    QVariant getConfig(const QString& _key, const QVariant &_default = QVariant()) const;
    void addConfig(const QString& _key, QVariant::Type _type, const QVariant& _defaultValue);

private:
    Configurations();
    Q_DISABLE_COPY(Configurations)

private:
    static Configurations* Instance;
    QScopedPointer<Private::clsConfigurationPrivate> pPrivate;
};

extern Configurations& gConfigs;

}
}

#endif // TARGOMAN_CORE_CONFIGURATIONS_H
