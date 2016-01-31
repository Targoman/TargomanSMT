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

#include "NBestPath.h"

namespace Targoman{
namespace SMT {
namespace Private{
namespace NBestFinder {

clsTrellisPath::clsTrellisPath(const SearchGraphBuilder::clsSearchGraphNode &_node){

    for(size_t i = 0; i < SearchGraphBuilder::clsSearchGraphNodeData::RegisteredFeatureFunctionCount; ++i)
        if(_node.featureFunctionDataAt(i) != NULL)
            setFeatureFunctionData(i, _node.featureFunctionDataAt(i)->copy());

    this->Data->TotalCost = _node.getTotalCost();

    SearchGraphBuilder::clsSearchGraphNode Hypo = _node;
    while(!Hypo.isInvalid()){
        this->Data->Nodes.push_back(Hypo);
        Hypo = Hypo.prevNode();
    }

}

clsTrellisPath::clsTrellisPath(const clsTrellisPath &_prevPath, size_t _changedEdgeIndex, const SearchGraphBuilder::clsSearchGraphNode &_changedArc): Data(_prevPath.Data){

    QVector<SearchGraphBuilder::clsSearchGraphNode> _prevNodes = _prevPath.getNodes();
    this->Data->Nodes.reserve(_prevNodes.size());
    for (size_t currEdge = 0; currEdge < _changedEdgeIndex; currEdge++){
        this->Data->Nodes.push_back(_prevNodes.at(currEdge));
    }
    this->Data->Nodes.push_back(_changedArc);

    SearchGraphBuilder::clsSearchGraphNode NewHypo = _changedArc.prevNode();

    while(!NewHypo.isInvalid()){
        this->Data->Nodes.push_back(NewHypo);
        NewHypo = NewHypo.prevNode();
    }

    this->Data->TotalCost = _prevPath.getTotalCost();
    this->Data->TotalCost -= _prevNodes.at(_changedEdgeIndex).getCost();
    this->Data->TotalCost += _changedArc.getCost();

    for(size_t i = 0; i < SearchGraphBuilder::clsSearchGraphNodeData::RegisteredFeatureFunctionCount; ++i){
        if(_prevPath.featureFunctionDataAt(i) != NULL){
            //this->Data->FeatureFunctionsData.append(_prevPath.featureFunctionDataAt(i)->copy());
            QVector<Cost_t> PrevPathCosts = _prevPath.featureFunctionDataAt(i)->costElements();
            QVector<Cost_t> PrevArcCosts = _prevNodes.at(_changedEdgeIndex).featureFunctionDataAt(i)->costElements();
            QVector<Cost_t> NewArcCosts = _changedArc.featureFunctionDataAt(i)->costElements();

            for(size_t CostsIter = 0; CostsIter < PrevArcCosts.size(); CostsIter++){
                PrevPathCosts.replace(CostsIter, PrevPathCosts.at(CostsIter) - PrevArcCosts.at(CostsIter) + NewArcCosts.at(CostsIter));
            }
            setFeatureFunctionData(i, PrevPathCosts);
        }
    }

}

void clsTrellisPath::createDeviantPaths(clsTrellisPathCollection &pathCollection){

}

void clsTrellisPathCollection::prune(size_t newSize){

}

void NBestPath::retrieveNBestPaths(NBestPath::Container_t &_storage,
                                const SearchGraphBuilder::clsSearchGraph &_searchGraph,
                                const SearchGraphBuilder::clsCardinalityHypothesisContainer &_lastCardinality)
{

    clsTrellisPath bestPath(_searchGraph.goalNode());
    clsTrellisPathCollection bestPathsCollection;

    bestPathsCollection.add(bestPath);


    while(bestPathsCollection.getSize() > 0){

    }

}


}
}
}
}
