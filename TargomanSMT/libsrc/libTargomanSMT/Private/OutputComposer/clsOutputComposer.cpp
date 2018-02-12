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
#include "Private/N-BestFinder/NBestPaths.h"

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
stuTranslationOutput clsOutputComposer::getTranslationOutput(enuOutputFormat::Type _outputFormat)
{
    switch(_outputFormat) {
    case enuOutputFormat::BestTranslationAndPhraseSuggestions:
        return this->bestTranslationAndPhraseSuggestions();
    case enuOutputFormat::NBestTranslations:
        return this->nBestTranslations();
    default:
        return this->justBestTranslation();
    }
}

stuTranslationOutput clsOutputComposer::justBestTranslation()
{
    stuTranslationOutput Output;
    Output.TaggedSource = this->InputDecomposerRef.normalizedString();
    Output.Translations.append(this->nodeTranslation(this->SearchGraphRef.goalNode()));
    return Output;
}

stuTranslationOutput clsOutputComposer::bestTranslationAndPhraseSuggestions()
{
    stuTranslationOutput Output = this->justBestTranslation();

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

        Output.BestTranslationPhraseAlternatives.append(
                    PhraseAlternatives_t(
                        NBestIter.key(),
                        NBestIter.value().Pos,
                        TargetOptions));
    }

    return Output;
}

stuTranslationOutput clsOutputComposer::nBestTranslations()
{
    stuTranslationOutput Output;

    Output.TaggedSource = this->InputDecomposerRef.normalizedString();

    NBestPaths::Container_t NBestPaths =
            NBestPaths::retrieve(this->SearchGraphRef, *this);

    for(NBestPaths::Container_t::ConstIterator NBestIter = NBestPaths.constBegin();
        NBestIter != NBestPaths.constEnd();
        ++NBestIter) {


        Output.Translations.append(this->pathTranslation(NBestIter->getNodes()));
        stuTranslationOutput::stuCostElements CostElements;

        foreach(FeatureFunction::intfFeatureFunction* FeatureFunction, gConfigs.ActiveFeatureFunctions.values()) {

            size_t index = FeatureFunction->getDataIndex();

            if(NBestIter->featureFunctionDataAt(index) != NULL){


                QString FeatureFunctionName = FeatureFunction->name();
                QStringList FeatureElementNames = FeatureFunction->getFeatureElementNames();

                QVector<Cost_t> Costs = NBestIter->featureFunctionDataAt(index)->costElements();
                for(int j = 0; j < Costs.size(); j++) {
                    CostElements.Elements.push_back(stuTranslationOutput::stuCostElement(
                        FeatureElementNames .size() > j ? FeatureFunctionName + "/" + FeatureElementNames [j] : FeatureFunctionName,
                        Costs[j]
                    ));
                }
            }
        }

        CostElements.Total = NBestIter->getTotalCost();

        Output.TranslationsCostElements.push_back(CostElements);
    }

    return Output;
}

QString clsOutputComposer::pathTranslation(const QList<SearchGraphBuilder::clsSearchGraphNode> &_path)
{
    QString result;
    foreach(const SearchGraphBuilder::clsSearchGraphNode& Node, _path) {
        if(result.isEmpty() == false)
            result += " ";
        result += this->getTargetString(Node.targetRule(),
                                        stuPos(Node.sourceRangeBegin(), Node.sourceRangeEnd()));
    }
    return result;
}


/**
 * @brief clsOutputComposer::getTargetString    helping function for creating the string representation of the translation hypothesis
 * @param _target                               the target rule for which the string representation is to be created, this will be a part of the hypothesis string representation
 * @param _sourcePos                            position of the source phrase for which the hypothesis holds the given target rule, used for extracting input attributes and using them in creating the string representation
 * @return
 */
QString clsOutputComposer::getTargetString(const clsTargetRule &_target, const stuPos &_sourcePhrasePos)
{
//    std::cout << _target.size() << " ||| " << _target.toStr().toStdString() << std::endl;
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
            return Token.string();
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
//        std::cout << "--- " << Alignments.size() << std::endl;
        QString TargetWordString = gConfigs.EmptyLMScorer->getWordByIndex(_target.at(i));
        if(Alignments.size() == 1) {
            int Alignment = Alignments.at(0);
            clsToken Token = this->InputDecomposerRef.tokens().at(Alignment + _sourcePhrasePos.start());
//            std::cout << "*** " << Token.string().toStdString() << " " << Token.tagStr().toStdString()<< std::endl;
            if(Token.tagStr().size() && TargetWordString.contains("<")) {
                if(Token.attrs().contains(enuDefaultAttrs::toStr(enuDefaultAttrs::Translation))){
                    String += Token.attrs().value(
                                enuDefaultAttrs::toStr(enuDefaultAttrs::Translation)).toString();
                    if(Q_LIKELY(i != _target.size() - 1))
                        String+= " ";
                    continue;
                }
                if(Token.attrs().contains(enuDefaultAttrs::toStr(enuDefaultAttrs::DefaultTranslation))){
                    String += Token.attrs().value(
                                enuDefaultAttrs::toStr(enuDefaultAttrs::DefaultTranslation)).toString();
//                if(Token.attrs().empty()){
//                    String += Token.string();
//                }
                if(Q_LIKELY(i != _target.size() - 1))
                    String+= " ";
                continue;
                }
            }
        }

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
