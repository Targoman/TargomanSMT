#include<vector>
#include<map>
#define MAXSZ 10000
#include <string>
#include <string.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
#include <stdlib.h>
#include <assert.h>
#include <cstring>

#include <map>
#include <set>
#include <vector>


using namespace std;
class Graph{
public:
	int n; //number of nodes
	int phrNum;
	vector<int> NextGr[MAXSZ];
	map<int, long> NextPathLen[MAXSZ];
	vector<int> PreGr[MAXSZ];
	map<int, long> PrePathLen[MAXSZ];
	map <string, float> countOfEdge;
	vector<long> alpha;
	vector<long> betta;
	Graph(int);
	~Graph();
	void fillNxtPathLen(bool& overflow);
	void fillPrePathLen(bool& overflow);
	void add(int ID1, int ID2, string str);
	map<int, long>& getPathFirstToEnd(int ID);
	map<int, long>& getPathEndToFirst(int ID);
	int getMaxPhrNum();
	long getAlpha(int ID);
	long getBeta(int ID);
	void test(int id);
	void CalcCount();
	//void CalcAlpha();
	//void CalcBetta();
	float getCount(string key);
	bool cheakOverflow(long preSum, long newSum);
	void initialize(vector<long>& vec);
	ofstream out;
	ifstream in;

};

Graph::Graph(int n_ = 0){
	n = n_;
}

Graph::~Graph(){
  for(int i = 0; i < MAXSZ; i++){
    NextGr[i].clear();
    NextPathLen[i].clear();
    PreGr[i].clear();
    PrePathLen[i].clear();
  }
}

void Graph::add(int ID1, int ID2, string str){
	if(strcmp(str.c_str(), "n") == 0){
	  NextGr[ID1].push_back(ID2);
	}
	else{
	  PreGr[ID1].push_back(ID2);
	}
}

void Graph::test(int id){
  if(id == 0) cout << "test:" << NextGr[0].size() << endl;
}

void Graph::fillPrePathLen(bool& overflow){
  //ofstream test("ofstr");
  initialize(betta);
  overflow = false;
      (PrePathLen[n-1])[0] = 1;
      map<int, long>::iterator it;
	for(int i = n-1; i > 0; i--){
		map<int, long> temp = PrePathLen[i];
		for(it = temp.begin(); it != temp.end(); it++){
		  for(int j = 0; j < PreGr[i].size(); j++){
		    int key = ((*it).first) + 1;
		    long overf = 0;
		    overf = betta.at(PreGr[i][j]);
		    ( PrePathLen[PreGr[i][j]])[key] += (*it).second;
		    betta.at(PreGr[i][j]) += (*it).second;
		    overflow = cheakOverflow(overf, betta.at(PreGr[i][j]));
		    if(overflow == true)
		      break;
		    if (( PrePathLen[PreGr[i][j]])[key] < 0){
		      cout << "out1: "<< endl;
		      exit(-1);
		    }
		  }
		  if(overflow == true)
		      break;
		}
		if(overflow == true)
		      break;
	}
	//CalcBetta();//test.close();;
}

void Graph::fillNxtPathLen(bool& overflow){
  initialize(alpha);
  overflow = false;
        (NextPathLen[0])[0] = 1;
	map<int, long>::iterator it;
	for(int i = 0; i < n; i++){
	  map<int, long> temp = NextPathLen[i];
		for(it = temp.begin(); it != temp.end(); it++){
		  for(int j = 0; j < NextGr[i].size(); j++){
		    int key = ((*it).first) + 1;
		    long overf = 0;
		    overf = alpha.at(NextGr[i][j]);
		    ( NextPathLen[NextGr[i][j]])[key] += (*it).second;
		    alpha.at(NextGr[i][j]) += (*it).second;
		    overflow = cheakOverflow(overf, alpha.at(NextGr[i][j]));
		    if(overflow == true)
		      break;
		    if (( NextPathLen[NextGr[i][j]])[key] < 0){
		      cout << "out2: "<< endl;
		      exit(-1);
		    }
		  }
		  if(overflow == true)
		      break;
		}
		if(overflow == true)
		      break;
	}
	//CalcAlpha();
}


void Graph::CalcCount(){
  float count = 0;
  stringstream os;
  bool overflow = false;
  for(int id = 0; id <= n-1; id++){
     for(int j = 0; j < NextGr[id].size(); j++){
       os.str("");
       if(NextGr[id][j] != n){
	  count = (getAlpha(id)*getBeta(NextGr[id][j]))/(float)getBeta(0);
	  os << id << "-" << NextGr[id][j];
	  countOfEdge[os.str()] = count;
	  if(count > 1){
	    overflow = true;
	    break;
	  }
	  /*if(count == 1){
	    cout << "edge: " << os.str() << " ,getAlpha(id): " << getAlpha(id) << " ,betta: " << getBeta(NextGr[id][j]) << " ,betta0: " << getBeta(0) << endl;
	    //exit(-1);
	  }*/
	  /*if(getAlpha(id) < 0){
	    cout << "getAlpha(id): " << getAlpha(id) << " ,id:" << id << endl;
	    exit(-1);
	  }
	  if(getBeta(NextGr[id][j]) < 0){
	    cout << "getBeta(NextGr[id][j]): " << getBeta(NextGr[id][j]) << " ,NextGr[id][j]: " << NextGr[id][j] << endl;
	    exit(-1);
	  }
	  if(getBeta(0) < 0){
	    cout << "getBeta(0): " << getBeta(0) << endl;
	    exit(-1);
	  }*/
       }
       if(overflow == true){
	 break;
      }
    }
  }
	  
}

/*void Graph::CalcAlpha(){
  
  map<int, long>::iterator it;
  for(int k = 0; k < MAXSZ; k++){
    alpha.push_back(0);
    for(it = NextPathLen[k].begin(); it != NextPathLen[k].end(); it++){
      alpha[k] += (*it).second;
    }
  }
}*/

/*void Graph::CalcBetta(){
  map<int, long>::iterator it;
  int a = 0;
  for(int k = 0; k < MAXSZ; k++){
    betta.push_back(0);
    for(it = PrePathLen[k].begin(); it != PrePathLen[k].end(); it++){
      betta[k] += (*it).second;
      if((*it).second < 1){
	cout << "second:" << (*it).second << endl;
	exit(-1);;
      }
    }
  }
}*/

void Graph::initialize(std::vector< long >& vec)
{
  for(int k =0; k < n; k++){
    vec.push_back(0);
  }
  vec.at(0) +=1;
}


long Graph::getAlpha(int ID)
{
  return alpha[ID];
}

long Graph::getBeta(int ID)
{
  return betta[ID];
}

int Graph::getMaxPhrNum()
{
  return phrNum;
}

map<int, long>& Graph::getPathFirstToEnd(int ID){
  return NextPathLen[ID];
}

map<int, long>& Graph::getPathEndToFirst(int ID)
{
  return PrePathLen[ID];
}

bool Graph::cheakOverflow(long int preSum, long int newSum)
{
  bool flag = false;
  if(preSum > newSum){
    flag = true;
  }
  return flag;
}


float Graph::getCount(string key){
  if(countOfEdge.count(key))
    return countOfEdge[key];
  else
    return -1;
}