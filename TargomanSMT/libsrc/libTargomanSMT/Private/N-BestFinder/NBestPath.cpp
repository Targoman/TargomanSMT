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

using namespace Configuration;

TARGOMAN_REGISTER_SINGLETON_MODULE(NBestPath);

tmplRangedConfigurable<int> NBestPath::NBestPathSize(
        MAKE_CONFIG_PATH("NBestPathSize"),
        "Number of best paths to return",
        0, 1024,
        128
        );

tmplRangedConfigurable<int> NBestPath::NBestFactor(
        MAKE_CONFIG_PATH("NBestFactor"),
        "Pruning factor for `Distinct` paths case",
        0, 1024,
        512
        );

tmplConfigurable<bool> NBestPath::IsDistinct(
        MAKE_CONFIG_PATH("IsDistinct"),
        "Determines whether only distinct paths are to be obtained",
        true
        );

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

    QVector<SearchGraphBuilder::clsSearchGraphNode> PrevNodes = _prevPath.getNodes();
    this->Data->Nodes.reserve(PrevNodes.size());
    for (size_t currEdge = 0; currEdge < _changedEdgeIndex; currEdge++){
        this->Data->Nodes.push_back(PrevNodes.at(currEdge));
    }
    this->Data->Nodes.push_back(_changedArc);

    SearchGraphBuilder::clsSearchGraphNode NewHypo = _changedArc.prevNode();

    while(!NewHypo.isInvalid()){
        this->Data->Nodes.push_back(NewHypo);
        NewHypo = NewHypo.prevNode();
    }

    this->Data->TotalCost = _prevPath.getTotalCost();
    this->Data->TotalCost -= PrevNodes.at(_changedEdgeIndex).getCost();
    this->Data->TotalCost += _changedArc.getCost();

    for(size_t i = 0; i < SearchGraphBuilder::clsSearchGraphNodeData::RegisteredFeatureFunctionCount; ++i){
        if(_prevPath.featureFunctionDataAt(i) != NULL){
            //this->Data->FeatureFunctionsData.append(_prevPath.featureFunctionDataAt(i)->copy());
            QVector<Cost_t> PrevPathCosts = _prevPath.featureFunctionDataAt(i)->costElements();
            QVector<Cost_t> PrevArcCosts = PrevNodes.at(_changedEdgeIndex).featureFunctionDataAt(i)->costElements();
            QVector<Cost_t> NewArcCosts = _changedArc.featureFunctionDataAt(i)->costElements();

            for(int CostsIter = 0; CostsIter < PrevArcCosts.size(); CostsIter++){
                PrevPathCosts.replace(CostsIter, PrevPathCosts.at(CostsIter) - PrevArcCosts.at(CostsIter) + NewArcCosts.at(CostsIter));
            }
            setFeatureFunctionData(i, PrevPathCosts);
        }
    }

}

QString clsTrellisPath::printPath(){


    QString res = getTranslation();
    res += " |||";


    for(size_t i = 0; i < SearchGraphBuilder::clsSearchGraphNodeData::RegisteredFeatureFunctionCount; ++i){
        if(this->featureFunctionDataAt(i) != NULL){
            //// TODO: get feature names

            res += " Feature" + QString::number(i) + "=";
            QVector<Cost_t> costs = this->featureFunctionDataAt(i)->costElements();
            for(int j = 0; j < costs.size(); j++){
                res += " " + QString::number(costs.at(j));
            }
        }
    }
    res += " ||| " + QString::number(getTotalCost());

    return res;
}


void NBestPath::createDeviantPaths(const clsTrellisPath &_prevPath, clsTrellisPathCollection &_pathCollection, const size_t N){

    QVector<SearchGraphBuilder::clsSearchGraphNode> PathNodes = _prevPath.getNodes();
    if(_prevPath.getPrevEdgeChanged() == -1){
        for(size_t currEdge = 0; currEdge < _prevPath.getSize(); currEdge++){
            if(PathNodes.at(currEdge).isRecombined())
                continue;
             QList<SearchGraphBuilder::clsSearchGraphNode> CombinedNodes = PathNodes.at(currEdge).getCombindedNodes();
             for(int cn = 0; cn < CombinedNodes.size(); cn++){
                 clsTrellisPath NewPath(_prevPath, currEdge, CombinedNodes.at(cn));
                 _pathCollection.add(NewPath, N);

             }
        }
    }else{
        for(size_t currEdge = _prevPath.getPrevEdgeChanged() + 1; currEdge < _prevPath.getSize(); currEdge++){
            if(PathNodes.at(currEdge).isRecombined())
                continue;
             QList<SearchGraphBuilder::clsSearchGraphNode> CombinedNodes = PathNodes.at(currEdge).getCombindedNodes();
             for(int cn = 0; cn < CombinedNodes.size(); cn++){
                 clsTrellisPath NewPath(_prevPath, currEdge, CombinedNodes.at(cn));
                 _pathCollection.add(NewPath, N);

             }
        }
    }
}

void NBestPath::retrieveNBestPaths(NBestPath::Container_t &_storage,
                                const SearchGraphBuilder::clsSearchGraph &_searchGraph)
                                //,SearchGraphBuilder::clsCardinalityHypothesisContainer &_lastCardinality)
{


    int N = NBestPath::NBestPathSize.value();
    bool OnlyDistinct = NBestPath::IsDistinct.value();
    int NBestFactor = NBestPath::NBestFactor.value();    /// nbest-factor defines stopping point for distinct n-best list if too many candidates identical

    if (NBestFactor < 1) NBestFactor = 1000; /// 0 = unlimited

    clsTrellisPathCollection BestPathsCollection;
    QSet<QString> DistinctHypos;

    Coverage_t FullCoverage =_searchGraph.goalNode().coverage();

    SearchGraphBuilder::clsLexicalHypoNodeSet BestNodeSet = _searchGraph.getSameCoverageNodes(FullCoverage);

    for(int i = 0; i < BestNodeSet.size(); i++){
        clsTrellisPath BestPath(BestNodeSet.at(i));
        if(OnlyDistinct)
            BestPathsCollection.add(BestPath, N * NBestFactor);
        else
            BestPathsCollection.add(BestPath, N);
    }

    int iteration = 0;
    while(BestPathsCollection.getSize() > 0 && _storage.size() < N && (iteration < N * NBestFactor)){

        clsTrellisPath Path = BestPathsCollection.pop();
        if(OnlyDistinct){
            QString t = Path.getTranslation();
            if(DistinctHypos.find(t) == DistinctHypos.end()){
                _storage.push_back(Path);
                DistinctHypos.insert(t);
            }
            createDeviantPaths(Path, BestPathsCollection, N * NBestFactor);
        }else{
            _storage.push_back(Path);
            createDeviantPaths(Path, BestPathsCollection, N);
        }

        iteration++;

    }

}



}
}
}
}
