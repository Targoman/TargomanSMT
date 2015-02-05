################################################################################
# Copyright © 2012-2015, Targoman.com
#
# Published under the terms of TCRL(Targoman Community Research License)
# You can find a copy of the license file with distributed source or
# download it from http://targoman.com/License.txt
#
################################################################################
ProjectName="TargomanTextProcessor"
VERSION=0.0.1

# +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-#
ProjectDependencies+=TargomanCommon

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

