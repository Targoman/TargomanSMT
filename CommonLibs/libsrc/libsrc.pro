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
    libTargomanCommon/Private/Logger_p.h

# +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-#
SOURCES += \
    libTargomanCommon/exTargomanBase.cpp \
    libTargomanCommon/clsSafeCoreApplication.cpp \
    libTargomanCommon/CmdIO.cpp \
    libTargomanCommon/Logger.cpp \
    libID.cpp

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

build_static {
    DEFINES += TARGOMAN_BUILD_STATIC
    CONFIG+= staticlib
}

QMAKE_POST_LINK += mkdir -p $$BaseLibraryIncludeFolder/lib$$ProjectName;
QMAKE_POST_LINK += cp -vf lib$$ProjectName/*.h lib$$ProjectName/*.hpp $$BaseLibraryIncludeFolder/lib$$ProjectName 2>/dev/null|| : ;

