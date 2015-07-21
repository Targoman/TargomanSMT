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
    UnitTest.h

# +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-#
SOURCES += \
    UnitTest.cpp \
    test_clsInputDecomposer_init.cpp \
    test_clsSearchGraphBuilder_conformsIBM1Constraint.cpp \
    test_clsSearchGraphBuilder_conformsHardReorderingJumpLimit.cpp \
    test_clsSearchGraphBuilder_collectPhraseCandidates.cpp \
    test_inputDecomposer_parseRichIXML.cpp \
    test_clsSearchGraphBuilder_initializeRestCostsMatrix.cpp \
    test_clsSearchGraphBuilder_calculateRestCost.cpp \
    test_ReorderingJump_getRestCostForPosition.cpp \
    test_clsLexicalHypothesisContainer_insertHypothesis.cpp \
    test_clsNBestFinder_fillBestOptions.cpp


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
