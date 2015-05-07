#!/bin/bash


QMAKE_COMMAND=qmake

Projects="TargomanCommon 
          NLPLibs/TargomanLM/ 
	  NLPLibs/TargomanTextProcessor/ 
	  ExternalToolsAndLibs/KenLM 
	  TargomanSMT 
	  Apps/TargomanSMTConsole 
	  Apps/TargomanSMTServer 
          Apps/loadBalancer"
BasePath=`pwd`
if [ "$1" == "full" ]; then
	rm -rf out
	for Proj in $Projects
	do
		cd  $BasePath/$Proj
		make distclean
		if [ "$2" != "release" ] ; then
 			$QMAKE_COMMAND CONFIG+=debug
		else
			$QMAKE_COMMAND
		fi
		make
		if [ $? -ne 0 ];then
			echo -e "\n\e[31m!!!!!!!!!!!!!!!!! $Proj Build Has failed!!!!!!!!!!!!!!!! \e[39m\n"
			break;
		else
			echo -e "\n\e[32m Module $Proj Compiled Successfully\e[39m\n"
		fi
	done
else
	for Proj in $Projects
	do
		cd  $BasePath/$Proj
		make
		if [ $? -ne 0 ];then
			echo -e "\n\e[31m!!!!!!!!!!!!!!!!! $Proj Build Has failed!!!!!!!!!!!!!!!! \e[39m\n"
			break;
		else
			echo -e "\n\e[32m Module $Proj Compiled Successfully\e[39m\n"
		fi
	done

fi

