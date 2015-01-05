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

#ifndef TARGOMAN_CORE_PRIVATE_CLSCONFIGURATIONS_P_H
#define TARGOMAN_CORE_PRIVATE_CLSCONFIGURATIONS_P_H

#include <QHash>
#include <QVariant>
#include "Configurations.h"

namespace Targoman {
namespace Core {
namespace Private {

class clsConfigurationPrivate
{
public:
    struct stuConfigItem{
        QVariant::Type  Type;
        QVariant        Value;
        quint8          ValCount;
        QString         ShortSwitch;
        QString         LongSwitch;
        QString         ShortHelp;
        QString         LongHelp;
        isValidConfig_t fValidator;
    };

    QHash<QString, stuConfigItem>    Configs;
    QString                          ConfigFilePath;
    bool Initialized;
};


}
}
}
#endif // TARGOMAN_CORE_PRIVATE_CLSCONFIGURATIONS_P_H
