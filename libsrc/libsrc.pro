################################################################################
# Copyright © 2012-2014, Targoman.com
#
# Published under the terms of TCRL(Targoman Community Research License)
# You can find a copy of the license file with distributed source or
# download it from http://targoman.com/License.txt
#
################################################################################
BasePath = ".."

# +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-#
HEADERS += \
    libTargomanCore/Private/Input/clsInput.h \
    libTargomanCore/Private/PhraseTable/intfPhraseTable.hpp \
    libTargomanCore/Private/Input/clsToken.h \
    libTargomanCore/GlobalConfigs.h \
    libTargomanCore/Private/PhraseTable/clsJanePTPlain.h \
    libTargomanCore/Private/PhraseTable/clsTargetRule.h \
    libTargomanCore/Private/PhraseTable/clsRuleNode.h

# +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-#
SOURCES += libID.cpp \
    libTargomanCore/Private/Input/clsInput.cpp \
    libTargomanCore/GlobalConfigs.cpp \
    libTargomanCore/Private/PhraseTable/clsJanePTPlain.cpp \
    libTargomanCore/Private/PhraseTable/clsTargetRule.cpp \
    libTargomanCore/Private/PhraseTable/clsRuleNode.cpp

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



