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

#ifndef TARGOMAN_NLPLIBS_PRIVATE_SPELLCORRECTOR_CLSPERSIANSPELLCORRECTOR_H
#define TARGOMAN_NLPLIBS_PRIVATE_SPELLCORRECTOR_CLSPERSIANSPELLCORRECTOR_H

#include <QSet>
#include <QRegExp>
#include "libTargomanTextProcessor/Private/SpellCorrector.h"

namespace Targoman {
namespace NLPLibs {
namespace Private {

class clsPersianSpellCorrector : public intfSpellCorrector
{
public:
    clsPersianSpellCorrector();
    bool init(const QVariantHash _settings);
    QString process(const QStringList& _tokens);
    bool canBeCheckedInteractive(const QString &_inputWord);


private:
    QSet<QString>            CanAppendHa;
    QSet<QString>            CanAppendTar;
    QSet<QString>            CanStartWithMi;
    QSet<QString>            CanStartWithBi_Ba;
    QSet<QString>            RemoveSpaces;

    QRegExp                  RxInteractiveChars;
    QRegExp                  RxVerbFinisher;
    QRegExp                  RxHa;

};

}
}
}
#endif // TARGOMAN_NLPLIBS_PRIVATE_SPELLCORRECTOR_CLSPERSIANSPELLCORRECTOR_H
