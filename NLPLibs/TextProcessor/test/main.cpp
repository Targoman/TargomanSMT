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
#include "libTargomanTextProcessor/Private/Unicode.hpp"
using namespace Targoman::NLPLibs::Private;
#include <QChar>
#include <QDebug>
#include <QString>
#include <iostream>



int main(int _argc, char *_argv[])
{
    Q_UNUSED(_argc)
    Q_UNUSED(_argv)

    try{
        Targoman::NLPLibs::Private::Normalizer::instance().init("../conf/Normalization.conf");

        for (int i=1; i<=0xFFFF; i++){
            QChar C = QChar(i);
            QString Normalized = Targoman::NLPLibs::Private::Normalizer::instance().normalize(C,QChar(),true,0,"",0);

            std::cerr<<QString("<0x%1>\t{%2}\t[%3]\t[%4]\t==>\t{%6}").arg(
                           QString::number(C.unicode(),16).toAscii().toUpper().constData()).arg(
                           ((C.isLetterOrNumber() || C.toAscii() == C)) ?
                               QString(C) : "N/A").arg(
                           QCHAR_UNICOE_CATEGORIES[C.category()]).arg(
                        enuUnicodeCharScripts::toStr(
                            (enuUnicodeCharScripts::Type)
                            QUnicodeTables::script(C.unicode()))).arg(
                        Normalized).toUtf8().constData()<<std::endl;
        }
    }catch(exNormalizer &e){
        qDebug()<<e.what();
    }
}


