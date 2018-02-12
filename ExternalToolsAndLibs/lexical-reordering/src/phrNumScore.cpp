#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <cstring>
#include <math.h>
#include <numeric>
#include "/home/neda/MTtools/moses/moses/src/Util.h"


using namespace std;

ifstream input;
map<int, map<int, vector<string> > > phrNumCount;
map<int, map<int, float > > totData;
typedef map<int, map<int, float> > nonP;
nonP negNumerator;
nonP posNumerator;
typedef map<int, float> averageOrVar;
typedef map<int, float> nonParaProb;
nonParaProb negDenominator;
nonParaProb posDenominator;
averageOrVar negAverage;
averageOrVar posAverage;
averageOrVar negVarince;
averageOrVar posVarince;
nonP negProb;
nonP posProb;

void dataStore();
void calcAverageOrVar(map<int, float > &, map<int, float > &, string param, int maxPhrNum);
//void calcWeight(float & sumCount, int count );
void func(float &, float &, float , int);
void tempFunc(float &, float &, float, float, int);
void insertToNumerator(float, nonP &, int, int);
void calcProb(nonP& , nonP &, nonParaProb &);
void writeToFile(string pathName, int maxPhrNum);

int main(int argc, char* argv[]){
  
   if (argc < 4) {
    cerr << " the number of parameters is not enough \n";
    exit(1);
  }
  char* extractFileName = argv[1];
  //string parametric = argv[2];
  int maxPhrNum = atoi(argv[2]);
  string filepath = argv[3];
  
  input.open(extractFileName);
  dataStore();
  //parametric
  calcAverageOrVar(negAverage, posAverage, "average", maxPhrNum);
  calcAverageOrVar(negVarince, posVarince, "variance", maxPhrNum);
  
  //nonParametric
  calcAverageOrVar(negAverage, posAverage, "non-param", 100);
  calcProb(negProb, negNumerator, negDenominator);
  calcProb(posProb, posNumerator, posDenominator);
  
  
  writeToFile(filepath, maxPhrNum);
  
  return 0;
}


/*
 * ********************************************************************storeData()**************************************************************
 */
void dataStore(){
  string line = "";
  int lineNum = 1, num29 = 0;
  map<int, map<float, vector<int> > >::iterator itt;
  map<float, vector<int> >::iterator ittt;
  while( (getline (input, line))){
    vector<string> models = Moses::Tokenize(line , "|||");
      
      //models[0] = fr
      //models[1] = en
      //models[2] = betta(0) 
      //models[--] = Alpha(phr) -> delete
      //models[--] = Beta(phr) -> delete
      //models[3] = backward(orientaton:count:delta:alpha(pre))
      //models[4] = forward(orientaton:count:delta:beta(nxt))
      //models[5] = phrNum
      
      map<int, vector< float > > countDelta;
      map<int, vector<float> >::iterator it;
      
      vector<string> phrNum = Moses::Tokenize(models[5], " ");
      map<int, int> numberOfPhrNums;
      vector<int> phraseNum;
      float betta0 = atof(models[2].c_str());
      for(int k = 0; k < phrNum.size(); k++){
	vector<string> temp = Moses::Tokenize(phrNum.at(k), ":");
	numberOfPhrNums[atoi(temp.at(0).c_str())] = atoi(temp.at(1).c_str());
	phraseNum.push_back(atoi(temp.at(0).c_str()));
      }
      
      vector<string> count_delta = Moses::Tokenize(models[4], " ");
      vector<int> deltaNum;
      for(int k = 0; k < count_delta.size(); k++){
	vector<string> temp = Moses::Tokenize(count_delta.at(k), ":");
	
	if(countDelta.count(atoi(temp.at(2).c_str())) == 1 ){
	  it = countDelta.find(atoi(temp.at(2).c_str()));
	  ((*it).second).push_back(atof(temp.at(3).c_str()));
	}
	else{
	  deltaNum.push_back(atoi(temp.at(2).c_str()));
	  vector<float> vec;
	  vec.push_back(atof(temp.at(3).c_str()));
	  countDelta[atoi(temp.at(2).c_str())] = vec;
	}
     }
     
     stringstream os;
     os.str("");
     for(int k = 0; k < phraseNum.size(); k++){
       map<int, vector<string> > tempMap;
	for(int j = 0; j < deltaNum.size(); j++){
	  vector<string> tempVec;
	  int betta = accumulate(countDelta[deltaNum.at(j)].begin(), countDelta[deltaNum.at(j)].end(), 0);
	  (totData[phraseNum.at(k)])[deltaNum.at(j)] += (betta*numberOfPhrNums[phraseNum.at(k)])/betta0;
	}
     }
     lineNum++;
  }
  //cout << "tot: " << totData[1][23] << " ,countDelta: " << endl;
     
}

