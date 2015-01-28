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

cp -vrf --parents `find $BasePath -name *.h -o -name *.hpp` $IncludeTarget || :
rm -rvf $IncludeTarget/$BasePath/Private || :
mkdir -p $ConfigTarget  || :
cp -rvf conf/* ConfigTarget || :
