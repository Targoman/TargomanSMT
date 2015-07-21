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
  
