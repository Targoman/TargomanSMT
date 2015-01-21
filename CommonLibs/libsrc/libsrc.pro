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
    libTargomanCommon/PrefixTree/tmplAbstractFilePrefixTree.hpp \
    libTargomanCommon/PrefixTree/tmplAbstractPrefixTree.hpp \
    libTargomanCommon/PrefixTree/tmplFullVectorFilePrefixTree.hpp \
    libTargomanCommon/PrefixTree/tmplFullVectorPrefixTree.hpp \
    libTargomanCommon/PrefixTree/tmplMapFilePrefixTree.hpp \
    libTargomanCommon/PrefixTree/tmplMapPrefixTree.hpp \
    libTargomanCommon/PrefixTree/tmplVectorFilePrefixTree.hpp \
    libTargomanCommon/PrefixTree/tmplVectorPrefixTree.hpp \
    libTargomanCommon/HashFunctions.hpp \
    libTargomanCommon/tmplExpirableCache.hpp \
    libTargomanCommon/Private/clsConfiguration_p.h \
    libTargomanCommon/Configuration.h

# +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-#
SOURCES += \
    libTargomanCommon/exTargomanBase.cpp \
    libTargomanCommon/clsSafeCoreApplication.cpp \
    libTargomanCommon/CmdIO.cpp \
    libTargomanCommon/Logger.cpp \
    libID.cpp \
    libTargomanCommon/Constants.cpp \
    libTargomanCommon/clsCmdProgressBar.cpp \
    libTargomanCommon/Configuration.cpp

OTHER_FILES +=

################################################################################
#                       DO NOT CHANGE ANYTHING BELOW                           #
################################################################################
ConfigFileile = $$BasePath/Configs.pri
!exists($$ConfigFileile){
error("**** libsrc: Unable to find Configuration file $$ConfigFileile ****")
}
include ($$ConfigFileile)

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


