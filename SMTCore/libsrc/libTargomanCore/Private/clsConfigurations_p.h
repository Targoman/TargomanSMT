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

namespace Targoman {
namespace Core {
namespace Private {

class clsConfigurationPrivate
{
public:
    struct stuConfigItem{
        QVariant::Type Type;
        QVariant        Value;
    };

    QHash<QString, stuConfigItem>    Configs;
    bool Initialized;

};


}
}
}
#endif // TARGOMAN_CORE_PRIVATE_CLSCONFIGURATIONS_P_H
