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
    libTargomanTextProcessor/Private/Normalizer.h \
    libTargomanTextProcessor/TextProcessor.h \
    libTargomanTextProcessor/Private/Unicode.hpp \
    libTargomanTextProcessor/Private/SpellCorrector.h \
    libTargomanTextProcessor/Private/SpellCheckers/clsPersianSpellCorrector.h \
    libTargomanTextProcessor/Private/IXMLWriter.h

# +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-#
SOURCES += libID.cpp \
    libTargomanTextProcessor/Private/Normalizer.cpp \
    libTargomanTextProcessor/TextProcessor.cpp \
    libTargomanTextProcessor/Private/SpellCorrector.cpp \
    libTargomanTextProcessor/Private/SpellCheckers/clsPersianSpellCorrector.cpp \
    libTargomanTextProcessor/Private/IXMLWriter.cpp

OTHER_FILES += \
    conf/PersianSpellCorrector.conf \
    conf/Abbreviations.tbl \
    conf/Normalization.conf

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
QMAKE_POST_LINK += cp -vf conf/* $$BaseConfigFolder/ 2>/dev/null|| : ;


