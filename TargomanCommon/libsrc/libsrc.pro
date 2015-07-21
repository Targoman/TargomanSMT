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
HEADERS += libTargomanCommon/exTargomanBase.h \
    libTargomanCommon/Macros.h \
    libTargomanCommon/clsSafeCoreApplication.h \
    libTargomanCommon/CmdIO.h \
    libTargomanCommon/Logger.h \
    libTargomanCommon/Private/Logger_p.h \
    libTargomanCommon/FastOperations.hpp \
    libTargomanCommon/Constants.h \
    libTargomanCommon/Types.h \
    libTargomanCommon/clsCmdProgressBar.h \
    libTargomanCommon/HashFunctions.hpp \
    libTargomanCommon/Configuration/intfConfigurable.hpp \
    libTargomanCommon/Configuration/clsFileBasedConfig.hpp \
    libTargomanCommon/Configuration/tmplConfigurable.h \
    libTargomanCommon/Configuration/ConfigManager.h \
    libTargomanCommon/Configuration/clsModuleConfig.hpp \
    libTargomanCommon/Configuration/intfConfigurableModule.hpp \
    libTargomanCommon/CompressedStream/clsCompressedInputStream.h \
    libTargomanCommon/CompressedStream/clsCompressedOutputStream.h \
    libTargomanCommon/CompressedStream/clsCompressedStreamBuff.h \
    libTargomanCommon/Configuration/Validators.hpp \
    libTargomanCommon/FStreamExtended.h \
    libTargomanCommon/PrefixTree/tmplPrefixTree.h \
    libTargomanCommon/PrefixTree/tmplOnMemoryPrefixTreeNode.hpp \
    libTargomanCommon/PrefixTree/tmplAbstractPrefixTreeNode.hpp \
    libTargomanCommon/PrefixTree/tmplOnDemandPrefixTreeNode.hpp \
    libTargomanCommon/JSONConversationProtocol.h \
    libTargomanCommon/Private/clsConfigManagerOverNet.h \
    libTargomanCommon/Private/clsConfigManager_p.h \
    libTargomanCommon/Configuration/tmplConfigurableArray.hpp \
    libTargomanCommon/SimpleAuthentication.h \
    libTargomanCommon/Private/RPCRegistry.hpp \
    libTargomanCommon/Configuration/intfRPCExporter.hpp \
    libTargomanCommon/Helpers.hpp \
    libTargomanCommon/PrefixTree/tmplAbstractOnDiskPrefixTreeNode.hpp \
    libTargomanCommon/PrefixTree/tmplFullCachePrefixTreeNode.hpp \
    libTargomanCommon/PrefixTree/tmplNoCachePrefixTreeNode.hpp \
    libTargomanCommon/tmplBoundedCache.hpp

# +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-#
SOURCES += \
    libTargomanCommon/exTargomanBase.cpp \
    libTargomanCommon/clsSafeCoreApplication.cpp \
    libTargomanCommon/CmdIO.cpp \
    libTargomanCommon/Logger.cpp \
    libID.cpp \
    libTargomanCommon/Constants.cpp \
    libTargomanCommon/clsCmdProgressBar.cpp \
    libTargomanCommon/Configuration/tmplConfigurable.cpp \
    libTargomanCommon/Configuration/ConfigManager.cpp \
    libTargomanCommon/CompressedStream/clsCompressedInputStream.cpp \
    libTargomanCommon/CompressedStream/clsCompressedOutputStream.cpp \
    libTargomanCommon/CompressedStream/clsCompressedStreamBuff.cpp \
    libTargomanCommon/FStreamExtended.cpp \
    libTargomanCommon/JSONConversationProtocol.cpp \
    libTargomanCommon/Configuration/clsConfigManagerOverNet.cpp \
    libTargomanCommon/SimpleAuthentication.cpp

OTHER_FILES += \
    libTargomanCommon/Private/primeGenerator.sh

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
OBJECTS_DIR = $$BaseBuildFolder/$$TARGET/obj
MOC_DIR = $$BaseBuildFolder/$$TARGET/moc
QMAKE_CXXFLAGS_RELEASE += -fPIC
QMAKE_CXXFLAGS_DEBUG += -fPIC
INCLUDEPATH+=lib$$ProjectName

build_static {
    DEFINES += TARGOMAN_BUILD_STATIC
    CONFIG+= staticlib
}

QMAKE_POST_LINK += $$BaseOutput/linuxPostBuild.sh lib$$ProjectName $$BaseLibraryIncludeFolder $$BaseConfigFolder



