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
    sampleAgent.hpp \
    test.h

# +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-#
SOURCES += \
    main.cpp \
    test.cpp

################################################################################
#                       DO NOT CHANGE ANYTHING BELOW                           #
################################################################################
ConfigFile = $$BasePath/Configs.pri
!exists($$ConfigFile){
error("**** libsrc: Unable to find Configuration file $$ConfigFile ****")
}
include ($$ConfigFile)

TEMPLATE = app
TARGET = test_$$ProjectName
DESTDIR = $$BaseTestBinFolder
OBJECTS_DIR = $$BaseBuildFolder/$$TARGET/obj
MOC_DIR = $$BaseBuildFolder/$$TARGET/moc
INCLUDEPATH += $$BasePath/libsrc \
               $$BasePath/libsrc/lib$$ProjectName
QMAKE_LIBDIR += $$BaseLibraryFolder
LIBS += -l$$ProjectName
