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



