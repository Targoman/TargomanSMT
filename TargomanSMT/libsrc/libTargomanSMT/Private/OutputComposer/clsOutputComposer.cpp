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
 @author Behrooz Vedadian <vedadian@gmail.com>
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
    Output.NormalizedSource = this->InputDecomposerRef.normalizedString();

    NBestSuggestions::Container_t NBestSuggestions =
            NBestSuggestions::retrieve(this->SearchGraphRef);

    for(NBestSuggestions::Container_t::ConstIterator NBestIter = NBestSuggestions.constBegin();
        NBestIter != NBestSuggestions.constEnd();
        ++NBestIter){
        QStringList TargetOptions;
        foreach(const clsTargetRule& TargetRule, NBestIter.value().TargetRules)
                TargetOptions.append(this->getTargetString(TargetRule, NBestIter.key()));

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
QString clsOutputComposer::getTargetString(const clsTargetRule &_target, const stuPhrasePos &_sourcePos)
{
    if (_sourcePos.isSingleWord() &&
        _target.size() == 1 &&
        _target.at(0) == gConfigs.EmptyLMScorer->unknownWordIndex()) {
        clsToken Token = this->InputDecomposerRef.tokens().at(_sourcePos.start());
        if (Token.attrs().value(enuDefaultAttrs::toStr(enuDefaultAttrs::NoShow), false) == true)
            return QString();
        if (Token.attrs().value(enuDefaultAttrs::toStr(enuDefaultAttrs::ShowSource), false) == true)
            return Token.string();
        if (Token.tagStr().size())
            return Token.attrs().value(
                        enuDefaultAttrs::toStr(enuDefaultAttrs::Translation),
                        Token.string()).toString();
        return Token.string();
    }

    if(_target.size() == 0)
        return QString();

    QString String = gConfigs.EmptyLMScorer->getWordByIndex(_target.at(0));
    for(size_t i=1; i< _target.size(); ++i){
        QString Token = gConfigs.EmptyLMScorer->getWordByIndex(_target.at(i));
        if (Token.isEmpty()){
            //TODO Tag management
            String += " <TAG("+QString::number(_target.at(i))+")>";
        }else
            String+= " " + Token;
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
                            stuPhrasePos(_node.sourceRangeBegin(), _node.sourceRangeEnd()));
}

}
}
}
}
