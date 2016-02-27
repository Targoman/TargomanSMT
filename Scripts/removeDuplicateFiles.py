#!/usr/bin/python

import os
import sys
import getopt
import re
import hashlib

import hashlib

def fileChecksum(_filePath):
    Md5 = hashlib.md5()
    with open(_filePath,'rb') as F: 
        for Chunk in iter(lambda: F.read(8192), b''): 
            Md5.update(Chunk)
    return Md5.hexdigest()
  

if len(sys.argv) < 2 :
    print ("Invalid count of arguments. Must be:  %s <InDir>" % str(sys.argv[0]))
    sys.exit(-1)

InDir   = str(sys.argv[1])

if not os.path.isdir(InDir):
  print ("Invalid input directory Must be:  %s <InDir>" % str(sys.argv[0]))
  sys.exit(-1)

Repeated=0
Approved=0
ZeroLen=0

CheckedFiles={}
for Path, SubDirs, Files in os.walk(InDir):
  for Name in Files:
      InFile=os.path.join(Path, Name)
      statinfo = os.stat(InFile)
      if statinfo.st_size == 0:
        print "[ZeroLen] " + InFile
        os.remove(InFile)
        ZeroLen+=1
      else:
        FSum=fileChecksum(InFile)
        if FSum in CheckedFiles:
          print "[SameFile]" + InFile + " as " + CheckedFiles[FSum]
          Repeated+=1
          os.remove(InFile)
        else:
          CheckedFiles[FSum] = InFile
          Approved+=1
          print "[Accepted] " + InFile

print ""
print "====================================================================================="
print "Total: Approved: ", Approved, " ZeroLen: ", ZeroLen, " Repeated: ", Repeated, " Acceptance: ", (Approved / float (Approved + Repeated + ZeroLen)) * 100, "%"
print "====================================================================================="
print ""
