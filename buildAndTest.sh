#!/bin/sh

Projects="CommonLibs NLPLibs/LanguageModel/ NLPLibs/TextProcessor/ SMTCore"
BasePath=`pwd`
if [ "$1" == "full" ]; then
	rm -rf out
	for Proj in $Projects
	do
		cd  $BasePath/$Proj
		make distclean
		qmake CONFIG+=debug
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


