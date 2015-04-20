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
    libTargomanCommon/tmplExpirableCache.hpp \
    libTargomanCommon/Configuration/intfConfigurable.hpp \
    libTargomanCommon/Configuration/clsFileBasedConfig.hpp \
    libTargomanCommon/Configuration/tmplConfigurable.h \
    libTargomanCommon/Private/clsConfigManager_p.h \
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
    libTargomanCommon/JSONConversationProtocol.h

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
    libTargomanCommon/Configuration/clsConfigManager_p.cpp

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



