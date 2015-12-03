################################################################################
#   Targoman: A robust Statistical Machine Translation framework
#
#   Copyright 2014-2015 by ITRC <http://itrc.ac.ir>
#
#   This file is part of Targoman.
#
#   Targoman is free software: you can redistribute it and/or modify
#   it under the terms of the GNU Lesser General Public License as published by
#   the Free Software Foundation, either version 3 of the License, or
#   (at your option) any later version.
#
#   Targoman is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU Lesser General Public License for more details.
#
#   You should have received a copy of the GNU Lesser General Public License
#   along with Targoman. If not, see <http://www.gnu.org/licenses/>.
################################################################################
BasePath=".."

# +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-#
HEADERS += \
    libTargomanSMT/Private/InputDecomposer/clsInput.h \
    libTargomanSMT/Private/InputDecomposer/clsToken.h \
    libTargomanSMT/Private/GlobalConfigs.h \
    libTargomanSMT/Private/OutputComposer/clsOutputComposer.h \
    libTargomanSMT/Private/SearchGraphBuilder/clsLexicalHypothesis.h \
    libTargomanSMT/Private/SearchGraphBuilder/clsSearchGraphNode.h \
    libTargomanSMT/Private/SearchGraphBuilder/clsCardinality.h \
    libTargomanSMT/Private/SearchGraphBuilder/clsHypothesisHolder.hpp \
    libTargomanSMT/Private/SearchGraphBuilder/clsSearchGraph.h \
    libTargomanSMT/Private/Proxies/intfLMSentenceScorer.hpp \
    libTargomanSMT/Private/Proxies/LanguageModel/clsTargomanLMProxy.h \
    libTargomanSMT/Private/FeatureFunctions/intfFeatureFunction.hpp \
    libTargomanSMT/Private/FeatureFunctions/LexicalReordering/LexicalReordering.h \
    libTargomanSMT/Private/RuleTable/clsRuleNode.h \
    libTargomanSMT/Private/RuleTable/clsTargetRule.h \
    libTargomanSMT/Private/RuleTable/clsJanePlainRuleTable.h \
    libTargomanSMT/Private/RuleTable/intfRuleTable.hpp \
    libTargomanSMT/Private/FeatureFunctions/PhraseTable/PhraseTable.h \
    libTargomanSMT/Private/SpecialTokenHandler/OOVHandler/OOVHandler.h \
    libTargomanSMT/Private/SpecialTokenHandler/OOVHandler/intfOOVHandlerModule.hpp \
    libTargomanSMT/Private/SpecialTokenHandler/OOVHandler/OOVDefaultHandlers.h \
    libTargomanSMT/Private/RuleTable/clsMosesPlainRuleTable.h \
    libTargomanSMT/Private/FeatureFunctions/LanguageModel/LanguageModel.h \
    libTargomanSMT/Private/FeatureFunctions/ReorderingJump/ReorderingJump.h \
    libTargomanSMT/Types.h \
    libTargomanSMT/Private/PrivateTypes.h \
    libTargomanSMT/Private/FeatureFunctions/WordPenalty/WordPenalty.h \
    libTargomanSMT/Private/RuleTable/clsBinaryRuleTable.h \
    libTargomanSMT/Private/FeatureFunctions/UnknownWordPenalty/UnknownWordPenalty.h \
    libTargomanSMT/Private/Proxies/LanguageModel/clsKenLMProxy.h \
    libTargomanSMT/Translator.h \
    libTargomanSMT/Private/N-BestFinder/NBestSuggestions.h \
    libTargomanSMT/Private/SpecialTokenHandler/IXMLTagHandler/IXMLTagHandler.h \
    libTargomanSMT/Private/SpecialTokenHandler/IXMLTagHandler/clsDateHandler.h \
    libTargomanSMT/Private/SpecialTokenHandler/SpecialTokensRegistry.hpp \
    libTargomanSMT/Private/SpecialTokenHandler/IXMLTagHandler/intfIXMLTagHandlerModule.hpp \
    libTargomanSMT/Private/SpecialTokenHandler/IXMLTagHandler/clsNumberHandler.h \
    libTargomanSMT/Private/SpecialTokenHandler/IXMLTagHandler/clsSpecialNumberHandler.h \
    libTargomanSMT/Private/SpecialTokenHandler/IXMLTagHandler/clsEmailHandler.h \
    libTargomanSMT/Private/SpecialTokenHandler/IXMLTagHandler/clsURLHandler.h \
    libTargomanSMT/Private/SpecialTokenHandler/IXMLTagHandler/clsAbbreviationHandler.h \
    libTargomanSMT/Private/SpecialTokenHandler/IXMLTagHandler/clsOrderedListItemHandler.h \
    libTargomanSMT/Private/SpecialTokenHandler/IXMLTagHandler/clsTimeHandler.h \
    libTargomanSMT/Private/SpecialTokenHandler/IXMLTagHandler/clsOrdinalsHandler.h \
    libTargomanSMT/Private/SpecialTokenHandler/IXMLTagHandler/clsSymbolHandler.h \
    $$PWD/libTargomanSMT/Private/tmplSMTConfigurable.hpp \
    $$PWD/libTargomanSMT/Private/Proxies/intfTransliterator.h \
    $$PWD/libTargomanSMT/Private/Proxies/Translitration/clsTargomanTransliteratorProxy.h

