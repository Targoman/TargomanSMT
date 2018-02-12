#ifndef _PHRASE_INFO_H
#define _PHRASE_INFO_H

#include <map>
#include <set>
#include <vector>

using namespace std;

// my code: a class holdes all of information of a phrase
class PhraseInfo{
	
   public:
	int startE,endE,startF,endF,ID;
        vector <PhraseInfo*> nextPhrase,previosPhrase;
	PhraseInfo(int a, int b, int c, int d, int i ){		
		startE=a; endE=b; startF=c; endF=d; ID=i;
	}
	PhraseInfo(){		
		
	}
};

#endif
