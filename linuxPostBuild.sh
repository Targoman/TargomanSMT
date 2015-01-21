#!/bin/sh

echo $0 $1 $2 $3

BasePath=$1
LibTarget=$2
ConfigTarget=$3

for Dir in `find $BasePath -type d -not -name Private`
do 
  mkdir -p $Dir
done 

cp -vrf --parents `find $BasePath -name *.h -o -name *.hpp` $LibTarget || :
rm -rvf $LibTarget/$BasePath/Private || :
mkdir -p $ConfigTarget  || :
cp -rvf conf/* ConfigTarget || :
