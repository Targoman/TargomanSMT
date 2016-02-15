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

#ifndef SMT

#ifndef TARGOMAN_SMT_PRIVATE_SPECIAL_TOKEN_HANDLER_OOV_TRANSLITERATENAMEDENTITIES_H
#define TARGOMAN_SMT_PRIVATE_SPECIAL_TOKEN_HANDLER_OOV_TRANSLITERATENAMEDENTITIES_H

#include "Types.h"
#include "intfOOVHandlerModule.hpp"
#include "Private/InputDecomposer/clsInput.h"
#include "Private/Proxies/NamedEntityRecognition/intfNamedEntityRecognizer.h"
#include "Private/Proxies/Transliteration/intfTransliterator.h"
#include "Private/RuleTable/clsTargetRule.h"

namespace Targoman{
namespace SMT {
namespace Private{
namespace SpecialTokenHandler {
namespace OOV{

class TransliterateNamedEntities : public intfOOVHandlerModule{
public:
    explicit TransliterateNamedEntities():
        intfOOVHandlerModule(/*this->moduleName()*/),
        refTransliterator(
            *gConfigs.Transliterator.getInstance<Proxies::Transliteration::intfTransliterator>())
        {
        this->refTransliterator.init(Configuration::ConfigManager::instance().configSettings());
    }

    ~TransliterateNamedEntities();

    /**
     * @brief process       Sets type of OOV handling and returns an appropriate target rule.
     * @param[in] _token    Input string of token.
     * @param[out] _attrs   Type of  OOV handling will be stored in this argument.
     * @return              Returns an invalid target rule.
     */

    RuleTable::clsTargetRule process(const QString &_token, QVariantMap& _attrs){
        if(_attrs.contains(NER_TAG_ATTR_KEY) == false ||
           _attrs.value(NER_TAG_ATTR_KEY) == NER_TAG_OTHER)
            return *RuleTable::pInvalidTargetRule;

        QString TargetWord = this->refTransliterator.transliterate(_token);
        QList<WordIndex_t> TargetPhrase;
        TargetPhrase.append(gConfigs.EmptyLMScorer->getWordIndex(TargetWord));
        _attrs.insert(
                    InputDecomposer::enuDefaultAttrs::toStr(InputDecomposer::enuDefaultAttrs::Translation),
                    TargetWord);
        return RuleTable::clsTargetRule::createZeroCostTargetRule(TargetPhrase, true);
    }

    bool isReusable() {return true;}

private:
    Proxies::Transliteration::intfTransliterator& refTransliterator;
    TARGOMAN_DEFINE_SINGLETON_MODULE(TransliterateNamedEntities);
};

}
}
}
}
}

#endif // TARGOMAN_SMT_PRIVATE_SPECIAL_TOKEN_HANDLER_OOV_TRANSLITERATENAMEDENTITIES_H

#endif
