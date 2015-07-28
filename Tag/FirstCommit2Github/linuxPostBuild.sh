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
#!/bin/sh

echo $0 $1 $2 $3

BasePath=$1
IncludeTarget=$2
ConfigTarget=$3

mkdir -pv $IncludeTarget

for Dir in `find $BasePath -type d -not -name Private`
do 
  mkdir -pv $Dir
done 

cp -vrf --parents `find $BasePath -name *.h -o -name *.hpp -o -name *.hh` $IncludeTarget || :
rm -rvf $IncludeTarget/$BasePath/Private || :
mkdir -p $ConfigTarget  || :
cp -rvf conf/* $ConfigTarget || :
