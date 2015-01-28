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
 @author Behrooz Vedadian <vedadian@gmail.com>
 */

#ifndef TARGOMAN_COMMON_CONFIGURATION_TMPLINSTANTIATOR_HPP
#define TARGOMAN_COMMON_CONFIGURATION_TMPLINSTANTIATOR_HPP

#include "libTargomanCommon/Configuration/intfConfigurable.hpp"

namespace Targoman {
namespace Common {
namespace Configuration {

template <class Type_t> class tmplInstantiator : public intfConfigurable{
    tmplInstantiator(const QString&  _configPath,
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
        this->ArgCount = -1;
    }

    Type_t* getInstance() {
        new Type_t;
    }
};

}
}
}
#endif // TARGOMAN_COMMON_CONFIGURATION_TMPLINSTANTIATOR_HPP
