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

#ifndef TARGOMAN_APPS_CLSTRANSLATIONJOB_H
#define TARGOMAN_APPS_CLSTRANSLATIONJOB_H

#include <QVariantList>
#include "libTargomanSMT/Translator.h"

namespace Targoman {
namespace Apps {

class clsTranslationJob
{
public:
    clsTranslationJob(bool _brief, bool _keepAsSource);
    QVariantList doJob(const QString& _inputStr);

private:
    SMT::stuTranslationOutput mapLineTranslation(const QString& _line);
    SMT::stuTranslationOutput mapSentenceTranslation(const QString& _ixml);
    void reduceLineTranslation(QVariantList &_result, const SMT::stuTranslationOutput& _intermediate);
    void reduceSentenceTranslation(SMT::stuTranslationOutput& _result, const SMT::stuTranslationOutput& _intermediate);

private:
    bool Brief;
    bool KeepAsSource;
};

}
}

#endif // TARGOMAN_APPS_CLSTRANSLATIONJOB_H
