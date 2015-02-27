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
    libTargomanCore/Private/InputDecomposer/clsInput.h \
    libTargomanCore/Private/InputDecomposer/clsToken.h \
    libTargomanCore/clsTranslator.h \
    libTargomanCore/Private/SearchGraph/clsSearchGraphBuilder.h \
    libTargomanCore/Private/clsTranslator_p.h \
    libTargomanCore/Private/GlobalConfigs.h \
    libTargomanCore/Private/N-BestFinder/clsNBestFinder.h \
    libTargomanCore/Private/OutputComposer/clsOutputComposer.h \
    libTargomanCore/Private/SearchGraph/clsLexicalHypothesis.h \
    libTargomanCore/Private/Proxies/intfLMSentenceScorer.hpp \
    libTargomanCore/Private/Proxies/LanguageModel/clsTargomanLMProxy.h \
    libTargomanCore/Private/SearchGraph/clsSearchGraphNode.h \
    libTargomanCore/Private/SearchGraph/clsCardinality.h \
    libTargomanCore/Private/FeatureFunctions/intfFeatureFunction.hpp \
    libTargomanCore/Private/FeatureFunctions/LexicalReordering/LexicalReordering.h \
    libTargomanCore/Private/RuleTable/clsRuleNode.h \
    libTargomanCore/Private/RuleTable/clsTargetRule.h \
    libTargomanCore/Private/RuleTable/clsJanePlainRuleTable.h \
    libTargomanCore/Private/RuleTable/intfRuleTable.hpp \
    libTargomanCore/Private/FeatureFunctions/PhraseTable/PhraseTable.h \
    libTargomanCore/Private/SearchGraph/clsHypothesisHolder.hpp \
    libTargomanCore/Private/OOVHandler/OOVHandler.h \
    libTargomanCore/Private/OOVHandler/intfOOVHandlerModule.hpp \
    libTargomanCore/Private/OOVHandler/OOVDefaultHandlers.h \
    libTargomanCore/Private/RuleTable/clsMosesPlainRuleTable.h \
    libTargomanCore/Private/FeatureFunctions/LanguageModel/LanguageModel.h \
    libTargomanCore/Private/FeatureFunctions/ReorderingJump/ReorderingJump.h \
    libTargomanCore/Types.h \
    libTargomanCore/Private/PrivateTypes.h \
    libTargomanCore/Private/FeatureFunctions/WordPenalty/WordPenalty.h

# +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-#
SOURCES += libID.cpp \
    libTargomanCore/Private/InputDecomposer/clsInput.cpp \
    libTargomanCore/clsTranslator.cpp \
    libTargomanCore/Private/GlobalConfigs.cpp \
    libTargomanCore/Private/N-BestFinder/clsNBestFinder.cpp \
    libTargomanCore/Private/OutputComposer/clsOutputComposer.cpp \
    libTargomanCore/Private/SearchGraph/clsLexicalHypothesis.cpp \
    libTargomanCore/Private/Proxies/LanguageModel/clsTargomanLMProxy.cpp \
    libTargomanCore/Private/SearchGraph/clsSearchGraphNode.cpp \
    libTargomanCore/Private/SearchGraph/clsCardinality.cpp \
    libTargomanCore/Private/SearchGraph/clsSearchGraphBuilder.cpp \
    libTargomanCore/Private/FeatureFunctions/LexicalReordering/LexicalReordering.cpp \
    libTargomanCore/Private/RuleTable/clsRuleNode.cpp \
    libTargomanCore/Private/RuleTable/clsTargetRule.cpp \
    libTargomanCore/Private/RuleTable/clsJanePlainRuleTable.cpp \
    libTargomanCore/Private/FeatureFunctions/PhraseTable/PhraseTable.cpp \
    libTargomanCore/Private/OOVHandler/OOVHandler.cpp \
    libTargomanCore/Private/OOVHandler/OOVDefaultHandlers.cpp \
    libTargomanCore/Private/RuleTable/clsMosesPlainRuleTable.cpp \
    libTargomanCore/Private/FeatureFunctions/LanguageModel/LanguageModel.cpp \
    libTargomanCore/Private/FeatureFunctions/ReorderingJump/ReorderingJump.cpp \
    libTargomanCore/Private/FeatureFunctions/WordPenalty/WordPenalty.cpp

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



