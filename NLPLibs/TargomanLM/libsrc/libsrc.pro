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
    libTargomanLM/clsLanguageModel.h \
    libTargomanLM/Definitions.h \
    libTargomanLM/Private/clsLanguageModel_p.h \
    libTargomanLM/Private/ARPAManager.h \
    libTargomanLM/clsLMSentenceScorer.h \
    libTargomanLM/Private/clsLMSentenceScorer_p.h \
    libTargomanLM/Private/WeightStructs.hpp \
    libTargomanLM/Private/intfBaseModel.hpp \
    libTargomanLM/Private/clsAbstractProbingModel.h \
    libTargomanLM/Private/clsStringBasedProbingModel.h \
    libTargomanLM/Private/clsIndexBasedProbingModel.h

# +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-#
SOURCES += libID.cpp \
    libTargomanLM/clsLanguageModel.cpp \
    libTargomanLM/Private/ARPAManager.cpp \
    libTargomanLM/clsLMSentenceScorer.cpp \
    libTargomanLM/Private/clsAbstractProbingModel.cpp \
    libTargomanLM/Private/clsStringBasedProbingModel.cpp \
    libTargomanLM/Private/clsIndexBasedProbingModel.cpp

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