/*
 * ********************************************************************calcAverageDelta()**************************************************************
 */

void calcAverageOrVar(std::map< int, float >& neg, std::map< int, float >& pos, string param, int maxPhrNum)
{
  vector<int> phrNum;
  map<int, map<int, float > >:: iterator it;
  map<int, float > :: iterator it1;
  float totSumNegDeltaCount = 0;
  float totSumPosDeltaCount = 0;
  float totSumNegNumber = 0;
  float totSumPosNumber = 0;
  
  for(it = totData.begin(); it != totData.end(); ++it){
    float sumNegDeltaCount = 0;
    float sumPosDeltaCount = 0;
    float sumNegNumber = 0;
    float sumPosNumber = 0;
    float sumNegCount = 0;
    float sumPosCount = 0;
    
    int phrase = (*it).first;
    map<int, float > temp1 = (*it).second;
    
    for(it1 = temp1.begin(); it1 != temp1.end(); ++it1){
      float sumNegCountDelta = 0;
      float sumPosCountDelta = 0;
      int delta = abs((*it1).first);
      float count = (*it1).second;
      
      if(phrase < maxPhrNum){
	if((*it1).first < 0){
	  if(param == "average")
	    func(sumNegNumber, sumNegDeltaCount, count, delta);
	  if(param == "variance")
	    tempFunc(sumNegNumber, sumNegDeltaCount, negAverage[phrase], count, delta);
	  if(param == "non-param"){
	    sumNegCount += count; //Denominator
	    sumNegCountDelta = count; //Numerator
	    insertToNumerator(sumNegCountDelta, negNumerator, phrase, delta);
	  }    
	}else if((*it1).first > 0){
	  if(param == "average")
	    func(sumPosNumber, sumPosDeltaCount, count, delta);
	  if(param == "variance")
	    tempFunc(sumPosNumber, sumPosDeltaCount, posAverage[phrase], count, delta);
	  if(param == "non-param"){
	    sumPosCount += count; //Denominator
	    sumPosCountDelta = count; //Numerator
	    insertToNumerator(sumPosCountDelta, posNumerator, phrase, delta);
	  }
	}
      }
      //else{
	if((*it1).first < 0){
	  if(param == "average")
	    func(totSumNegNumber, totSumNegDeltaCount, count, delta);
	  if(param == "variance")
	    tempFunc(totSumNegNumber, totSumNegDeltaCount, negAverage[maxPhrNum], count, delta);
	  
	}else if((*it1).first > 0){
	  if(param == "average")
	    func(totSumPosNumber, totSumPosDeltaCount, count, delta);
	  if(param == "variance")
	    tempFunc(totSumPosNumber, totSumPosDeltaCount, posAverage[maxPhrNum], count, delta);
	}
    }
    
    if(sumNegNumber != 0)
      if(param == "average")
	neg[phrase] = sumNegDeltaCount/sumNegNumber;
      else
	neg[phrase] = sqrt(sumNegDeltaCount/sumNegNumber);
      
    if(sumPosNumber != 0)
      if(param == "average")
	pos[phrase] = sumPosDeltaCount/sumPosNumber;
      else
	pos[phrase] = sqrt(sumPosDeltaCount/sumPosNumber);
    if(param == "non-param"){
      //Denominator
      negDenominator[phrase] = sumNegCount;
      posDenominator[phrase] = sumPosCount;
    }
    
  }
  
  if(totSumNegNumber != 0)
    if(param == "average")
      neg[maxPhrNum] = totSumNegDeltaCount/totSumNegNumber;
    else
      neg[maxPhrNum] = sqrt(totSumNegDeltaCount/totSumNegNumber);
    
  if(totSumPosNumber != 0)
     if(param == "average")
       pos[maxPhrNum] = totSumPosDeltaCount/totSumPosNumber;
     else
       pos[maxPhrNum] = sqrt(totSumPosDeltaCount/totSumPosNumber);
  	  
}

/*
 * ********************************************************************func()**************************************************************
 */
void func(float & str1, float & str2, float alphaBetta, int delta){
  str1 += alphaBetta;
  str2 += delta * alphaBetta;
}

/*
 * ********************************************************************tempFunc()**************************************************************
 */
