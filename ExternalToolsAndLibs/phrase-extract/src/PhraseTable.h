#ifndef _PHRASE_TABLES_H
#define _PHRASE_TABLES_H

#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <string>

#include "SentenceAlignment.h"
#include "PhraseInfo.h"
#include "Graph.h"

using namespace std;

// PhraseCorner holds specific corner of all extracted phrases
// The key of the map is the specific corner and the value is a set of the phrases with same corner
typedef map <string, vector<PhraseInfo*> > phraseCorners;
typedef vector < PhraseInfo* > vec;

class OpOnPhrase {
 public:
  phraseCorners corners;
  SentenceAlignment sentence;
  vec setOfPhrases;
  Graph* graph;
  OpOnPhrase(SentenceAlignment&);
  ~OpOnPhrase();
  //void deleteObj();
  void add( PhraseInfo* phrase );
  void addCorners(string, PhraseInfo*);
  void clear();
  void reorderingGraph();
  vec getOrient(int e, int f, string flag, PhraseInfo& obj);
  bool isIncluded ( PhraseInfo, PhraseInfo );
  void connectedToGraph(SentenceAlignment &sentence, bool& overflow);
  //void calcCount(SentenceAlignment &sentence);
};

OpOnPhrase::OpOnPhrase(SentenceAlignment &s){
  sentence=s;
}

OpOnPhrase::~OpOnPhrase(){
  if(graph)
    delete graph;
  for(int k = 0; k < setOfPhrases.size(); k++){
    delete setOfPhrases.at(k);
  }
}

// void OpOnPhrase::deleteObj(){
//   if(graph)
//     delete graph;
//   for(int k = 0; k < setOfPhrases.size(); k++){
//     delete setOfPhrases.at(k);
//   }
// }

void OpOnPhrase::add( PhraseInfo* phrase ) {
  //cout << phrase.startF << "\n";
  setOfPhrases.push_back(phrase);
  int startE,endE,startF,endF;
  bool state1=false,state2=false,state3=false,state4=false;
  startE= phrase->startE;
  endE= phrase->endE;
  startF= phrase->startF;
  endF= phrase->endF;
  //cout << "startE'...endE': " << phrase.startE << "," << phrase.endE <<"  -startF'...endF': "<< phrase.startF << "," << phrase.endF  <<"\n";
  int a=endE-startE;
  int b=endF-startF;  
  if(a==0 && b==0) state1=true;
  if(a==0 && b>0) state2=true;
  if(a>0 && b==0) state3=true;
  if(a>0 && b>0) state4=true;
  //cout << "state1:" << state1 << ", state2:" << state2 << ", state3:" << state3 << ", state4:" << state4 << "\n";  
  string cornerKey;
  const char base[] = "";
  char num1 [ 10 ];
  char num2 [ 10 ];


  // (startE,startF) : LeftDown
  sprintf(num1, "%s%d", base, startE);
  sprintf(num2, "%s%d", base, startF);
  cornerKey= string(num1) + "," + string(num2);
  addCorners(cornerKey, phrase);
  
  if(state3 || state4){
	  // (endE,startF) : LeftTop
	  cornerKey="";
	  sprintf(num1, "%s%d", base, endE);
	  sprintf(num2, "%s%d", base, startF);
	  cornerKey= string(num1) + "," + string(num2);
	  addCorners(cornerKey, phrase);
  }

  if(state2 || state4){
	  // (startE,endF) : rightDown
	  cornerKey="";
	  sprintf(num1, "%s%d", base, startE);
	  sprintf(num2, "%s%d", base, endF);
	  cornerKey= string(num1) + "," + string(num2);
	  addCorners(cornerKey, phrase);
  }

  if(state4){
	  // (endE,endF) : rightTop
	  cornerKey="";
	  sprintf(num1, "%s%d", base, endE);
	  sprintf(num2, "%s%d", base, endF);
	  cornerKey= string(num1) + "," + string(num2);
	  addCorners(cornerKey, phrase);
	  //cout << "startE'...endE': " << phrase.startE << "," << phrase.endE <<"  -startF'...endF': "<< phrase.startF << "," << phrase.endF  <<"\n";
   }

}

void OpOnPhrase::addCorners(string s, PhraseInfo* ID){
  phraseCorners:: iterator it;   
  it=corners.find(s);
  if(it == corners.end()){
     //cout << s << "\n";
     vec v1;
     v1.push_back(ID);
     corners[s]=v1;
  }
  else{
    it->second.push_back(ID);
  }
}

void OpOnPhrase::clear() {
  setOfPhrases.clear();
}

