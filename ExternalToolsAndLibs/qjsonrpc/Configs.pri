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
ProjectName="QJsonRPC"
VERSION=1.1.0

# +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-#
ProjectDependencies+=

QT+=network

################################################################################
#                       DO NOT CHANGE ANYTHING BELOW                           #
# more info: http://www.qtcentre.org/wiki/index.php?title=Undocumented_qmake   #
################################################################################
LookUps=. .. ../.. ../../.. ../../../.. ../../../../.. ../../../../../.. \
        ../../../../../../.. ../../../../../../../.. ../../../../../../../../..

for(CurrPath, LookUps) {
    exists($$CurrPath/Project.pri) {
      ProjectConfig = $$CurrPath/Project.pri
      BaseOutput = $$CurrPath
      break()
  }
}
DependencySearchPaths +=$$BaseOutput/out/lib
INCLUDEPATH+=$$BaseOutput/out/include

!exists($$ProjectConfig){
error("***** $$ProjectName: Unable to find Configuration file $$ProjectConfig ***** ")
}

include ($$ProjectConfig)

# +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-#

for(Project, ProjectDependencies) {
  for(Path, FullDependencySearchPaths):isEmpty( Found ) {
      message(Looking for $$Project in $$Path/)
      exists($$Path/lib$$Project*) {
        Found = "TRUE"
        message(-------------> $$Project Found!!!)
      }
      message("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-")
  }
  isEmpty( Found ) {
    message("***********************************************************************************************")
    message("!!!!!! $$ProjectName Depends on $$Project but not found ")
    message("***********************************************************************************************")
    error("")
  }
  Found = ""
}


for(Library, ProjectDependencies):LIBS += -l$$Library
# +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-#

INCLUDEPATH+=$$BaseLibraryIncludeFolder