# +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-#
SOURCES += libID.cpp \
    libTargomanSMT/Private/InputDecomposer/clsInput.cpp \
    libTargomanSMT/Private/GlobalConfigs.cpp \
    libTargomanSMT/Private/OutputComposer/clsOutputComposer.cpp \
    libTargomanSMT/Private/SearchGraphBuilder/clsLexicalHypothesis.cpp \
    libTargomanSMT/Private/SearchGraphBuilder/clsSearchGraphNode.cpp \
    libTargomanSMT/Private/SearchGraphBuilder/clsCardinality.cpp \
    libTargomanSMT/Private/SearchGraphBuilder/clsSearchGraph.cpp \
    libTargomanSMT/Private/FeatureFunctions/LexicalReordering/LexicalReordering.cpp \
    libTargomanSMT/Private/RuleTable/clsRuleNode.cpp \
    libTargomanSMT/Private/RuleTable/clsTargetRule.cpp \
    libTargomanSMT/Private/RuleTable/clsJanePlainRuleTable.cpp \
    libTargomanSMT/Private/FeatureFunctions/PhraseTable/PhraseTable.cpp \
    libTargomanSMT/Private/SpecialTokenHandler/OOVHandler/OOVHandler.cpp \
    libTargomanSMT/Private/SpecialTokenHandler/OOVHandler/OOVDefaultHandlers.cpp \
    libTargomanSMT/Private/RuleTable/clsMosesPlainRuleTable.cpp \
    libTargomanSMT/Private/FeatureFunctions/LanguageModel/LanguageModel.cpp \
    libTargomanSMT/Private/FeatureFunctions/ReorderingJump/ReorderingJump.cpp \
    libTargomanSMT/Private/FeatureFunctions/WordPenalty/WordPenalty.cpp \
    libTargomanSMT/Private/RuleTable/clsBinaryRuleTable.cpp \
    libTargomanSMT/Private/FeatureFunctions/UnknownWordPenalty/UnknownWordPenalty.cpp \
    libTargomanSMT/Private/Proxies/LanguageModel/clsTargomanLMProxy.cpp \
    libTargomanSMT/Private/Proxies/LanguageModel/clsKenLMProxy.cpp \
    libTargomanSMT/Translator.cpp \
    libTargomanSMT/Private/N-BestFinder/NBestSuggestions.cpp \
    libTargomanSMT/Private/SpecialTokenHandler/IXMLTagHandler/IXMLTagHandler.cpp \
    libTargomanSMT/Private/SpecialTokenHandler/IXMLTagHandler/clsDateHandler.cpp \
    libTargomanSMT/Private/SpecialTokenHandler/IXMLTagHandler/clsNumberHandler.cpp \
    libTargomanSMT/Private/SpecialTokenHandler/IXMLTagHandler/clsSpecialNumberHandler.cpp \
    libTargomanSMT/Private/SpecialTokenHandler/IXMLTagHandler/clsEmailHandler.cpp \
    libTargomanSMT/Private/SpecialTokenHandler/IXMLTagHandler/clsURLHandler.cpp \
    libTargomanSMT/Private/SpecialTokenHandler/IXMLTagHandler/clsAbbreviationHandler.cpp \
    libTargomanSMT/Private/SpecialTokenHandler/IXMLTagHandler/clsOrderedListItemHandler.cpp \
    libTargomanSMT/Private/SpecialTokenHandler/IXMLTagHandler/clsTimeHandler.cpp \
    libTargomanSMT/Private/SpecialTokenHandler/IXMLTagHandler/clsOrdinalsHandler.cpp \
    libTargomanSMT/Private/SpecialTokenHandler/IXMLTagHandler/clsSymbolHandler.cpp \
    $$PWD/libTargomanSMT/Private/Proxies/Translitration/clsTargomanTransliteratorProxy.cpp

# +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-#
OTHER_FILES +=

################################################################################
#                       DO NOT CHANGE ANYTHING BELOW                           #
################################################################################
ConfigFile = $$BasePath/Configs.pri
!exists($$ConfigFile){
error("**** libsrc: Unable to find Configuration file $$ConfigFile ****")
}

include ($$ConfigFile)

TEMPLATE = lib
TARGET = $$ProjectName
DESTDIR = $$BaseLibraryFolder
OBJECTS_DIR = $$BaseBuildFolder/obj
MOC_DIR = $$BaseBuildFolder/moc
QMAKE_CXXFLAGS_RELEASE += -fPIC
QMAKE_CXXFLAGS_DEBUG += -fPIC
INCLUDEPATH+=lib$$ProjectName

build_static {
    DEFINES += TARGOMAN_BUILD_STATIC
    CONFIG+= staticlib
}

QMAKE_POST_LINK += $$BaseOutput/linuxPostBuild.sh lib$$ProjectName $$BaseLibraryIncludeFolder $$BaseConfigFolder



