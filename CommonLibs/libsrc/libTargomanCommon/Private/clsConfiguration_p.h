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

#ifndef TARGOMAN_COMMON_PRIVATE_CLSCONFIGURATION_P_H
#define TARGOMAN_COMMON_PRIVATE_CLSCONFIGURATION_P_H

#include <QHash>
#include <QVariant>
#include "Configuration.h"

namespace Targoman {
namespace Common {
namespace Private {

class clsConfigurationPrivate
{
public:
    void printHelp(const QString &_license);

    QHash<QString, intfConfigurable*>    Configs;
    QString                              ConfigFilePath;
    bool Initialized;

    //TODO add --save switch
};

}
}
}

#endif // TARGOMAN_COMMON_PRIVATE_CLSCONFIGURATION_P_H
