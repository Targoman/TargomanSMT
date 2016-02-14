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

#include "clsOutputComposer.h"
#include "Private/N-BestFinder/NBestSuggestions.h"

namespace Targoman{
namespace SMT {
namespace Private{
namespace OutputComposer {

using namespace Common;
using namespace InputDecomposer;
using namespace NBestFinder;
using namespace RuleTable;

/**
 * @brief clsOutputComposer::translationOutput  creates the translation output structure and returns the value
 *
 * this function is an interface that calls N-Best Finder and Input Decomposer modules and puts their return values together in a well formed manner
 * @return                                      created translation output strucutre
 */
stuTranslationOutput clsOutputComposer::translationOutput()
{
    stuTranslationOutput Output;

    Output.Translation = this->translationString();
    Output.TaggedSource = this->InputDecomposerRef.normalizedString();

    NBestSuggestions::Container_t NBestSuggestions =
            NBestSuggestions::retrieve(this->SearchGraphRef);

    for(NBestSuggestions::Container_t::ConstIterator NBestIter = NBestSuggestions.constBegin();
        NBestIter != NBestSuggestions.constEnd();
        ++NBestIter){
        QStringList TargetOptions;
        foreach(const clsTargetRule& TargetRule, NBestIter.value().TargetRules){
            QString TargetString = this->getTargetString(TargetRule, NBestIter.key());
                if (TargetOptions.contains(TargetString) == false)
                TargetOptions.append(TargetString);
        }

        Output.MetaInfo.append(
                    TranslationMetaInfo_t(
                        NBestIter.key(),
                        NBestIter.value().Pos,
                        TargetOptions));
    }

    return Output;
}

/**
 * @brief clsOutputComposer::translationString  creates the string representation of the best translation hypothesis useful for debugging purposes
 * @return
 */
QString clsOutputComposer::translationString()
{
    return this->nodeTranslation(this->SearchGraphRef.goalNode());
}


/**
 * @brief clsOutputComposer::getTargetString    helping function for creating the string representation of the translation hypothesis
 * @param _target                               the target rule for which the string representation is to be created, this will be a part of the hypothesis string representation
 * @param _sourcePos                            position of the source phrase for which the hypothesis holds the given target rule, used for extracting input attributes and using them in creating the string representation
 * @return
 */
QString clsOutputComposer::getTargetString(const clsTargetRule &_target, const stuPos &_sourcePhrasePos)
{
    if (_sourcePhrasePos.hasSingleItem() && _target.size() == 1) {
        clsToken Token = this->InputDecomposerRef.tokens().at(_sourcePhrasePos.start());
        if(Token.tagStr().size()) {
            if(Token.attrs().contains(enuDefaultAttrs::toStr(enuDefaultAttrs::Translation)))
                return Token.attrs().value(
                            enuDefaultAttrs::toStr(enuDefaultAttrs::Translation)).toString();
            if(Token.attrs().contains(enuDefaultAttrs::toStr(enuDefaultAttrs::DefaultTranslation)))
                return Token.attrs().value(
                            enuDefaultAttrs::toStr(enuDefaultAttrs::DefaultTranslation)).toString();
            if (Token.attrs().value(enuDefaultAttrs::toStr(enuDefaultAttrs::NoShow), false) == true)
                return QString();
        }
        // Absolutely just a fall-back for when the translation can not be shown using
        // a target language word
        if(_target.at(0) == gConfigs.EmptyLMScorer->unknownWordIndex()) {
            if(Token.attrs().contains(enuDefaultAttrs::toStr(enuDefaultAttrs::Translation)))
                return Token.attrs().value(
                            enuDefaultAttrs::toStr(enuDefaultAttrs::Translation)).toString();
            if(Token.attrs().contains(enuDefaultAttrs::toStr(enuDefaultAttrs::DefaultTranslation)))
                return Token.attrs().value(
                            enuDefaultAttrs::toStr(enuDefaultAttrs::DefaultTranslation)).toString();
            if (Token.attrs().value(enuDefaultAttrs::toStr(enuDefaultAttrs::NoShow), false) == true)
                return QString();
            if (Token.attrs().value(enuDefaultAttrs::toStr(enuDefaultAttrs::ShowSource), false) == true)
                return Token.string();
            //TODO toglle comment after checking why unknown is generated while there are valid target rules
            //Sample آقای فارسیجانی به آقای علی‌خانی گفت که آقای منصوری به شهر برازجان رفتند
            //return "<unk/>";
            return Token.string();
        }
    }

    if(_target.size() == 0)
        return QString();

    QString String;
    for(size_t i=0; i< _target.size(); ++i) {
        QList<int> Alignments = _target.wordLevelAlignment(i);
        if(Alignments.size() == 1) {
            int Alignment = Alignments.at(0);
            clsToken Token = this->InputDecomposerRef.tokens().at(Alignment + _sourcePhrasePos.start());
            if(Token.tagStr().size()) {
                if(Token.attrs().contains(enuDefaultAttrs::toStr(enuDefaultAttrs::Translation)))
                    String += Token.attrs().value(
                                enuDefaultAttrs::toStr(enuDefaultAttrs::Translation)).toString();
                if(Token.attrs().contains(enuDefaultAttrs::toStr(enuDefaultAttrs::DefaultTranslation)))
                    String += Token.attrs().value(
                                enuDefaultAttrs::toStr(enuDefaultAttrs::DefaultTranslation)).toString();
            }
        }
        QString TargetWordString = gConfigs.EmptyLMScorer->getWordByIndex(_target.at(i));
        String+= TargetWordString;
        if(Q_LIKELY(i != _target.size() - 1))
            String+= " ";
    }
    return String;
}

/**
 * @brief clsOutputComposer::nodeTranslation    helping function for creating the string representation of any partial or full translation hypothesis
 * @param _node                                 input translation hypothesis
 * @return
 */
QString clsOutputComposer::nodeTranslation(const SearchGraphBuilder::clsSearchGraphNode &_node)
{
    if(_node.isInvalid())
        return QString();

    QString PrevNodeTranslation = nodeTranslation(_node.prevNode());
    if(PrevNodeTranslation.isEmpty() == false)
        PrevNodeTranslation += " ";
    return PrevNodeTranslation +
            getTargetString(_node.targetRule(),
                            stuPos(_node.sourceRangeBegin(), _node.sourceRangeEnd()));
}

}
}
}
}
