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

#include "UnitTest.h"
using namespace UnitTestNameSpace;
using namespace Targoman::SMT::Private::SearchGraphBuilder;

void clsUnitTest::test_clsSearchGraphBuilder_conformsIBM1Constraint()
{
     Coverage_t  TestConverage;
     clsSearchGraph SearchGraphBuilder(false,InputDecomposer::Sentence_t());

     clsSearchGraph::ReorderingConstraintMaximumRuns.setFromVariant(4);
     TestConverage = makeCoverageByString("110000111000");
     QVERIFY(SearchGraphBuilder.conformsIBM1Constraint(TestConverage) == true);

     clsSearchGraph::ReorderingConstraintMaximumRuns.setFromVariant(3);
     QVERIFY(SearchGraphBuilder.conformsIBM1Constraint(TestConverage) == false);

     clsSearchGraph::ReorderingConstraintMaximumRuns.setFromVariant(4);
     TestConverage = makeCoverageByString("110000");
     QVERIFY(SearchGraphBuilder.conformsIBM1Constraint(TestConverage) == true);

     TestConverage = makeCoverageByString("000000");
     QVERIFY(SearchGraphBuilder.conformsIBM1Constraint(TestConverage) == true);

     TestConverage = makeCoverageByString("111111");
     QVERIFY(SearchGraphBuilder.conformsIBM1Constraint(TestConverage) == true);

}
