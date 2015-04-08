################################################################################
# Copyright © 2012-2015, Targoman.com
#
# Published under the terms of TCRL(Targoman Community Research License)
# You can find a copy of the license file with distributed source or
# download it from http://targoman.com/License.txt
#
################################################################################
BasePath = ".."

# +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-#
HEADERS += \
    libTargomanSMT/Private/InputDecomposer/clsInput.h \
    libTargomanSMT/Private/InputDecomposer/clsToken.h \
    libTargomanSMT/clsTranslator.h \
    libTargomanSMT/Private/SearchGraph/clsSearchGraphBuilder.h \
    libTargomanSMT/Private/clsTranslator_p.h \
    libTargomanSMT/Private/GlobalConfigs.h \
    libTargomanSMT/Private/N-BestFinder/clsNBestFinder.h \
    libTargomanSMT/Private/OutputComposer/clsOutputComposer.h \
    libTargomanSMT/Private/SearchGraph/clsLexicalHypothesis.h \
    libTargomanSMT/Private/Proxies/intfLMSentenceScorer.hpp \
    libTargomanSMT/Private/Proxies/LanguageModel/clsTargomanLMProxy.h \
    libTargomanSMT/Private/SearchGraph/clsSearchGraphNode.h \
    libTargomanSMT/Private/SearchGraph/clsCardinality.h \
    libTargomanSMT/Private/FeatureFunctions/intfFeatureFunction.hpp \
    libTargomanSMT/Private/FeatureFunctions/LexicalReordering/LexicalReordering.h \
    libTargomanSMT/Private/RuleTable/clsRuleNode.h \
    libTargomanSMT/Private/RuleTable/clsTargetRule.h \
    libTargomanSMT/Private/RuleTable/clsJanePlainRuleTable.h \
    libTargomanSMT/Private/RuleTable/intfRuleTable.hpp \
    libTargomanSMT/Private/FeatureFunctions/PhraseTable/PhraseTable.h \
    libTargomanSMT/Private/SearchGraph/clsHypothesisHolder.hpp \
    libTargomanSMT/Private/OOVHandler/OOVHandler.h \
    libTargomanSMT/Private/OOVHandler/intfOOVHandlerModule.hpp \
    libTargomanSMT/Private/OOVHandler/OOVDefaultHandlers.h \
    libTargomanSMT/Private/RuleTable/clsMosesPlainRuleTable.h \
    libTargomanSMT/Private/FeatureFunctions/LanguageModel/LanguageModel.h \
    libTargomanSMT/Private/FeatureFunctions/ReorderingJump/ReorderingJump.h \
    libTargomanSMT/Types.h \
    libTargomanSMT/Private/PrivateTypes.h \
    libTargomanSMT/Private/FeatureFunctions/WordPenalty/WordPenalty.h \
    libTargomanSMT/Private/RuleTable/clsBinaryRuleTable.h \
    libTargomanSMT/Private/FeatureFunctions/UnknownWordPenalty/UnknownWordPenalty.h \
    libTargomanSMT/Private/Proxies/LanguageModel/clsKenLMProxy.h

# +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-#
SOURCES += libID.cpp \
    libTargomanSMT/Private/InputDecomposer/clsInput.cpp \
    libTargomanSMT/clsTranslator.cpp \
    libTargomanSMT/Private/GlobalConfigs.cpp \
    libTargomanSMT/Private/N-BestFinder/clsNBestFinder.cpp \
    libTargomanSMT/Private/OutputComposer/clsOutputComposer.cpp \
    libTargomanSMT/Private/SearchGraph/clsLexicalHypothesis.cpp \
    libTargomanSMT/Private/Proxies/LanguageModel/clsTargomanLMProxy.cpp \
    libTargomanSMT/Private/SearchGraph/clsSearchGraphNode.cpp \
    libTargomanSMT/Private/SearchGraph/clsCardinality.cpp \
    libTargomanSMT/Private/SearchGraph/clsSearchGraphBuilder.cpp \
    libTargomanSMT/Private/FeatureFunctions/LexicalReordering/LexicalReordering.cpp \
    libTargomanSMT/Private/RuleTable/clsRuleNode.cpp \
    libTargomanSMT/Private/RuleTable/clsTargetRule.cpp \
    libTargomanSMT/Private/RuleTable/clsJanePlainRuleTable.cpp \
    libTargomanSMT/Private/FeatureFunctions/PhraseTable/PhraseTable.cpp \
    libTargomanSMT/Private/OOVHandler/OOVHandler.cpp \
    libTargomanSMT/Private/OOVHandler/OOVDefaultHandlers.cpp \
    libTargomanSMT/Private/RuleTable/clsMosesPlainRuleTable.cpp \
    libTargomanSMT/Private/FeatureFunctions/LanguageModel/LanguageModel.cpp \
    libTargomanSMT/Private/FeatureFunctions/ReorderingJump/ReorderingJump.cpp \
    libTargomanSMT/Private/FeatureFunctions/WordPenalty/WordPenalty.cpp \
    libTargomanSMT/Private/RuleTable/clsBinaryRuleTable.cpp \
    libTargomanSMT/Private/FeatureFunctions/UnknownWordPenalty/UnknownWordPenalty.cpp \
    libTargomanSMT/Private/Proxies/LanguageModel/clsKenLMProxy.cpp

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



