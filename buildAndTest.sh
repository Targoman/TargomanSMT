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
    fi
  done

fi

