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

#include "NBestPaths.h"
#include "libTargomanCommon/Configuration/Validators.hpp"
#include <iostream>
#include "Private/OutputComposer/clsOutputComposer.h"

namespace Targoman{
namespace SMT {
namespace Private{
namespace NBestFinder {

using namespace Configuration;

TARGOMAN_REGISTER_SINGLETON_MODULE(NBestPaths);

tmplRangedConfigurable<int> NBestPaths::NBestPathCount(
        MAKE_CONFIG_PATH("NBestPathSize"),
        "Number of best paths to return",
        0, 1024,
        128
        );

tmplRangedConfigurable<int> NBestPaths::NBestExpansionFactor(
        MAKE_CONFIG_PATH("NBestExpansionFactor"),
        "Expansion factor when `Distinct` paths are required (we need to keep more paths than needed to choose N distinct paths from them)",
        0, 1024,
        512
        );

tmplConfigurable<bool> NBestPaths::OnlyDistinctPaths(
        MAKE_CONFIG_PATH("OnlyDistinctPaths"),
        "Determines whether only distinct paths are to be obtained",
        false
        );

QVector<Cost_t> getPathCostElements(const SearchGraphBuilder::clsSearchGraphNode& _node, int _featureID){
    QVector<Cost_t> result = _node.featureFunctionDataAt(_featureID)->costElements();
    SearchGraphBuilder::clsSearchGraphNode PrevNode = _node;
    PrevNode = PrevNode.prevNode();
    while(!PrevNode.isInvalid()) {
        QVector<Cost_t> v = PrevNode.featureFunctionDataAt(_featureID)->costElements();
        for(int j = 0; j < result.size(); j++){
            result[j] += v[j];
        }
        PrevNode = PrevNode.prevNode();
    }
    return result;
}

clsTrellisPath::clsTrellisPath() : Data(new clsTrellisPathData()) { }

clsTrellisPath::clsTrellisPath(const SearchGraphBuilder::clsSearchGraphNode &_node):
    Data(new clsTrellisPathData())
{

    this->Data->Nodes.push_back(_node);
    SearchGraphBuilder::clsSearchGraphNode PrevNode = _node.prevNode();
    while(!PrevNode.isInvalid()){
        this->Data->Nodes.push_back(PrevNode);
        PrevNode = PrevNode.prevNode();
    }

    this->Data->TotalCost = _node.getTotalCost();

    for(size_t i = 0; i < SearchGraphBuilder::clsSearchGraphNodeData::RegisteredFeatureFunctionCount; ++i){
       if(_node.featureFunctionDataAt(i) != NULL){
            setFeatureFunctionData(i, _node.featureFunctionDataAt(i)->copy());
            QVector<Cost_t> v = getPathCostElements(_node, i);
            updateFeatureFunctionCosts(i, v);
       }
    }
}

clsTrellisPath clsTrellisPath::createDeviantPath(size_t _substitutedNodeIndex, const SearchGraphBuilder::clsSearchGraphNode &_newNode) const
{
    clsTrellisPath DeviantPath;

    QList<SearchGraphBuilder::clsSearchGraphNode> CurrentNodes = this->getNodes();

    // Keep the current path up until the given node
    DeviantPath.Data->Nodes.reserve(CurrentNodes.size());
    for (size_t nodeIndex = 0; nodeIndex < _substitutedNodeIndex; nodeIndex++)
        DeviantPath.Data->Nodes.push_back(CurrentNodes[nodeIndex]);

    // Add the new arc
    DeviantPath.Data->Nodes.push_back(_newNode);
    SearchGraphBuilder::clsSearchGraphNode PrevNode = _newNode.prevNode();
    while(!PrevNode.isInvalid()){
        DeviantPath.Data->Nodes.push_back(PrevNode);
        PrevNode = PrevNode.prevNode();
    }

    // Update the costs
    DeviantPath.Data->TotalCost = this->getTotalCost();
    DeviantPath.Data->TotalCost -= CurrentNodes[_substitutedNodeIndex].getCost();
    DeviantPath.Data->TotalCost += _newNode.getCost();

    for(size_t i = 0; i < SearchGraphBuilder::clsSearchGraphNodeData::RegisteredFeatureFunctionCount; ++i){
        if(this->featureFunctionDataAt(i) != NULL){
            QVector<Cost_t> PrevPathCosts = this->featureFunctionDataAt(i)->costElements();
            QVector<Cost_t> PrevArcCosts = getPathCostElements(CurrentNodes[_substitutedNodeIndex], i);
            QVector<Cost_t> NewArcCosts = getPathCostElements(_newNode, i);

            for(int CostsIter = 0; CostsIter < PrevArcCosts.size(); CostsIter++)
                PrevPathCosts.replace(CostsIter, PrevPathCosts[CostsIter] - PrevArcCosts[CostsIter] + NewArcCosts[CostsIter]);

            DeviantPath.Data->FeatureFunctionsData[i] = this->featureFunctionDataAt(i)->copy();
            DeviantPath.updateFeatureFunctionCosts(i, PrevPathCosts);
        }
    }

    DeviantPath.Data->PrevSubstitutedNodeIndex = _substitutedNodeIndex;

    return DeviantPath;
}

void NBestPaths::expandAvailablePaths(const clsTrellisPath &_prevPath, clsTrellisPathCollection &_pathCollection, const size_t _maxSize)
{
    const QList<SearchGraphBuilder::clsSearchGraphNode>& PathNodes = _prevPath.getNodes();
    for(size_t nodeIndex = _prevPath.getPrevSubstitutedNodeIndex() + 1; nodeIndex < _prevPath.getSize(); nodeIndex++){
        if(PathNodes[nodeIndex].isRecombined())
            continue;
         QList<SearchGraphBuilder::clsSearchGraphNode> CombinedNodes = PathNodes[nodeIndex].getCombindedNodes();
         for(int CombinedNodeIndex = 0; CombinedNodeIndex < CombinedNodes.size(); CombinedNodeIndex++)
             _pathCollection.add(
                         _prevPath.createDeviantPath(nodeIndex, CombinedNodes[CombinedNodeIndex]),
                         _maxSize);
    }
}

NBestPaths::Container_t NBestPaths::retrieve(const SearchGraphBuilder::clsSearchGraph &_searchGraph, OutputComposer::clsOutputComposer &_outputComposer)
{

    NBestPaths::Container_t Storage;

    int N = NBestPaths::NBestPathCount.value();
    bool OnlyDistinct = NBestPaths::OnlyDistinctPaths.value();
    int ExpansionFactor = NBestPaths::NBestExpansionFactor.value();    /// nbest-factor defines stopping point for distinct n-best list if too many candidates identical

    if (ExpansionFactor == 0)
        ExpansionFactor = 1000; /// 0 = unlimited

    clsTrellisPathCollection BestPathsCollection;
    QSet<QString> DistinctHypos;

    Coverage_t FullCoverage =_searchGraph.goalNode().coverage();

    SearchGraphBuilder::clsLexicalHypoNodeSet BestNodeSet = _searchGraph.getSameCoverageNodes(FullCoverage);

    for(int i = 0; i < BestNodeSet.size(); i++) {
        clsTrellisPath BestPath(BestNodeSet.at(i));
        if(OnlyDistinct)
            BestPathsCollection.add(BestPath, N * ExpansionFactor);
        else
            BestPathsCollection.add(BestPath, N);
    }

    int Iteration = 0;
    while(BestPathsCollection.getSize() > 0 && Storage.size() < N && (Iteration < N * ExpansionFactor)) {
        clsTrellisPath Path = BestPathsCollection.pop();
        if(OnlyDistinct) {
            QString t = _outputComposer.pathTranslation(Path.getNodes());
            if(DistinctHypos.find(t) == DistinctHypos.end()){
                Storage.push_back(Path);
                DistinctHypos.insert(t);
            }
            expandAvailablePaths(Path, BestPathsCollection, N * ExpansionFactor);
        } else {
            Storage.push_back(Path);
            expandAvailablePaths(Path, BestPathsCollection, N);
        }
        Iteration++;

    }

    return Storage;

}



}
}
}
}
