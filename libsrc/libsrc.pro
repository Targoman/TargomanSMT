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
    libTargomanCore/Private/SearchGraphBuilder/clsSearchGraphBuilder.h \
    libTargomanCore/Private/SearchGraphBuilder/clsHypothesisHolder.h \
    libTargomanCore/Private/clsTranslator_p.h \
    libTargomanCore/Private/GlobalConfigs.h \
    libTargomanCore/Private/N-BestFinder/clsNBestFinder.h \
    libTargomanCore/Private/OutputComposer/clsOutputComposer.h \
    libTargomanCore/Private/SearchGraphBuilder/clsLexicalHypothesis.h \
    libTargomanCore/Private/LanguageModel/intfLMSentenceScorer.hpp \
    libTargomanCore/Private/LanguageModel/clsTargomanLMProxy.h \
    libTargomanCore/Private/SearchGraphBuilder/clsSearchGraphNode.h \
    libTargomanCore/Private/SearchGraphBuilder/clsCardinality.h \
    libTargomanCore/Private/FeatureFunctions/intfFeatureFunction.hpp \
    libTargomanCore/Private/FeatureFunctions/LexicalReordering/LexicalReordering.h \
    libTargomanCore/Private/RuleTable/clsRuleNode.h \
    libTargomanCore/Private/RuleTable/clsTargetRule.h \
    libTargomanCore/Private/RuleTable/clsJanePlainRuleTable.h \
    libTargomanCore/Private/RuleTable/intfRuleTable.hpp \
    libTargomanCore/Private/FeatureFunctions/PhraseTable/PhraseTable.h

# +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-#
SOURCES += libID.cpp \
    libTargomanCore/Private/InputDecomposer/clsInput.cpp \
    libTargomanCore/clsTranslator.cpp \
    libTargomanCore/Private/SearchGraphBuilder/clsHypothesisHolder.cpp \
    libTargomanCore/Private/GlobalConfigs.cpp \
    libTargomanCore/Private/N-BestFinder/clsNBestFinder.cpp \
    libTargomanCore/Private/OutputComposer/clsOutputComposer.cpp \
    libTargomanCore/Private/SearchGraphBuilder/clsLexicalHypothesis.cpp \
    libTargomanCore/Private/LanguageModel/clsTargomanLMProxy.cpp \
    libTargomanCore/Private/SearchGraphBuilder/clsSearchGraphNode.cpp \
    libTargomanCore/Private/SearchGraphBuilder/clsCardinality.cpp \
    libTargomanCore/Private/SearchGraphBuilder/clsSearchGraphBuilder.cpp \
    libTargomanCore/Private/FeatureFunctions/LexicalReordering/LexicalReordering.cpp \
    libTargomanCore/Private/RuleTable/clsRuleNode.cpp \
    libTargomanCore/Private/RuleTable/clsTargetRule.cpp \
    libTargomanCore/Private/RuleTable/clsJanePlainRuleTable.cpp \
    libTargomanCore/Private/FeatureFunctions/PhraseTable/PhraseTable.cpp

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



