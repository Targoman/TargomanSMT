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

/**
 * @brief The clsConfigurationPrivate class
 *
 * The pointer of this class is used as "pPrivate" data variable of Configuration class.
 * This class is used as integrated place for all options and configs of programs.
 */
class clsConfigurationPrivate
{
public:
    void printHelp(const QString &_license);
    /**
     * @brief This is a registry (Map) for all configs and arguments of all programs such as normalizer, the key value of
     * this Map, specifies the program and option of that program, and value of this Map, specifies the value of that
     * option.
     */
    QHash<QString, intfConfigurable*>    Configs;
    /**
     * @brief In case that we have a config file for arguments of our programs, we put the address of
     * that config file in #ConfigFilePath
     */
    QString                              ConfigFilePath;
    /**
     * @brief This variable will be true if init() function of Configuration class works without any exceptions.
     */
    bool Initialized;

    /**
      @todo add --save switch
    */
};

}
}
}

#endif // TARGOMAN_COMMON_PRIVATE_CLSCONFIGURATION_P_H
