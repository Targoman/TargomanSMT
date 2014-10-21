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

#ifndef TARGOMAN_NLPLIBS_PRIVATE_SPELLCORRECTOR_H
#define TARGOMAN_NLPLIBS_PRIVATE_SPELLCORRECTOR_H

#include <QHash>
#include <QVariantHash>
#include "ISO639.h" //From https://github.com/softnhard/ISO639

#include "libTargomanTextProcessor/TextProcessor.h"

namespace Targoman {
namespace NLPLibs {
namespace Private {

TARGOMAN_ADD_EXCEPTION_HANDLER(exSpellCorrecter, exTextProcessor);

class intfSpellCorrector
{
public:
    intfSpellCorrector(){this->Active = false;}
    virtual ~intfSpellCorrector(){}

    inline bool active(){return this->Active;}
    inline QString& autoCorrectFile(){return this->AutoCorrectFile;}
    inline uint maxAutoCorrectTokens(){return this->MaxAutoCorrectTokens;}

public:
    virtual bool init(const QVariantHash _settings) = 0;
    virtual QString process(const QStringList& _tokens) = 0;
    virtual bool canBeCheckedInteractive(const QString& _inputWord) = 0;

public:
    QHash<QString, QString>  AutoCorrectTerms;

protected:
    uint  MaxAutoCorrectTokens;
    QString AutoCorrectFile;
    bool Active;
};

class SpellCorrector
{
public:
    static SpellCorrector& instance() {return Q_LIKELY(Instance) ? *Instance : *(Instance = new SpellCorrector);}

    QString process(const QString& _lang, const QString& _inputStr, bool _interactive);
    bool init(const QHash<QString, QVariantHash> &_settings);

private:
    SpellCorrector();

private:
    QHash<QString, intfSpellCorrector*> Processors;
    static SpellCorrector* Instance;
};

}
}
}
#endif // TARGOMAN_NLPLIBS_PRIVATE_SPELLCORRECTOR_H
