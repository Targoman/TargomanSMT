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


#include "../libsrc/libTargomanSMT/Private/InputDecomposer/clsInput.h"
#include "../libsrc/libTargomanSMT/Private/SearchGraph/clsSearchGraphBuilder.h"
#include "../libsrc/libTargomanSMT/Private/N-BestFinder/clsNBestFinder.h"
#include "../libsrc/libTargomanSMT/Private/OutputComposer/clsOutputComposer.h"
#include <QtTest/QtTest>
#include <iostream>
#include <iomanip>

inline void replace_all_indexes(QString& s, const QString& allTokens, const QVector<int>& replacements)
{
    QStringList tokens = allTokens.split(",", QString::SkipEmptyParts);
    for(int i = 0; i < tokens.size(); ++i) {
        s.replace(QString("(%1)").arg(tokens.at(i)), "(" + QString::number(replacements.at(i)) + ")");
    }
}

inline std::ostream& operator << (std::ostream& o, const QString& s)
{
    o << "\"" << s.toUtf8().constData() << "\"";
    return o;
}


#define PRINT_QVERIFY(statement,...) \
{\
    QString theStatement = #statement;\
    replace_all_indexes(theStatement, #__VA_ARGS__, QVector<int>::fromStdVector({ __VA_ARGS__ }));\
    std::cout << "QVERIFY( " << theStatement.toUtf8().constData() << " == " << statement << " );\n";\
}


#define PRINT_QVERIFY_DOUBLE_PRECISIONS(statement,...) \
{\
    std::cout.precision(15);\
    QString theStatement = #statement;\
    replace_all_indexes(theStatement, #__VA_ARGS__, QVector<int>::fromStdVector({ __VA_ARGS__ }));\
    if(isinf(statement))\
        std::cout << "QVERIFY( isinf(" << theStatement.toUtf8().constData() << ") );\n";\
    else\
        std::cout << "QVERIFY( qFuzzyCompare(" << theStatement.toUtf8().constData() << ", " << statement << ") );\n";\
}

using namespace Targoman::SMT::Private;

namespace UnitTestNameSpace {

double PredictableRandom();

class clsUnitTest: public QObject
{
    Q_OBJECT

public:
    clsUnitTest();
    ~clsUnitTest();

private:
    Coverage_t makeCoverageByString(QString _coverageString);

//    QScopedPointer<InputDecomposer::clsInputDecomposer> Input;
//    QScopedPointer<SearchGraph::clsSearchGraphBuilder> SearchGraphBuilder;
//    QScopedPointer<NBestFinder::clsNBestFinder> NBest;
//    QScopedPointer<OutputComposer::clsOutputComposer> Output;

private slots:
    void initTestCase();
    void test_clsInputDecomposer_init();
    void test_clsInputDecomposer_parseRichIXML();
    void test_clsSearchGraphBuilder_conformsIBM1Constraint();
    void test_clsSearchGraphBuilder_collectPhraseCandidates();
    void test_clsSearchGraphBuilder_initializeRestCostsMatrix();
    void test_clsSearchGraphBuilder_conformsHardReorderingJumpLimit();
    void test_clsSearchGraphBuilder_calculateRestCost();
    void test_ReorderingJump_getRestCostForPosition();
    void test_clsLexicalHypothesisContainer_insertHypothesis();
    void test_clsNBestFinder_fillBestOptions();
};
}
#endif // UNITTEST_H