void OpOnPhrase::reorderingGraph(){
  int startE,endE,startF,endF;
  vec previosL,previosR;
  vec nextL,nextR;
  

  //cout << "setOfPhrases.size(): " << setOfPhrases.size()<< "\n";
  for(int i=0;i<setOfPhrases.size();i++){
      PhraseInfo *phraseObj=(setOfPhrases.at(i));
      startE=phraseObj->startE;
      endE=phraseObj->endE;
      startF=phraseObj->startF;
      endF=phraseObj->endF;
      
//        if(phraseObj->ID == 1)
// 	 cout << "startE...endE: " << startE << "," << endE <<"  -startF...endF: " << startF << "," << endF << " ,phraseID: "<<phraseObj->ID<<"\n";
      
      // (startE,startF) : LeftDown
     
      previosL = getOrient(startE-1,startF-1, "LD", *phraseObj);
      // (endE,startF) : LeftTop
      nextL=getOrient(endE+1,startF-1, "LT", *phraseObj);
      // (startE,endF) : rightDown
      previosR=getOrient(startE-1,endF+1, "RD", *phraseObj);
      // (endE,endF) : rightTop
      nextR=getOrient(endE+1,endF+1, "RT", *phraseObj);

      //cout << "previosL: " << previosL.size() << ",previosR: " << previosR.size() << ",nextL: " << nextL.size() << ", nextR: " << nextR.size() << "\n"; 
      
    if(previosL.size()!=0) 
      phraseObj->previosPhrase.insert(phraseObj->previosPhrase.end(), previosL.begin(), previosL.end());
      //phraseObj->previosPhrase=previosL;
    else if(previosR.size()!=0) 
      phraseObj->previosPhrase.insert(phraseObj->previosPhrase.end(), previosR.begin(), previosR.end());
      //phraseObj->previosPhrase=previosR;
    if(nextL.size()!=0) 
      phraseObj->nextPhrase.insert(phraseObj->nextPhrase.end(), nextL.begin(), nextL.end()); 
      //(*phraseObj).nextPhrase=nextL;
    else if(nextR.size()!=0) 
      phraseObj->nextPhrase.insert(phraseObj->nextPhrase.end(), nextR.begin(), nextR.end()); 
      //phraseObj->nextPhrase=nextR;
   
	
	
	if(phraseObj->previosPhrase.size() == 0){
	  for(int indexE=phraseObj->startE-2; (indexE >= -1); indexE--){
	    previosL=getOrient(indexE, startF-1, "LD", *phraseObj);
	    if(previosL.size() == 0){
	      previosR=getOrient(indexE, endF+1, "RD", *phraseObj);
	      if(previosR.size() != 0){
		for(int j = 0; j < previosR.size(); j++){
		  phraseObj->previosPhrase.push_back(previosR.at(j));
		  previosR.at(j)->nextPhrase.push_back(phraseObj);
		}
		break;
	      }
	    }
	    else{
	      for(int j = 0; j < previosL.size(); j++){
		  phraseObj->previosPhrase.push_back(previosL.at(j));
		  previosL.at(j)->nextPhrase.push_back(phraseObj);
	      }
 	      break;
	    }
	  }
	 }
	
	if(phraseObj->nextPhrase.size() == 0){
	  //cout << "no nxt: " << phraseObj->ID << endl;
	  for(int indexE=phraseObj->endE+2; (indexE <= sentence.target.size() ); indexE++){
	    nextL=getOrient(indexE, startF-1, "LT", *phraseObj);
	    if(nextL.size() == 0){
	      nextR=getOrient(indexE, endF+1, "RT", *phraseObj);
	      if(nextR.size() != 0){
		for(int j = 0; j < nextR.size(); j++){
		  phraseObj->nextPhrase.push_back(nextR.at(j));
		  nextR.at(j)->previosPhrase.push_back(phraseObj);
		}
		break;
	      }
	    }
	    else 
	    {
	      for(int j = 0; j < nextL.size(); j++){
		  phraseObj->nextPhrase.push_back(nextL.at(j));
		  nextL.at(j)->previosPhrase.push_back(phraseObj);
	      }
	      break;
	    }
	  }
	} 
      }
//     connectedToGraph();
}

