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

#ifndef TARGOMAN_NLPLIBS_TARGOMANTP_PRIVATE_SPELLCORRECTOR_H
#define TARGOMAN_NLPLIBS_TARGOMANTP_PRIVATE_SPELLCORRECTOR_H

#include <QHash>
#include <QVariantHash>
#include "ISO639.h" //From https://github.com/softnhard/ISO639

#include "../TextProcessor.h"
#include "../Private/Normalizer.h"

namespace Targoman {
namespace NLPLibs {
namespace TargomanTP{
namespace Private {

TARGOMAN_ADD_EXCEPTION_HANDLER(exSpellCorrector, exTextProcessor);

/**
 * @brief The intfSpellCorrector class is an interface for spell correction tasks.
 * Language specific spell corrector classes will be derived form this class.
 */
class intfSpellCorrector
{
protected:
    struct stuConfigType{
        QString Name;
        bool    IsKeyVal;
        union{
            QHash<QString, QString>* KeyValStorage;
            QSet<QString>*           ListStorage;
        };

        stuConfigType(const QString& _name, QHash<QString, QString>* _storage){
            this->Name = _name;
            this->IsKeyVal = true;
            this->KeyValStorage = _storage;
        }

        stuConfigType(const QString& _name, QSet<QString>* _storage){
            this->Name = _name;
            this->IsKeyVal = false;
            this->ListStorage = _storage;
        }

        stuConfigType(){
            this->KeyValStorage = NULL;
        }
    };

public:
    virtual ~intfSpellCorrector(){}
    inline bool active() const {return this->Active;}
    inline const QHash<QString, QString>&  autoCorrectTerms(){return this->AutoCorrectTerms;}
    inline int maxAutoCorrectTokens(){return this->MaxAutoCorrectTokens;}
    bool init(const QString &_baseConfigPath, const QVariantHash _settings);

    virtual QString process(const QStringList& _tokens) = 0;
    virtual bool canBeCheckedInteractive(const QString& _inputWord) const = 0;
    virtual void storeAutoCorrectTerm(const QString& _from, const QString& _to) = 0;
    /**
     * @brief This function just checks #AutoCorrectTerms HashMap for input token.
     * @param _token input token.
     * @return corrected or empty token.
     */
    virtual QString process(const QString& _token){
        QString Normalized = this->AutoCorrectTerms.value(_token);
        if (Normalized.size()){
            QString SpellCorrected = this->process(QStringList()<<Normalized);
            if (SpellCorrected.size())
                return SpellCorrected;
            else
                return Normalized;
        }else{
            return this->process(QStringList()<<_token);
        }
    }

protected:
    intfSpellCorrector(const char _code[2]);
    virtual bool postInit(const QVariantHash _settings) = 0;

protected:
    QHash<QString, QString>           AutoCorrectTerms; /**< A list of terms and their correct forms that can be corrected directly.*/
    QList<stuConfigType>              ConfigTypes;      /**< A list of containers which store spellCorrector configuration data.*/
    int  MaxAutoCorrectTokens;                          /**< Max number of consecutive words that should be checked in a group, for spell corrector.*/
    QString AutoCorrectFile;                            /**< Does spell corrector for this language is active or not.  */
    bool Active;                                        /**< Does spell corrector for this language is active or not.  */
    QString Lang;                                       /**< Name of Language.  */

    Normalizer& refNormalizerInstance;                  /**< An instance of Normalizer class for faster access to normalizer class */
};

class SpellCorrector
{
public:
    static SpellCorrector& instance() {
        static SpellCorrector* Instance = NULL;
        return Q_LIKELY(Instance) ? *Instance : *(Instance = new SpellCorrector);
    }

    QString process(const QString& _lang, const QString& _inputStr, INOUT bool& _changed, bool _interactive);
    void init(const QString& _baseConfigPath, const QHash<QString, QVariantHash> &_settings);

private:
    SpellCorrector();
    Q_DISABLE_COPY(SpellCorrector)
    inline void registerProcessor(const char _code[2], intfSpellCorrector* _instance){
        this->Processors.insert(_code, _instance);
    }

private:
    QHash<QString, intfSpellCorrector*> Processors;     /**< A HashMap that key is language name and value is its respective language based spell corrector. */
    Normalizer& refNormalizerInstance;                  /**< An instance of Normalizer class for faster access to normalizer class. */

    friend class intfSpellCorrector;
};

}
}
}
}
#endif // TARGOMAN_NLPLIBS_TARGOMANTP_PRIVATE_SPELLCORRECTOR_H
