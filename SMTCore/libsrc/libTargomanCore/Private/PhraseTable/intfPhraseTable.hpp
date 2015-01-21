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

#ifndef TARGOMAN_CORE_PRIVATE_PHRASETABLE_INTFPHRASETABLE_H
#define TARGOMAN_CORE_PRIVATE_PHRASETABLE_INTFPHRASETABLE_H

#include "libTargomanCommon/Macros.h"
#include "libTargomanCommon/tmplExpirableCache.hpp"
#include "Private/Input/clsInput.h"

namespace Targoman {
namespace Core {
namespace Private {
namespace PhraseTable{

TARGOMAN_ADD_EXCEPTION_HANDLER(exPhraseTable, exTargomanCore);

TARGOMAN_DEFINE_ENHANCED_ENUM_BEGIN(enuPTFileFormat)
    Moses,
    Jane,
    JaneBinary,
    Binary
TARGOMAN_DEFINE_ENHANCED_ENUM_STRINGS
    "Moses",
    "Jane",
    "JaneBinary",
    "Binary"
TARGOMAN_DEFINE_ENHANCED_ENUM_END

class intfPhraseTable
{
public:
    intfPhraseTable(){}

    virtual void init() = 0;
    virtual void newSentence() = 0;
    virtual bool isBinary() = 0;

/*    virtual void sortRuleTrees(const std::vector<double> &_scalingFactors,
                               unsigned _rulesCostsBegin,
                               size_t _observationHistogramSize = 0) = 0;*/
    virtual void partialSortRuleTreesWithLM(const std::vector<double> &_scalingFactors,
                                            unsigned _rulesCostsBegin,
                                            size_t _observationHistogramSize) {}

    //RulePrefixTree *getRulesPrefixTree() { return rulesPrefixTree_; }
    //void setRulesPrefixTree(RulePrefixTree *rulesPrefixTree) { rulesPrefixTree_ = rulesPrefixTree; }

   /* virtual std::string getConfigText() const;
    static std::string getUsage(const std::string &indentation, bool showNamesHelp = true);
    static std::string getMan();*/

   /* const std::vector<std::string> getCostsNames() { return costsNames_; }

    size_t getNCosts() const { return nCosts_; }

    Word getUnknownWord() const { return unknownWord_; }

    std::vector<Word> getReverseSourcePart(const SourcePartInfo &sourcePart) const;
    int getOrAddAdditionalInfoPosition(const std::string &name){//implemented but discarded}
    int getAdditionalInfoPosition(const std::string &name) const {//implemented but discarded}
    ConstAlphabetRef getSourceAlphabet() const { return sourceAlphabet_; }
    ConstAlphabetRef getTargetAlphabet() const { return targetAlphabet_; }
    char getNonTerminalIndicator() const { return nonTerminalIndicator_; }
*/
protected:
    static const QString& baseConfigPath(){return "PhraseTable";}
    friend class clsRuleNode;

protected:
    QString ActorUUID;

    /*
        static Core::ParameterString       paramRulesFileName_;
        static Core::ParameterString       paramNonTerminalIndicator_;
        static Core::ParameterIntVector    paramWhichCosts_;
        static Core::ParameterStringVector paramCostsNames_;
        static Core::ParameterString       paramUnknownString_;
        static Core::ParameterBool         paramDoL1o_;

        StaticAlphabetRef sourceAlphabet_;
        StaticAlphabetRef targetAlphabet_;

        RulePrefixTree *rulesPrefixTree_;
        RulePrefixTree *lexicalPrefixTree_; //TODO what is this, how is this used?

        char nonTerminalIndicator_;

        std::string stringUnknownWord_;
        Word unknownWord_;

        Word maxKnownSourceWord_;

        quint32 maxNumberOfNonTerminals_;

        size_t nCosts_;

        std::vector<int> whichCosts_;
        std::vector<std::string> costsNames_;

        bool doL1o_;

        friend Core::Ref<RuleSet> createRuleSet(const Core::Configuration &config,
                                                StaticAlphabetRef sourceAlphabet, StaticAlphabetRef targetAlphabet);

     */

    /*
    static const quint32 binaryRuleFormatVersion_;

    // Ids for the additional information present in the rules
    std::map<std::string, unsigned> additionalInfoMapping_;
    std::vector<std::string> additionalInfoNames_;

    std::vector<AdditionalRuleInfoReader *> additionalRuleInfoReaders_;

    friend AdditionalRuleInfoReader *getAdditionalRuleInfoReader(const std::string&, RuleSet*);
        // We make this function a friend, so that it can take the components the
        // readers need, without cluttering the interface
    */
};

}
}
}
}

#endif // TARGOMAN_CORE_PRIVATE_PHRASETABLE_INTFPHRASETABLE_H
