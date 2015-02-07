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

#ifndef TARGOMAN_COMMON_CONFIGURATION_VALIDATORS_H
#define TARGOMAN_COMMON_CONFIGURATION_VALIDATORS_H

#include "libTargomanCommon/Configuration/intfCrossValidate.hpp"
#include "libTargomanCommon/Constants.h"

namespace Targoman {
namespace Common {
namespace Configuration {

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
}
}
}
#endif // TARGOMAN_COMMON_CONFIGURATION_VALIDATORS_H
