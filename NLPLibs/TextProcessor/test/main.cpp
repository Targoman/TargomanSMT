/*************************************************************************
 * Copyright © 2012-2014, Targoman.com
 *
 * Published under the terms of TCRL(Targoman Community Research License)
 * You can find a copy of the license file with distributed source or
 * download it from http://targoman.com/License.txt
 *
 *************************************************************************/
/**
 @author S. Mohammad M. Ziabary <smm@ziabary.com>
 */

//#include "libLIB_NAME/???.h"

#include "libTargomanTextProcessor/Private/Normalizer.h"
#include <QChar>
#include <QString>
#include <iostream>



int main(int argc, char *argv[])
{
    QString A = QString::fromUtf8("بیمعرفت");
    QString B = QString::fromUtf8("بی‌معرفت");
    A = A.mid(2);
    B = B.mid(2);
    std::cout<<A.toUtf8().constData()<<std::endl;
    std::cout<<B.toUtf8().constData()<<std::endl;
    std::cout << (A == B) << std::endl;
    std::cout << (A.trimmed() == B.trimmed()) << std::endl;
    std::cout << (A.remove(ARABIC_ZWNJ) == B.remove(ARABIC_ZWNJ)) << std::endl;
}


