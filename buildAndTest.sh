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
#!/bin/bash


QMAKE_COMMAND=qmake-qt5
if [ -z "$(which $QMAKE_COMMAND 2> /dev/null)" ]; then
  QMAKE_COMMAND=qmake
  if [ -z "$(which $QMAKE_COMMAND 2> /dev/null)" ]; then
    echo -e "\n\e[31m!!!!!!!!!!!!!!!!! QMake version 5 is needed for compiling the project! !!!!!!!!!!!!!!!! \e[39m\n"
    exit 1
  fi
fi

echo Using $QMAKE_COMMAND ...

Projects="TargomanCommon 
          NLPLibs/TargomanLM/ 
    NLPLibs/TargomanTextProcessor/
    Apps/E4SMT 
    ExternalToolsAndLibs/KenLM
    TargomanSMT 
    Apps/TargomanSMTConsole 
    Apps/TargomanSMTServer 
          Apps/TargomanLoadBalancer"
          
BasePath=`pwd`
export LD_LIBRARY_PATH="$BasePath/out/lib"
if [ "$2" != "release" ] ; then
  QMAKE_CONFIG="CONFIG+=debug"
else
  QMAKE_CONFIG=""
fi



if [ "$1" == "full" ]; then
  rm -rf out
  for Proj in $Projects
  do
    cd  $BasePath/$Proj
    if [ -f *.pro ]; then
      make distclean
      $QMAKE_COMMAND $QMAKE_CONFIG
    else
      make clean
    fi
    make -j 8
    if [ $? -ne 0 ];then
      echo -e "\n\e[31m!!!!!!!!!!!!!!!!! $Proj Build Has failed!!!!!!!!!!!!!!!! \e[39m\n"
      exit 1;
    else
      echo -e "\n\e[32m Module $Proj Compiled Successfully\e[39m\n"
      TestProgram=$(echo $BasePath/out/unitTest/unitTest_$(basename $Proj))
      #if [ -f  "$TestProgram" ]; then
      #  eval "$TestProgram"
      #  if [ $? -ne 0 ]; then
      #    echo -e "\n\e[31m!!!!!!!!!!!!!!!!Testing $Proj Failed!!!!!!!!!!!!!!!! \e[39m\n"
      #    exit 0
      #  else
      #    echo -e "\e[0;34m Testing $Proj Finished Successfully :) \e[39m\n"
      #  fi
      #fi
    fi
  done
else
  for Proj in $Projects
  do
    cd  $BasePath/$Proj
    $QMAKE_COMMAND $QMAKE_CONFIG
    make -j 8
    if [ $? -ne 0 ];then
      echo -e "\n\e[31m!!!!!!!!!!!!!!!!! $Proj Build Has failed!!!!!!!!!!!!!!!! \e[39m\n"
      exit 1;
    else
      echo -e "\n\e[32m Module $Proj Compiled Successfully\e[39m\n"
      TestProgram=$(echo $BasePath/out/unitTest/unitTest_$(basename $Proj))
      #if [ -f  "$TestProgram" ]; then
#	eval "$TestProgram"
#	if [ $? -ne 0 ]; then
#	  echo -e "\n\e[31m!!!!!!!!!!!!!!!!Testing $Proj Failed!!!!!!!!!!!!!!!! \e[39m\n"
#	  exit 0
#	else
#	  echo -e "\e[0;34m Testing $Proj Finished Successfully :) \e[39m\n"
#	fi
#      fi
    fi
  done

fi

