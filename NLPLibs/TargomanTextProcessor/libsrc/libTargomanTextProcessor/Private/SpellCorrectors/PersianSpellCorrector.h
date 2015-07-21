/******************************************************************************
 * Targoman: A robust Statistical Machine Translation framework               *
 *                                                                            *
 * Copyright 2014-2015 by ITRC <http://itrc.ac.ir>                            *
 *                                                                            *
 * This file is part of Targoman.                                             *
 *                                                                            *
 * Targoman is free software: you can redistribute it and/or modify           *
 * it under the terms of the GNU Lesser General Public License as published   *
 * by the Free Software Foundation, either version 3 of the License, or       *
 * (at your option) any later version.                                        *
 *                                                                            *
 * Targoman is distributed in the hope that it will be useful,                *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of             *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              *
 * GNU Lesser General Public License for more details.                        *
 * You should have received a copy of the GNU Lesser General Public License   *
 * along with Targoman. If not, see <http://www.gnu.org/licenses/>.           *
 *                                                                            *
 ******************************************************************************/
/**
 * @author S. Mohammad M. Ziabary <ziabary@targoman.com>
 * @author Behrooz Vedadian <vedadian@targoman.com>
 * @author Saeed Torabzadeh <saeed.torabzadeh@targoman.com>
 */

#ifndef TARGOMAN_NLPLIBS_TARGOMANTP_PRIVATE_SPELLCORRECTORS_CLSPERSIANSPELLCORRECTOR_H
#define TARGOMAN_NLPLIBS_TARGOMANTP_PRIVATE_SPELLCORRECTORS_CLSPERSIANSPELLCORRECTOR_H

#include <QSet>
#include <QRegExp>
#include "libTargomanTextProcessor/Private/SpellCorrector.h"

namespace Targoman {
namespace NLPLibs {
namespace TargomanTP{
namespace Private {
namespace SpellCorrectors {

class PersianSpellCorrector : public intfSpellCorrector
{
public:
    PersianSpellCorrector();
    bool postInit(const QVariantHash _settings);
    QString process(const QStringList& _tokens);
    bool canBeCheckedInteractive(const QString &_inputWord) const;
    void storeAutoCorrectTerm(const QString& _from, const QString& _to);

private:
    QString processStartingWithBi_Ba_Na(const QSet<QString>& _set,
                                        const QString& _prefix,
                                        const QString& _postfix);
    QString processVerbs(const QString& _prefix,
                         const QString _postfix);
    QString processHa(const QString& _prefix,
                      const QString& _complexWord,
                      const QString& _postfix);
    QString processTar_Tarin(const QSet<QString> &_set,
                             const QString& _prefix,
                             const QString& _complexWord,
                             const QString& _postfix,
                             bool _checkVerb = true);

private:
    QSet<QString>            Nouns;                 /**< A set to store all Nouns from Persian SpellCorrector 'Noun' config file. */
    QSet<QString>            Adjectives;            /**< A set to store all Adjectives from Persian SpellCorrector 'Adjective' config file. */
    QSet<QString>            CanStartWithBi_Ba;     /**< A set to store all Adjectives that can be started with Bi or Ba from Persian SpellCorrector 'StartWith_Bi_Ba' config file. */
    QSet<QString>            CanStartWithNa;        /**< A set to store all Adjectives that can be started with Na from Persian SpellCorrector 'StartWith_Na' config file. */
    QSet<QString>            Space2ZWNJ;            /**< A set to store all compound words that should join to each other with ZWNJ from Persian SpellCorrector 'Space2ZWNJ' config file. */
    QSet<QString>            VerbStemPresent;       /**< A set to store all present verb stems from Persian SpellCorrector 'verbStemPresent' config file. */
    QSet<QString>            VerbStemPast;          /**< A set to store all past verb stems from Persian SpellCorrector 'verbStemPast' config file. */
    QSet<QString>            HamzeAllowed;          /**< A set to store all words that Hamze or Mad is allowed from Persian SpellCorrector 'HamzeOrMadAllowed' config file. */
    QSet<QString>            AdverbsEndWithFathatan;      /**< A set to store all adverbs that end with An from Persian SpellCorrector 'AdverbsEndWithAn' config file. */

    QRegExp                  RxInteractiveChars;    /**< A RegExp to match any interactive chars like ؤ,ئ,إ,أ,آ */
    QRegExp                  RxPresentImperfect;    /**< A RegExp to match last parts of any present imperfect verb. */
    QRegExp                  RxEndPresentImperfect; /**< A RegExp to match any present imperfect verb. */
    QRegExp                  RxPastImperfect;       /**< A RegExp to match last parts of any past imperfect verb. */
    QRegExp                  RxEndPastImperfect;    /**< A RegExp to match any past imperfect verb. */
    QRegExp                  RxPastPerfect;         /**< A RegExp to match last parts of any past perfect verb. */
    QRegExp                  RxEndPastPerfect;    /**< A RegExp to match any past imperfect verb. */
    QRegExp                  RxEndVerbPerfect;      /**< A RegExp to match any perfect verb. */
    QRegExp                  RxVerbPerfect;         /**< A RegExp to match last parts of any perfect verb. */
    QRegExp                  RxHa;                  /**< A RegExp to match Persian all variations of 'Ha' plural maker */
    QRegExp                  RxEndWithHa;           /**< A RegExp to match any Persian plural noun that ends with 'Ha'*/
    QRegExp                  RxAn;                  /**< A RegExp to match Persian all variations of 'An' plural maker */
    QRegExp                  RxEndWithAn;           /**< A RegExp to match any Persian plural noun that ends with 'An'*/
    QRegExp                  RxPossesive;           /**< A RegExp to match all kinds of Persian Possesives*/
    QRegExp                  RxEndWithPossesive;    /**< A RegExp to match any Persian word that have any variation of possesives*/
};

}
}
}
}
}
#endif // TARGOMAN_NLPLIBS_TARGOMANTP_PRIVATE_SPELLCORRECTOR_CLSPERSIANSPELLCORRECTOR_H
