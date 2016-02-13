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
 * @author Fatemeh Azadi <f.azadi@targoman.com>
 */

#ifndef TARGOMAN_CORE_PRIVATE_NBESTFINDER_NBESTPATH_H
#define TARGOMAN_CORE_PRIVATE_NBESTFINDER_NBESTPATH_H

#include "libTargomanCommon/CmdIO.h"
#include "libTargomanCommon/Configuration/tmplConfigurable.h"
#include "libTargomanCommon/Types.h"
#include "Private/SearchGraphBuilder/clsLexicalHypothesis.h"
#include "Private/SearchGraphBuilder/clsCardinality.h"
#include "Private/SearchGraphBuilder/clsSearchGraph.h"
#include "Private/SearchGraphBuilder/clsSearchGraphNode.h"
#include "Private/FeatureFunctions/intfFeatureFunction.hpp"
#include "libTargomanCommon/Configuration/tmplConfigurable.h"

namespace Targoman{
namespace SMT {
namespace Private{

/**
 *  @brief NBest finder module
 */
namespace NBestFinder {

class clsTrellisPathData;
class clsTrellisPathCollection;

class clsTrellisPath {

public:
    clsTrellisPath(){}
    clsTrellisPath(const SearchGraphBuilder::clsSearchGraphNode& _node);
    clsTrellisPath(const clsTrellisPath& _prevPath, size_t _changedEdgeIndex, const SearchGraphBuilder::clsSearchGraphNode& _changedArc);
    clsTrellisPath(const clsTrellisPath& _other) : Data(_other.Data) {}

    inline Common::Cost_t getTotalCost() const;
    inline const QVector<SearchGraphBuilder::clsSearchGraphNode>& getNodes() const;
    inline int getPrevEdgeChanged() const;
    inline const SearchGraphBuilder::intfFeatureFunctionData * featureFunctionDataAt(size_t _index) const;
    inline void setFeatureFunctionData(size_t _index, QVector<Cost_t>& newCosts);
    inline void setFeatureFunctionData(size_t _index, SearchGraphBuilder::intfFeatureFunctionData* _data);
    inline size_t getSize() const;
    inline QString getTranslation();

    QString printPath();


private:
    QExplicitlySharedDataPointer<clsTrellisPathData>     Data;
};

class clsTrellisPathData : public QSharedData{
public:
    clsTrellisPathData() :
        Nodes(0),
        PrevEdgeChanged(-1),
        TotalCost(0),
        FeatureFunctionsData(SearchGraphBuilder::clsSearchGraphNodeData::RegisteredFeatureFunctionCount, NULL)
    {
    }

    clsTrellisPathData(const clsTrellisPathData& _other) :
        QSharedData(_other),
        Nodes(_other.Nodes),
        PrevEdgeChanged(_other.PrevEdgeChanged),
        TotalCost(_other.TotalCost),
        FeatureFunctionsData(_other.FeatureFunctionsData.size())
    {
        for(int i = 0; i < this->FeatureFunctionsData.size(); ++i)
            this->FeatureFunctionsData[i] = _other.FeatureFunctionsData.at(i)->copy();
    }

    ~clsTrellisPathData() {
        for(int i = 0; i < this->FeatureFunctionsData.size(); ++i) {
            if(this->FeatureFunctionsData.at(i) != NULL)
                delete FeatureFunctionsData[i];
        }
    }

public:
    QVector<SearchGraphBuilder::clsSearchGraphNode> Nodes;
    int PrevEdgeChanged;
    Common::Cost_t  TotalCost;
    QVector<SearchGraphBuilder::intfFeatureFunctionData*>   FeatureFunctionsData;

};

class clsTrellisPathCollection{
    QMultiMap<Cost_t, clsTrellisPath> Collection;

public:
    void add(clsTrellisPath _path, size_t maxSize){

        Collection.insertMulti(_path.getTotalCost(), _path);

        while(Collection.size() > (int)maxSize && maxSize > 0){
            QMutableMapIterator<Cost_t, clsTrellisPath> iter(Collection);
            iter.toBack();
            Collection.erase(iter.previous());
        }
    }

    clsTrellisPath pop() {
        clsTrellisPath path = Collection.begin().value();
        Collection.erase(Collection.begin());
        return path;
    }

    size_t getSize(){
        return Collection.size();
    }


};

class NBestPath : public Configuration::intfModule
{
public:
    typedef QVector<clsTrellisPath> Container_t;

public:
    static void retrieveNBestPaths(NBestPath::Container_t& _storage,
                                   const SearchGraphBuilder::clsSearchGraph &_searchGraph);
                                   //, SearchGraphBuilder::clsCardinalityHypothesisContainer& _lastCardinality);

    static void createDeviantPaths(const clsTrellisPath &_prevPath, clsTrellisPathCollection &_pathCollection, const size_t N);

private:
    static Configuration::tmplRangedConfigurable<int> NBestPathSize;
    static Configuration::tmplConfigurable<bool> IsDistinct;
    static Configuration::tmplRangedConfigurable<int> NBestFactor;

    TARGOMAN_DEFINE_SINGLETON_MODULE(NBestPath);

private:
    NBestPath() {
    }

    friend class UnitTestNameSpace::clsUnitTest;
};

inline Common::Cost_t clsTrellisPath::getTotalCost() const{
    return this->Data->TotalCost;
}

inline void clsTrellisPath::setFeatureFunctionData(size_t _index, QVector<Cost_t> &newCosts){
    Data->FeatureFunctionsData[_index]->setCostElements(newCosts);
}

inline void clsTrellisPath::setFeatureFunctionData(size_t _index, SearchGraphBuilder::intfFeatureFunctionData* _data){
    this->Data->FeatureFunctionsData[_index] = _data;
}

inline const SearchGraphBuilder::intfFeatureFunctionData * clsTrellisPath::featureFunctionDataAt(size_t _index) const {
    return this->Data->FeatureFunctionsData.at(_index);
}

inline int clsTrellisPath::getPrevEdgeChanged() const{
    return this->Data->PrevEdgeChanged;
}
inline size_t clsTrellisPath::getSize() const{
    return this->Data->Nodes.size();
}

inline const QVector<SearchGraphBuilder::clsSearchGraphNode>& clsTrellisPath::getNodes() const{
    return this->Data->Nodes;
}

inline QString clsTrellisPath::getTranslation() {
    QVector<SearchGraphBuilder::clsSearchGraphNode> PathNodes = this->getNodes();
    QString Translation = "";
    for(int i = PathNodes.size() - 1; i >= 0 ; i--){
        if(Translation.length() > 0)
            Translation += ' ';
        Translation += PathNodes[i].targetRule().toStr();
    }
    return Translation;
}

}
}
}
}

#endif // TARGOMAN_CORE_PRIVATE_NBESTFINDER_NBESTPATH_H
