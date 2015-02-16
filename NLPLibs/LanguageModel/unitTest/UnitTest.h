/*************************************************************************
 * Copyright © 2012-2015, Targoman.com
 *
 * Published under the terms of TCRL(Targoman Community Research License)
 * You can find a copy of the license file with distributed source or
 * download it from http://targoman.com/License.txt
 *
 *************************************************************************/
/**
 *@author Saeed Torabzadeh <saeed.torabzadeh@gmail.com>
 */

#ifndef UNITTEST_H
#define UNITTEST_H


#include <QTest>

#include "libLanguageModel/clsLanguageModel.h"
#include "libLanguageModel/clsLMSentenceScorer.h"

class UnitTest: public QObject
{
    Q_OBJECT

private slots:
void testIndexBased();
void testStringBased();
};

#endif // UNITTEST_H
