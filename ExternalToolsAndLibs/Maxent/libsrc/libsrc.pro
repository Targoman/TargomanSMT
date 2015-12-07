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
    libMaxent/config.h \
    libMaxent/display.hpp \
    libMaxent/eventspace.hpp \
    libMaxent/ext_algorithm.hpp \
    libMaxent/finite.h \
    libMaxent/getopt.h \
    libMaxent/gistrainer.hpp \
    libMaxent/itemmap.hpp \
    libMaxent/lbfgs.h \
    libMaxent/lbfgstrainer.hpp \
    libMaxent/line_mem_iterator.hpp \
    libMaxent/line_stream_iterator.hpp \
    libMaxent/maxent_cmdline.h \
    libMaxent/maxentmodel.hpp \
    libMaxent/meevent.hpp \
    libMaxent/mmapfile.h \
    libMaxent/mmapfile.hpp \
    libMaxent/modelfile.hpp \
    libMaxent/rfevent.hpp \
    libMaxent/token_mem_iterator.hpp \
    libMaxent/trainer.hpp

# +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-#
SOURCES += \
    libMaxent/display.cpp \
    libMaxent/eventspace.tcc \
    libMaxent/f77_dummy_main.cpp \
    libMaxent/gistrainer.cpp \
    libMaxent/itemmap.tcc \
    libMaxent/lbfgstrainer.cpp \
    libMaxent/maxent.cpp \
    libMaxent/maxentmodel.cpp \
    libMaxent/modelfile.cpp \
    libMaxent/trainer.cpp \
    libMaxent/getopt.c \
    libMaxent/getopt1.c \
    libMaxent/lbfgs_wrapper.c \
    libMaxent/maxent_cmdline.c \
    libMaxent/mmapfile.c \
    libMaxent/sdrive.c

CONFIG += staticlib
#CONFIG += warn_off

#QMAKE_CXXFLAGS_RELEASE -= -O1
#QMAKE_CXXFLAGS_RELEASE -= -O2
#QMAKE_CXXFLAGS_RELEASE *= -O3
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

QMAKE_POST_LINK += $$BaseOutput/linuxPostBuild.sh lib$$ProjectName $$BaseLibraryIncludeFolder $$BaseConfigFolder  ;
QMAKE_POST_LINK += cp -vrf --parents `find ./lib$$ProjectName/ -name *.tcc ` $$BaseLibraryIncludeFolder || :

