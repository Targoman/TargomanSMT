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
    intfSpellCorrector();
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
    virtual bool postInit(const QVariantHash _settings) = 0;

protected:
    QHash<QString, QString>           AutoCorrectTerms; /**< A list of terms and their correct forms that can be corrected directly.*/
    QList<stuConfigType>              ConfigTypes;      /**< A list of containers which store spellCorrector configuration data.*/
    int  MaxAutoCorrectTokens;                          /**< Max number of consecutive words that should be checked in a group, for spell corrector.*/
    QString AutoCorrectFile;                            /**< Does spell corrector for this language is active or not.  */
    bool Active;                                        /**< Does spell corrector for this language is active or not.  */
    QString Lang;                                       /**< Name of Language.  */

    Normalizer& NormalizerInstance;                     /**< An instance of Normalizer class for faster access to normalizer class */
};

class SpellCorrector
{
public:
    static SpellCorrector& instance() {return Q_LIKELY(Instance) ? *Instance : *(Instance = new SpellCorrector);}

    QString process(const QString& _lang, const QString& _inputStr, INOUT bool& _changed, bool _interactive);
    void init(const QString& _baseConfigPath, const QHash<QString, QVariantHash> &_settings);

private:
    SpellCorrector();
    Q_DISABLE_COPY(SpellCorrector)

private:
    QHash<QString, intfSpellCorrector*> Processors;     /**< A HashMap that key is language name and value is its respective language based spell corrector. */
    static SpellCorrector* Instance;                    /**< Static instnace of spellCorrector class. */
    Normalizer& NormalizerInstance;                     /**< An instance of Normalizer class for faster access to normalizer class. */
};

}
}
}
}
#endif // TARGOMAN_NLPLIBS_TARGOMANTP_PRIVATE_SPELLCORRECTOR_H
