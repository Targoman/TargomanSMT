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
    bool postInit(const QVariantHash _settings);
    QString process(const QStringList& _tokens);
    bool canBeCheckedInteractive(const QString &_inputWord) const;
    void storeAutoCorrectTerm(const QString& _from, const QString& _to);

private:
    QString processStartingWithBi_Ba_Na(const QSet<QString>& _set, const QString& _prefix, const QString& _postfix);
    QString processVerbs(const QString& _prefix, const QString _postfix);

private:
    QSet<QString>            Nouns;
    QSet<QString>            Adjectives;
    QSet<QString>            CanStartWithBi_Ba;
    QSet<QString>            CanStartWithNa;
    QSet<QString>            Space2ZWNJ;
    QSet<QString>            VerbStemPresent;
    QSet<QString>            VerbStemPast;
    QSet<QString>            HamzeAllowed;

    QRegExp                  RxInteractiveChars;
    QRegExp                  RxPresentImperfect;
    QRegExp                  RxEndPresentImperfect;
    QRegExp                  RxPastImperfect;
    QRegExp                  RxEndPastImperfect;
    QRegExp                  RxEndVerbPerfect;
    QRegExp                  RxVerbPerfect;
    QRegExp                  RxHa;
    QRegExp                  RxEndWithHa;
    QRegExp                  RxAn;
    QRegExp                  RxEndWithAn;

};

}
}
}
#endif // TARGOMAN_NLPLIBS_PRIVATE_SPELLCORRECTOR_CLSPERSIANSPELLCORRECTOR_H
