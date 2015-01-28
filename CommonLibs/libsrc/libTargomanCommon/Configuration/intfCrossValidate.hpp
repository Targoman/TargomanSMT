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
class intfCrossValidate{
public:
    intfCrossValidate(){}

    virtual bool validate(const intfConfigurable& _item,
                          QString& _errorMessage) = 0;
};

}
}
}

#endif // TARGOMAN_COMMON_CONFIGURATION_INTFCROSSVALIDATE_HPP
