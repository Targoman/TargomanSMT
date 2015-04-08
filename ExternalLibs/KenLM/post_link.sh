#!/bin/bash
mkdir -p ../../out/lib ../../out/include/libKenLM
cp libKenLM.a ../../out/lib/
cp lm/*.h* ../../out/include/libKenLM/
cp util/*.h* ../../out/include/libKenLM/
for HEADER in ../../out/include/libKenLM/*.h*; do
  perl -i -pe 's%(#include.*?)lm/%\1libKenLM/%g;s%(#include.*?)util/%\1libKenLM/%g' $HEADER
done