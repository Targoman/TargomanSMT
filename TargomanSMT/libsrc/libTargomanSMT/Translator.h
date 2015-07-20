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

class Translator
{
public:
    static void init(const QString _configFilePath);
    static void saveBinaryRuleTable(const QString& _filePath);
    static stuTranslationOutput translate(const QString& _inputStr,
                                          bool _justTranslationString = false,
                                          bool _isIXML = false);
};

}
}



#endif // TARGOMAN_CORE_CLSTRANSLATOR_H
