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
    libKenLM/Private/clsBinaryFormat.h \
    libKenLM/BinaryModelTypes.h \
    libKenLM/Configs.h \
    libKenLM/Private/clsKenLM_p.h \
    libKenLM/Private/clsArpaReader.h

# +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-#
SOURCES += libID.cpp \
    libKenLM/Private/clsBinaryFormat.cpp \
    libKenLM/Private/clsKenLM_p.cpp \
    libKenLM/Private/clsArpaReader.cpp

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

QMAKE_POST_LINK += mkdir -p $$BaseLibraryIncludeFolder/lib$$ProjectName;
QMAKE_POST_LINK += cp -vf lib$$ProjectName/*.h lib$$ProjectName/*.hpp $$BaseLibraryIncludeFolder/lib$$ProjectName 2>/dev/null|| : ;
QMAKE_POST_LINK += mkdir -p $$BaseConfigFolder/  ;
QMAKE_POST_LINK += cp -rvf conf/* $$BaseConfigFolder/ 2>/dev/null|| : ;


