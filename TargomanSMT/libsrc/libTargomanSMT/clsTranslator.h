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

#ifndef TARGOMAN_CORE_CLSTRANSLATOR_H
#define TARGOMAN_CORE_CLSTRANSLATOR_H

#include <QMap>
#include <QStringList>
#include "libTargomanSMT/Types.h"

namespace Targoman{
namespace SMT {

namespace Private {
class clsTranslatorPrivate;
}
class clsTranslator
{
public:
    clsTranslator(const QString &_inputStr);
    ~clsTranslator();

    static void init(const QString _configFilePath);

    //Input language is predefined
    stuTranslationOutput translate(bool _justTranslationString = false);

private:
    QScopedPointer<Private::clsTranslatorPrivate> pPrivate;
};

}
}



#endif // TARGOMAN_CORE_CLSTRANSLATOR_H
