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

#ifndef UNITTEST_H
#define UNITTEST_H

#include <QtTest/QtTest>

#include "libTargomanTextProcessor/TextProcessor.h"

class UnitTest: public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void normalizeText();
    void text2IXML();
    void ixml2Text();
    void text2RichIXML();
    void richIXML2Text();
};

#endif // UNITTEST_H
