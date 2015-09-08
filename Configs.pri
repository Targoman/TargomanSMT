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
VERSION = 0.0.0

Dependencies +=

################################################################################
#                     DO NOT CHANGE ANYTHING BELOW
################################################################################

!unix{
  error("********* Compile on OS other than Linux is not ready yet")
}

LookUps=. .. ../.. ../../.. ../../../.. ../../../../.. ../../../../../..
for(CurrPath, LookUps) {
  isEmpty($$NoChange){
    exists($$CurrPath/Configs.pri) {
      BaseOutput = $$CurrPath
    }
    else{
      NoChange=True
    }
  }
}

BasePath=$$BasePath/$$BaseOutput
message("*******************   $$ProjectName CONFIG  ************************ ")
message("* Building $$ProjectName Ver. $$VERSION")
message("* Base Path has been set to: $$BasePath/")
message("* Install Path has been set to: $$PREFIX/")
message("******************************************************************** ")

#############################################################################
LibFolderPattern = ./lib/lib
LibIncludeFolderPattern = ./lib/include
BinFolderPattern = ./bin
BaseLibraryFolder = $$BasePath/out/$$LibFolderPattern
BaseLibraryIncludeFolder = $$BasePath/out/$$LibIncludeFolderPattern
BaseBinFolder = $$BasePath/out/$$BinFolderPattern

#############################################################################
CONFIG(debug, debug|release): DEFINES += TARGOMAN_SHOW_DEBUG=1
DEFINES += TARGOMAN_DEBUG_PROCESS_LINE=1
DEFINES += TARGOMAN_DEBUG_PROCESS_LINE_SHOW_FILE
DEFINES += TARGOMAN_DEFAULT_DEBUG_LEVEL=7
DEFINES += TARGOMAN_LOG_SYNC=1
DEFINES += TARGOMAN_SHOW_LOG=1
DEFINES += TARGOMAN_DEFAULT_ERROR_LEVEL=10
DEFINES += TARGOMAN_ERROR_SHOW_FILE=1
DEFINES += TARGOMAN_DEFAULT_WARNING_LEVEL=10
DEFINES += TARGOMAN_WARNING_SHOW_FILE=1
DEFINES += TARGOMAN_DEFAULT_INFO_LEVEL=10
DEFINES += TARGOMAN_INFO_SHOW_FILE=1
DEFINES += PROJ_VERSION=$$VERSION

#############################################################################
QT += core

#############################################################################
INCLUDEPATH += $$BaseLibraryIncludeFolder \
               $$BasePluginInterfaceFolder \
               $$(PREFIX)/include \
               $$(EXTERNAL_DEPS)/

#############################################################################
DependencySearchPaths += $$BaseOutput/out/$$LibFolderPattern \
                         $$BaseLibraryFolder
FullDependencySearchPaths=   $$DependencySearchPaths

unix {
  DependencySearchPaths += $$(PREFIX)/lib \
                           $$(PREFIX)/lib/lib
  FullDependencySearchPaths=   /usr/lib \
                               /usr/lib64 \
                               /usr/local/lib \
                               /usr/local/lib64
}

QMAKE_LIBDIR +=  $$DependencySearchPaths

for(project, Dependencies) {
  for(path, FullDependencySearchPaths):isEmpty( Found ) {
      message(Searching for $$project in $$path)
      exists($$path/lib$$project*) {
        Found = "TRUE"
        message(-------------> $$project Found!!!)
      }
      message("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-")
  }
  isEmpty( Found ) {
    message("*****************************************************************")
    message("!!!!!! $$ProjectName Depends on $$project but not found ")
    message("*****************************************************************")
    error("")
  }
  Found = ""
}


for(Library, Dependencies):LIBS += -l$$Library
#############################################################################

isEmpty(PREFIX) {
 PREFIX = $$(HOME)/local
}

documentation.path = $$(PREFIX)/doc
documentation.files=docs/*
#  documentation.extra=create_docs; mv master.doc toc.doc

target.files= out/$$BinFolderPattern \
  out/$$LibFolderPattern \
  out/$$LibIncludeFolderPattern \
  out/$$PluginFolderPattern \
  out/$$PluginInterfaceFolderPattern

target.path = $$(PREFIX)/
target.extra= rm -f $$(PREFIX)/lib/lib

INSTALLS += documentation \
            target

system("ls $$BaseOutput")

unix {
  DependencySearchPaths += $$(PREFIX)/lib \
                           $$(PREFIX)/lib/lib
  FullDependencySearchPaths=   /usr/lib \
                               /usr/lib64 \
                               /usr/local/lib \
                               /usr/local/lib64

  PostQMakeScript = $$BasePath/postqmake.unix.sh

  system("mkdir -p $$BaseOutput/out/$$LibFolderPattern")
  system("mkdir -p $$BaseOutput/out/$$LibIncludeFolderPattern")
  system("mkdir -p $$BaseOutput/out/$$BinFolderPattern")
  system("mkdir -p $$BaseOutput/out/$$PluginFolderPattern")
  system("mkdir -p $$BaseOutput/out/$$PluginInterfaceFolderPattern")
}

