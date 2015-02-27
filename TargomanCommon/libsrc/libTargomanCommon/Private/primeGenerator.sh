#!/bin/sh

rm rands
Step=200000
for ((i=10000000; i<=500000000;i+=$Step)); do
    Random=$(( ( RANDOM % 10000 )  + i - 5000 ))
    echo $Random
    wget http://www.numberempire.com/$Random -O ./r 
    Pos=`grep -n "Next" ./r | cut -d ':' -f 1`
    Rand=`head -n $((Pos+1)) ./r | tail -n 2 |  cut -d '/' -f 2 | cut -d "'" -f 1`
    echo $Rand >> rands
done
rm r
  