vec OpOnPhrase::getOrient(int e, int f, string flag, PhraseInfo& obj){
  phraseCorners::iterator it;
  map<int, PhraseInfo*> candidateVec;
  vec selectedVec;
  string cornerKey;
  bool connectedL=false,connectedR=false;
  const char base[] = "";
  char num1 [ 10 ];
  char num2 [ 10 ];
  sprintf(num1, "%s%d", base, e);
  sprintf(num2, "%s%d", base, f);
  cornerKey= string(num1) + "," + string(num2);
  int ID;
  it = corners.find("5,1");
  if(it != corners.end() ){
      //cout << it->second.size() <<"\n";
  }
  
  //discontinious orientation or monotone or swap
  //{
 if(flag=="LD" || flag=="LT"){
    for(int indexF=f; (indexF>= -1) /*&& !connectedL*/; indexF--){
    cornerKey="";
         sprintf(num1, "%s%d", base, e);
         sprintf(num2, "%s%d", base, indexF);
         cornerKey= string(num1) + "," + string(num2);
         it=corners.find(cornerKey);							 
         if(it != corners.end() ){
	// connectedL=true;
	  for(int i=0;i < it->second.size();i++){
	    ID = it->second.at(i)->ID;
	      candidateVec[ID] = (it->second.at(i));
	  }
         }
    }
  }
  
 if(flag=="RD" || flag=="RT"){
    for(int indexF=f; (indexF <= sentence.source.size())/* && !connectedR*/; indexF++){
	 cornerKey="";
         sprintf(num1, "%s%d", base, e);
         sprintf(num2, "%s%d", base, indexF);
         cornerKey= string(num1) + "," + string(num2);
         it=corners.find(cornerKey);
         if(it != corners.end() ){
	 //connectedR=true;
	  for(int i=0;i < it->second.size();i++){
	      ID = it->second.at(i)->ID;
	      candidateVec[ID] = (it->second.at(i));
	  }
         }
    }
  }
  //}
  
  //cheak candidateVec: have needed canditions?
  map<int, PhraseInfo*> ::iterator myit;
  for(myit = candidateVec.begin(); myit != candidateVec.end(); myit++){  
    PhraseInfo* p = myit->second;
    if(p->endF</*=f*/obj.startF && p->endE/*==e*/< obj.startE && flag=="LD"){  //previosL
      selectedVec.push_back(p);
    }
    else if(p->endF/*<=f*/< obj.startF && p->startE/*==e*/> obj.endE && flag=="LT"){  //nextL
      selectedVec.push_back(p);
    }
    else if(p->startF/*>=f*/ > obj.endF && p->endE/*==e*/ < obj.startE && flag=="RD"){   //previosR
      selectedVec.push_back(p);
    }
    else if(p->startF/*>=f*/ > obj.endF && p->startE/*==e*/ > obj.endE && flag=="RT"){   //nextR
      selectedVec.push_back(p);
    }
  } 
  //if(obj.ID == 9 && selectedVec.size()!=0 && flag == "RD") cout << "adada: " << selectedVec.at(0)->nextPhrase.size() <<  " ," << selectedVec.at(0)->startE << "," << selectedVec.at(0)->endE <<endl;
  return selectedVec;
}

void OpOnPhrase::connectedToGraph(SentenceAlignment& sentence, bool& overflow)
{
  /***/
  graph = new Graph(setOfPhrases.size());
  /***/
  for(int i=0;i<setOfPhrases.size();i++){
      PhraseInfo *phraseObj=(setOfPhrases.at(i));
      //if(phraseObj->ID == 0) cout << "3: " << setOfPhrases.at(0)->nextPhrase.size() << endl;
      int ID = phraseObj->ID;
      if(phraseObj->endE != sentence.target.size()-1){
	for(int i = 0; i < phraseObj->nextPhrase.size(); i++){
	  int ID2  = phraseObj->nextPhrase[i]->ID;
	  graph->add(ID, ID2, "n");
	}
      }
      for(int i = 0; i < phraseObj->previosPhrase.size(); i++){
	int ID2  = phraseObj->previosPhrase[i]->ID;
	graph->add(ID, ID2, "p");
      }
  }
  
  graph->fillNxtPathLen(overflow);
  if(overflow == false){
    graph->fillPrePathLen(overflow);
    graph->CalcCount();
  }
}
/*
void OpOnPhrase::calcCount(SentenceAlignment& sentence){
  float count;  
  for(int i=0;i<setOfPhrases.size();i++){
    PhraseInfo *phraseObj=(setOfPhrases.at(i));
    vec outBound = phraseObj->nextPhrase;
    vec inbound = phraseObj->previosPhrase;
    if(outBound.size() != 0&& phraseObj->endE != sentence.target.size()-1) 
      for(int i = 0 ;i < outBound.size(); i++){
	count = (graph->Beta(outBound.at(i)->ID)*graph->Alpha(phraseObj->ID))/(float)graph->Beta(0);
	//phraseObj->OutBoundCount[outBound.at(i)->ID] = count;
      }
    if(inbound.size() != 0&& phraseObj->startE != 0) 
      for(int i = 0 ;i < inbound.size(); i++){
	count = (graph->Alpha(inbound.at(i)->ID)*graph->Beta(phraseObj->ID))/(float)graph->Beta(0);
	//phraseObj->InBoundCount[inbound.at(i)->ID] = count;
      }
  }
  //file.close();
}*/

#endif
