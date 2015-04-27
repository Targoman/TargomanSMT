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
    UnitTest.h

# +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-#
SOURCES += \
    UnitTest.cpp \
    test_clsInputDecomposer_init.cpp \
    test_clsSearchGraphBuilder_conformsIBM1Constraint.cpp \
    test_clsSearchGraphBuilder_conformsHardReorderingJumpLimit.cpp \
    test_clsSearchGraphBuilder_collectPhraseCandidates.cpp \
    test_inputDecomposer_parseRichIXML.cpp \
    test_clsSearchGraphBuilder_initializeRestCostsMatrix.cpp


################################################################################
#                       DO NOT CHANGE ANYTHING BELOW                           #
################################################################################
ConfigFile = $$BasePath/Configs.pri
!exists($$ConfigFile){
error("**** libsrc: Unable to find Configuration file $$ConfigFile ****")
}
include ($$ConfigFile)

TEMPLATE = app
QT+=testlib
TARGET = unitTest_$$ProjectName
DESTDIR = $$BaseUnitTestBinFolder
OBJECTS_DIR = $$BaseBuildFolder/obj
MOC_DIR = $$BaseBuildFolder/moc
INCLUDEPATH += $$BasePath/libsrc
QMAKE_LIBDIR += $$BaseLibraryFolder
LIBS += -l$$ProjectName

QMAKE_POST_LINK += ln -s $$PWD/assets/ $$BaseUnitTestBinFolder || :
