/*************************************************************************
 * Copyright © 2012-2015, Targoman.com
 *
 * Published under the terms of TCRL(Targoman Community Research License)
 * You can find a copy of the license file with distributed source or
 * download it from http://targoman.com/License.txt
 *
 *************************************************************************/
/**
 @author S. Mohammad M. Ziabary <smm@ziabary.com>
 */

#include "libTargomanCore/Private/InputDecomposer/clsInput.h"
#include "libTargomanCore/Private/GlobalConfigs.h"
#include "libTargomanCommon/Logger.h"
#include <iostream>

#include <QFile>

//TODO remove
#include <fstream>
#include <string>
#include <QTime>

using namespace std;

class clsData : public QSharedData{
public:
    clsData(int i): A(i){}
    ~clsData() {
       cout<<(void*)this<<" "<<A<<" Deleted !!!!!"<<endl;
    }
    int A;
};

class clsClass
{
public:
    clsClass(int i):Data(new clsData(i)) {}
    void setA(int i){Data->A = i;}

    QSharedDataPointer<clsData> Data;
};

int main(int argc, char *argv[])
{
    QList<clsClass> A;

    A.append(clsClass(5));
    if (true){
        clsClass B = A.first();
        B.setA(111);
    }
    A.append(clsClass(7));
    A.takeFirst();

    cout<<"Finished"<<endl;
}


