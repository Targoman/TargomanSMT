#!/usr/bin/python
import os
import sys
import getopt
import re
import hashlib

Approved=0
Repeated=0
DiffFa=0
Discarded=0

if len(sys.argv) < 2 :
    print ("Invalid count of arguments. Must be:  %s <InFileEn> <InFileFa> " % str(sys.argv[0]))
    sys.exit(-1)

InEn   = str(sys.argv[1])
InFa   = str(sys.argv[2])


try:
    EnFile  = open( InEn,  "r" )
except:
    print "Unable to READ: ", _inEn
    sys.exit(-1);

try:
    FaFile = open( InFa, "r" )
except:
    print "Unable to READ: ", _inFa
    sys.exit(-1);

try:
    OutEn    = open( InEn + ".uniq",    "w" )
except:
    print "Unable to APPEND: ", _outEn
    sys.exit(-1);

try:
    OutFa    = open( InFa + ".uniq",    "w" )
except:
    print "Unable to APPEND: ", _outFa
    sys.exit(-1);

try:
    RepEn    = open( InEn + ".rep",    "w" )
except:
    print "Unable to APPEND: ", _outEn
    sys.exit(-1);

try:
    RepFa    = open( InFa + ".rep",    "w" )
except:
    print "Unable to APPEND: ", _outFa
    sys.exit(-1);

Lines={}

while 1 :
  EnLine = EnFile.readline()
  if EnLine == '':
    break;
  FaLine = FaFile.readline()

  EnWC=len(EnLine.split())
  FaWC=len(FaLine.split())

  if EnWC > 0 and FaWC > 0 :
    MD5En = hashlib.md5()
    MD5En.update(EnLine)
    MD5Fa = hashlib.md5()
    MD5Fa.update(FaLine)

    if MD5En.hexdigest() in Lines:
      if Lines[MD5En.hexdigest()]==MD5Fa.hexdigest():
        Repeated+=1
      else:
        DiffFa+=1
      RepEn.write(EnLine)
      RepFa.write(FaLine)
    else:
      Lines[MD5En.hexdigest()]=MD5Fa.hexdigest()
      OutEn.write(EnLine)
      OutFa.write(FaLine)
      Approved+=1
  else:
    Discarded+=1


print "====================================================================================="
print "Total: Approved: ", Approved, " Discarded", Discarded, " Repeated", Repeated, " DiffFa", DiffFa," Acceptance: ", (Approved / float (Approved + Repeated + Discarded + DiffFa)) * 100, "%"

