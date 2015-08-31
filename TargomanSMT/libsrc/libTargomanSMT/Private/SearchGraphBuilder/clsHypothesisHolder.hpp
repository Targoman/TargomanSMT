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
 * @author S. Mohammad M. Ziabary <ziabary@targoman.com>
 * @author Behrooz Vedadian <vedadian@targoman.com>
 * @author Saeed Torabzadeh <saeed.torabzadeh@targoman.com>
 */

#ifndef TARGOMAN_CORE_PRIVATE_SEARCHGRAPHBUILDER_CLSHYPOTHESISHOLDER_H
#define TARGOMAN_CORE_PRIVATE_SEARCHGRAPHBUILDER_CLSHYPOTHESISHOLDER_H

#include <QVector>
#include "clsCardinality.h"
#include "clsSearchGraphNode.h"
#include "libTargomanCommon/Types.h"

namespace Targoman{
namespace SMT {
namespace Private{
namespace SearchGraphBuilder {

class clsHypothesisHolderData : public QSharedData
{
public:
    clsHypothesisHolderData(size_t _sentenceSize){
        this->EmptyCoverage.resize(_sentenceSize);
        this->EmptyCoverage.fill(0);
    }
    clsHypothesisHolderData(const clsHypothesisHolderData& _other):
        QSharedData(_other),
        CardinalityHypotheses(_other.CardinalityHypotheses)
    {}
    ~clsHypothesisHolderData() {}

public:
    QVector<clsCardinalityHypothesisContainer> CardinalityHypotheses;
    Coverage_t EmptyCoverage;

    friend class UnitTestNameSpace::clsUnitTest;
};

class clsHypothesisHolder
{
public:
    clsHypothesisHolder(size_t _sentenceSize) :
        Data(new clsHypothesisHolderData(_sentenceSize))
    {}
    clsHypothesisHolder(){}
    clsHypothesisHolder(const clsHypothesisHolder& _other) :
        Data(_other.Data)
    {}

    void clear() {
        this->Data->CardinalityHypotheses.clear();
        this->Data->CardinalityHypotheses.append(clsCardinalityHypothesisContainer::rootCardinalityHypothesisContainer(this->Data->EmptyCoverage));
    }
    void resize(size_t _size) {
            this->Data->CardinalityHypotheses.resize(_size);
    }

    inline const clsSearchGraphNode& getRootNode(){
        return this->Data->CardinalityHypotheses[0][this->Data->EmptyCoverage].nodes().first();
    }

    inline clsCardinalityHypothesisContainer& operator [](size_t _index){
        return this->Data->CardinalityHypotheses[_index];
    }

    inline size_t size() const{
        return this->Data->CardinalityHypotheses.size();
    }


private:
    QExplicitlySharedDataPointer<clsHypothesisHolderData> Data;

    friend class UnitTestNameSpace::clsUnitTest;

};

}
}
}
}

#endif // TARGOMAN_CORE_PRIVATE_SEARCHGRAPHBUILDER_CLSHYPOTHESISHOLDER_H
