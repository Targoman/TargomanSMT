#!/usr/bin/python

import os
import sys
import getopt
import re
import hashlib

Discarded=0
Approved=0
Repeated=0
TotalLines=0
TotalEnWC=0
MaxEnWC=0
MinEnWC=1000
TotalFaWC=0
MaxFaWC=0
MinFaWC=1000

Lines={}

def extractPhrases(_inEn, _inFa, _outEn, _outFa, _repEn, _repFa):
  global Discarded
  global Approved
  global Repeated
  global TotalLines
  global TotalEnWC
  global MaxEnWC
  global MinEnWC
  global TotalFaWC
  global MaxFaWC
  global MinFaWC
  global Lines

  try:
      EnFile  = open( _inEn,  "r" )
  except:
      print "Unable to READ: ", _inEn
      sys.exit(-1);

  try:
      FaFile = open( _inFa, "r" )
  except:
      print "Unable to READ: ", _inFa
      sys.exit(-1);

  try:
      OutEn    = open( _outEn,    "a" )
  except:
      print "Unable to APPEND: ", _outEn
      sys.exit(-1);

  try:
      OutFa    = open( _outFa,    "a" )
  except:
      print "Unable to APPEND: ", _outFa
      sys.exit(-1);

  try:
      RepEn    = open( _repEn,    "a" )
  except:
      print "Unable to APPEND: ", _repEn
      sys.exit(-1);

  try:
      RepFa    = open( _repFa,    "a" )
  except:
      print "Unable to APPEND: ", _repFa
      sys.exit(-1);

  ReadFF=1
  ReadSF=1
  LastID=1
  MyRepeated=0
  MyApproved=0
  MyDiscarded=0
  MyTotalLines=0
  MyEnMax=0
  MyEnMin=1000
  MyEnSumWC=0;
  MyFaMax=0
  MyFaMin=1000
  MyFaSumWC=0;
  
  while 1 :
    EnLine = EnFile.readline()
    if EnLine == '':
      break;
    FaLine = FaFile.readline()
    MyTotalLines+=1
    
    EnWC=len(EnLine.split())
    FaWC=len(FaLine.split())
   
    if EnWC > 0 and EnWC < 95 and FaWC > 0 and FaWC < 95:
      MD5 = hashlib.md5()
      MD5.update(EnLine)

      if MD5.hexdigest() in Lines:
        RepEn.write(EnLine)
        RepFa.write(FaLine)
        MyRepeated+=1
      else:
        Lines[MD5.hexdigest()]=1
        OutEn.write(EnLine)
        OutFa.write(FaLine)
        MyApproved+=1
    else:
      MyDiscarded+=1
    
    if MyEnMax < EnWC:
      MyEnMax = EnWC
    if MyFaMax < FaWC:
      MyFaMax = FaWC

    if MyEnMin > EnWC and EnWC > 0:
      MyEnMin = EnWC
    if MyFaMin > FaWC and FaWC > 0:
      MyFaMin = FaWC
      
    MyEnSumWC += EnWC
    MyFaSumWC += FaWC
        
      
  print _inEn,"  Acceptance:", MyApproved," of ", MyApproved + MyDiscarded + MyRepeated
  Approved+=MyApproved
  Discarded+=MyDiscarded
  Repeated+=MyRepeated
  TotalEnWC += MyEnSumWC
  TotalFaWC += MyFaSumWC
  TotalLines+=MyTotalLines
  
  if MaxFaWC < MyFaMax:
    MaxFaWC = MyFaMax
  if MaxEnWC < MyEnMax:
    MaxEnWC = MyEnMax
  
  if MinFaWC > MyFaMin:
    MinFaWC = MyFaMin
  if MinEnWC > MyEnMin:
    MinEnWC = MyEnMin


if len(sys.argv) < 2 :
    print ("Invalid count of arguments. Must be:  %s <InDir> <OutName>" % str(sys.argv[0]))
    sys.exit(-1)

InDir   = str(sys.argv[1])
OutName = str(sys.argv[2])

if not os.path.isdir(InDir + "/en") or not os.path.isdir(InDir + "/fa") :
  print ("Invalid input directory must have en and fa folders. Must be:  %s <InDir> <OutName>" % str(sys.argv[0]))
  sys.exit(-1)

if os.path.exists(InDir + "/" + OutName + "_Short.fa"):
  os.remove(InDir + "/" + OutName + "_Short.fa")
  
if os.path.exists(InDir + "/" + OutName + "_Short.en"):
  os.remove(InDir + "/" + OutName + "_Short.en")

if os.path.exists(InDir + "/" + OutName + "_Repeated.fa"):
  os.remove(InDir + "/" + OutName + "_Repeated.fa")

if os.path.exists(InDir + "/" + OutName + "_Repeated.en"):
  os.remove(InDir + "/" + OutName + "_Repeated.en")

#  for Path, SubDirs, Files in os.walk(InputDir):
for Path, SubDirs, Files in os.walk(InDir + "/en/"):
  for Name in Files:
    if re.match(r'.*\.ixml$', Name) :
      extractPhrases(os.path.join(Path, Name), 
                     os.path.join(Path.replace(InDir + "/en/", InDir+"/fa/"), Name), 
                     InDir + "/" + OutName + "_Short.en", 
                     InDir + "/" + OutName + "_Short.fa",
                     InDir + "/" + OutName + "_Repeated.en", 
                     InDir + "/" + OutName + "_Repeated.fa")

print ""
print "Phrases below 90 words:"
print "====================================================================================="
print "Total: Approved: ", Approved, " Discarded", Discarded, " Repeated", Repeated, " Acceptance: ", (Approved / float (Approved + Repeated + Discarded)) * 100, "%"
print "Word per line: En Max: "+str(MaxEnWC)+", Avg: "+ str(TotalEnWC/float(TotalLines))+", Min: " + str(MinEnWC)
print "               Fa Max: "+str(MaxFaWC)+", Avg: "+ str(TotalFaWC/float(TotalLines))+", Min: " + str(MinFaWC)
