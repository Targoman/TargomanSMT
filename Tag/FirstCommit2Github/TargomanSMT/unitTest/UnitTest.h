/******************************************************************************
 * Targoman: A robust Statistical Machine Translation framework               *
 *                                                                            *
 * Copyright 2014-2015 by ITRC <http://itrc.ac.ir>                            *
 *                                                                            *
 * This file is part of Targoman.                                             *
 *                                                                            *
 * Targoman is free software: you can redistribute it and/or modify           *
 * it under the terms of the GNU Lesser General Public License as published   *
 * by the Free Software Foundation, either version 3 of the License, or       *
 * (at your option) any later version.                                        *
 *                                                                            *
 * Targoman is distributed in the hope that it will be useful,                *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of             *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              *
 * GNU Lesser General Public License for more details.                        *
 * You should have received a copy of the GNU Lesser General Public License   *
 * along with Targoman. If not, see <http://www.gnu.org/licenses/>.           *
 *                                                                            *
 ******************************************************************************/
/**
 * @author Saeed Torabzadeh <saeed.torabzadeh@targoman.com>
 */


#ifndef UNITTEST_H
#define UNITTEST_H


#include "../libsrc/libTargomanSMT/Private/InputDecomposer/clsInput.h"
#include "../libsrc/libTargomanSMT/Private/SearchGraphBuilder/clsSearchGraph.h"
#include "../libsrc/libTargomanSMT/Private/N-BestFinder/NBestSuggestions.h"
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