void tempFunc(float & str1, float & str2, float avrg, float alphaBetta, int delta ){
  str1 += alphaBetta;
  str2 += pow((delta-avrg), 2)*alphaBetta;
}


/*
 * ********************************************************************insertToNumerator()**************************************************************
 */
void insertToNumerator(float val , nonP & map, int phr, int delta)
{
  if(map.count(phr) == 1){
    (map[phr])[delta] = val;
  }
  else{
    nonParaProb m;
    m[delta] = val;
    map[phr] = m;
  }
}

/*
 * ********************************************************************calcProb()**************************************************************
 */

void calcProb(nonP & res , nonP & ano, nonParaProb &deno)
{
  map<int, map<int, float> >::iterator it1;
  map<int, float>::iterator it2;
  
  for(it1 = ano.begin(); it1 != ano.end(); it1++){
      int phr = (*it1).first;
      //cout << (*it1).second.size() << endl;
      for(it2 = ((*it1).second).begin(); it2 != ((*it1).second).end(); it2++){
	int delta = (*it2).first;
	float numerator = (*it2).second;
	float denominator = deno[phr];
	/*if(numerator == 0 && denominator != 0)
	  cout << phr << endl;*/
	(res[phr])[delta] = numerator/denominator;
      }
   }
}
/*
 * ********************************************************************writeToFile()**************************************************************
 */

void writeToFile(string pathName, int maxPhrNum)
{
  map<int, map<int, float > >::iterator it;
  nonParaProb::iterator itt;
  nonParaProb::iterator negIt;
  vector<int> keys ;
  string path = pathName + "phrNum";
//   string path2 = pathName + ".param";
  
  ofstream /*nonpara, para,*/ out;
  out.open(path.c_str());
//   para.open(path2.c_str());
  
  stringstream negArg, posArg, negVar, posVar;
  
  for(int i = 1; i <= maxPhrNum ; i++){
    if(negAverage.count(i) == 1)
      negArg << negAverage[i] << " ";
    else
      negArg << "0 ";
    if(posAverage.count(i) == 1)
      posArg << posAverage[i] << " ";
    else
      posArg << "0 ";
    if(negVarince.count(i) == 1)
      negVar << negVarince[i] << " ";
    else
      negVar << "0 ";
    if(posVarince.count(i) == 1)
      posVar << posVarince[i] << " ";
    else
      posVar << "0 ";
  }
  
  out << "para\n";
  out << "Pos Delta\n" << posArg.str() << "\n" << posVar.str() << "\n" ; 
  out << "Neg Delta\n" << negArg.str() << "\n" << negVar.str() << "\n" ;
  out << "non-para\n";
  
  
  for(it = totData.begin(); it != totData.end(); it++){
    keys.push_back((*it).first);
  }
  for(int i = 0; i < keys.size(); i++){
    if(keys.at(i) <= maxPhrNum){
	stringstream line;
	line << keys.at(i) << " ||| ";
	
	if(posProb.count(keys.at(i)) == 1){
	    for(itt = posProb[keys.at(i)].begin(); itt != posProb[keys.at(i)].end(); itt++){
	      line << (*itt).first << ":";
	      if(negProb.count(keys.at(i)) == 1){
		if(negProb[keys.at(i)].count((*itt).first) == 1){
		  negIt = negProb[keys.at(i)].find((*itt).first);
		  line << (*negIt).second << "*";
		  negProb[keys.at(i)][(*itt).first] = -1;
		}
		else {
		  line << 0 << "*"; 
		}
	      }
	      else {
		  line << 0 << "*"; 
	      }
	      line <<  (*itt).second << " , ";
	    }
	}
	
	if(negProb.count(keys.at(i)) == 1){ 
	    for(itt = negProb[keys.at(i)].begin(); itt != negProb[keys.at(i)].end(); itt++){
	      if((*itt).second != -1){
		line << (*itt).first << ":" << (*itt).second << "*0 , ";
	      }
	    }
	out << line.str() << endl;
	}
    }
  }
  
  
  out.close();
//   para.close();
}


// /home/neda/MTtools/training/marta/phrnum /home/neda/MTtools/prj/Reordering/reoGraph/lexPhrNum/model/extract.lexphr.sorted 10 /home/neda/MTtools/prj/Reordering/reoGraph/lexPhrNum/model/phraseNum
// /home/neda/MTtools/training/marta/phrnum /home/neda/MTtools/prj/test/diastance/model/extract.o 10 /home/neda/MTtools/prj/test/diastance/model/test