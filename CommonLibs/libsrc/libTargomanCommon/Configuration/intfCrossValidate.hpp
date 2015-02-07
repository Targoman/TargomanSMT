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

#ifndef TARGOMAN_COMMON_CONFIGURATION_INTFCROSSVALIDATE_HPP
#define TARGOMAN_COMMON_CONFIGURATION_INTFCROSSVALIDATE_HPP

#include <QString>

namespace Targoman {
namespace Common {
namespace Configuration {

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
     * @param[out] _errorMessage error message for showing in case of not validity of configurable item.
     */
    virtual bool validate(const intfConfigurable& _item,
                          QString& _errorMessage) = 0;
};

}
}
}

#endif // TARGOMAN_COMMON_CONFIGURATION_INTFCROSSVALIDATE_HPP
