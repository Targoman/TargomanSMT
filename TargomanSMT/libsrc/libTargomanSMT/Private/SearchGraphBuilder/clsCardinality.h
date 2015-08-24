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

#ifndef TARGOMAN_CORE_PRIVATE_SEARCHGRAPHBUILDER_CLSCARDINALITY_H
#define TARGOMAN_CORE_PRIVATE_SEARCHGRAPHBUILDER_CLSCARDINALITY_H

#include <QHash>
#include <QBitArray>
#include "clsLexicalHypothesis.h"
#include "Private/PrivateTypes.h"

namespace Targoman{
namespace SMT {
namespace Private{
namespace SearchGraphBuilder {

/**
 * @brief CoverageLexicalHypothesisMap_t    used to make the source code more readable
 */
typedef QMap<Coverage_t, clsLexicalHypothesisContainer> CoverageLexicalHypothesisMap_t;

/**
 * @brief The clsCardinalityHypothesisContainerData class   placeholder for the cardinality hypothesis holder data
 */
class clsCardinalityHypothesisContainerData : public QSharedData
{
public:
    clsCardinalityHypothesisContainerData()
    {
        this->SelectedLexicalHypothesis = NULL;
        this->WorstLexicalHypothesis = NULL;
        this->BestLexicalHypothesis = NULL;
        this->TotalSearchGraphNodeCount = 0;
        this->CostLimit = INFINITY;
    }

    clsCardinalityHypothesisContainerData(const clsCardinalityHypothesisContainerData& _other) :
        QSharedData(_other),
        LexicalHypothesisContainer(_other.LexicalHypothesisContainer),
        SelectedCoverage(_other.SelectedCoverage),
        SelectedLexicalHypothesis(_other.SelectedLexicalHypothesis),
        TotalSearchGraphNodeCount(_other.TotalSearchGraphNodeCount),
        BestCoverage(_other.BestCoverage),
        BestLexicalHypothesis(_other.BestLexicalHypothesis),
        WorstCoverage(_other.WorstCoverage),
        WorstLexicalHypothesis(_other.WorstLexicalHypothesis),
        CostLimit(_other.CostLimit)
    {}

    ~clsCardinalityHypothesisContainerData(){}

public:
    CoverageLexicalHypothesisMap_t  LexicalHypothesisContainer;
    Coverage_t                      SelectedCoverage;
    clsLexicalHypothesisContainer*  SelectedLexicalHypothesis;
    size_t                          TotalSearchGraphNodeCount;
    Coverage_t                      BestCoverage;
    clsLexicalHypothesisContainer*  BestLexicalHypothesis;
    Coverage_t                      WorstCoverage;
    clsLexicalHypothesisContainer*  WorstLexicalHypothesis;
    Common::Cost_t                  CostLimit;

    friend class UnitTestNameSpace::clsUnitTest;
};

/**
 * @brief The clsCardinalityHypothesisContainer class   managing unit for hypothesis covering the same number of source words
 */
class clsCardinalityHypothesisContainer
{
public:
    clsCardinalityHypothesisContainer();
    clsCardinalityHypothesisContainer(const clsCardinalityHypothesisContainer& _other) :
        Data(_other.Data)
    {}
    ~clsCardinalityHypothesisContainer(){}

    /**
     * @brief operator []   access operator that gives the lexical hypothesis container for any given coverage
     * @param _coverage     the coverage for which we want to get the lexical hypothesis container
     * @return
     */
    inline clsLexicalHypothesisContainer& operator [] (const Coverage_t& _coverage){
        /*if(this->Data->LexicalHypothesisContainer.contains(_coverage) == false){
            TargomanDebug(1,"Creating new LexHypo for coverage = " + bitArray2Str(_coverage));
        }*/
        return this->Data->LexicalHypothesisContainer[_coverage];
    }

    /**
     * @brief isEmpty   returns whether this cardinality contains any hypothesis
     * @return          returns true if this cardinality contains any hypothesis and false otherwise
     */
    inline size_t isEmpty(){
        return this->Data->LexicalHypothesisContainer.isEmpty();
    }

    /**
     * @brief lexicalHypotheses returns the collection of all lexical hypotheses
     * @return                  returns the map holding the lexical hypotheses
     */
    inline CoverageLexicalHypothesisMap_t& lexicalHypotheses(){
        return this->Data->LexicalHypothesisContainer;
    }

    /**
     * @brief rootCardinalityHypothesisContainer    instantiates the root container for root translation node which represents no source word translation (empty node)
     * @param _emptyCoverage                        this parameter is used to avoid creation of multiple instances of empty coverage
     * @return                                      the root cardinality hypothesis container holding translation start (empty node)
     */
    inline static clsCardinalityHypothesisContainer rootCardinalityHypothesisContainer(Coverage_t _emptyCoverage){
        clsCardinalityHypothesisContainer CoverageContainer;
        CoverageContainer.Data->LexicalHypothesisContainer.insert(_emptyCoverage,
                                                            clsLexicalHypothesisContainer::rootLexicalHypothesis());
        return CoverageContainer;
    }

    /**
     * @brief remove        removes lexical hypothesis container corresponding the given coverage and updates the internal parameters
     * @param _coverage     input coverage corresponding to which the lexical hypothesis container will be removed from the list
     */
    void remove(Coverage_t _coverage){
        this->Data->LexicalHypothesisContainer.remove(_coverage);
        if (this->Data->WorstLexicalHypothesis != NULL && this->Data->WorstCoverage == _coverage){
            this->updateWorstNode();
        }
    }

    /**
     * @brief setLexicalHypothesis   selects the lexical hypothesis targetted for hypothesis insertion
     * @param _coverage              coverage of the targetted hypothesis
     */
    void setLexicalHypothesis(const Coverage_t& _coverage);

    /**
     * @brief removeSelectedLexicalHypothesisIfEmpty   Removes the selected lexical hypothesis if it is empty and unset the selection
     */
    void removeSelectedLexicalHypothesisIfEmpty();

    /**
     * @brief insertNewHypothesis   inserts the new hypothesis into its corresponding lexical hypothesis container and updates its internal parameters (worst node, total number of hypothesis and etc)
     * @param _node                 the new translation hypothesis to be inserted
     * @return
     */
    bool insertNewHypothesis(clsSearchGraphNode& _node);

    /**
     * @brief mustBePruned  preinsertion pruning facilitator.
     *
     * this function indicates whether any node with the given cost can be inserted or is
     * faced to be pruned. It could possibly be used during the decode process to avoid
     * unnecessary hypothesis container updates
     *
     * @param _cost         the cost to be checked for pruning
     * @return              true if the insertion is unnecessary and false otherwise
     */
    bool mustBePruned(Common::Cost_t _cost) const;

    /**
     * @brief finlizePruningAndcleanUp  finalize the pruning for this cardinality container
     */
    inline void finlizePruningAndcleanUp() {
        this->prune();
#ifdef TARGOMAN_SHOW_DEBUG
        auto LexHypoContainerIter = this->Data->LexicalHypothesisContainer.begin();
        while(LexHypoContainerIter != this->Data->LexicalHypothesisContainer.end()) {
            Q_ASSERT(LexHypoContainerIter->nodes().size());
            ++LexHypoContainerIter;
        }
#endif
    }

    /**
     * @brief totalSearchGraphNodeCount getter function for the total translation hypothesis nodes count
     * @return                          total number of hypotheses contained in this container
     */
    size_t totalSearchGraphNodeCount() const{return this->Data->TotalSearchGraphNodeCount;}

private:
    /**
     * @brief updateWorstNode   updates the worst node cache for this cardinality
     */
    void updateWorstNode();

    /**
     * @brief updateWorstNode   updates the best node cache for this cardinality
     */
    void updateBestAndWorstNodes();

    /**
     * @brief pruneAndUpdateWorstNode   helper function that performs pruning of the whole cardinality hypothesis container and updates the worst node cache
     * @param _coverage                 the coverage corresponding to the lexical hypothesis (input just for speed and memory optimization purposes)
     * @param _lexicalHypo              the lexcial hypothesis corresponding to the translation hypothesis node (input just for speed and memory optimization purposes)
     * @param _node                     the translation hypothesis node that is inserted just before calling this helper function
     */
    void localUpdateBestAndWorstNodes(const Coverage_t& _coverage, clsLexicalHypothesisContainer& _container, const clsSearchGraphNode &_node);

    /**
     * @brief prune performs pruning (called in a lazy manner)
     */
    void prune();

private:
    static size_t MaxCardinalitySizeLazyPruning;

public:
    static void setHardReorderingJumpLimit(int _hardReorderingJumpLimit) {
        clsCardinalityHypothesisContainer::MaxCardinalitySizeLazyPruning = 2 * clsCardinalityHypothesisContainer::MaxCardinalityContainerSize.value() - 1;
        if(clsCardinalityHypothesisContainer::PrimaryCoverageShare.value() != 0)
            clsCardinalityHypothesisContainer::MaxCardinalitySizeLazyPruning +=
                clsCardinalityHypothesisContainer::PrimaryCoverageShare.value() << _hardReorderingJumpLimit;
    }

private:
    QExplicitlySharedDataPointer<clsCardinalityHypothesisContainerData> Data;
    static Common::Configuration::tmplConfigurable<quint16>  MaxCardinalityContainerSize; /**< Maximum number of nodes contained in a cardinality hypothesis container*/
    static Common::Configuration::tmplConfigurable<quint8>  PrimaryCoverageShare; /**< Primary share of each coverage from the cardinality nodes.*/
    static Common::Configuration::tmplConfigurable<double>  SearchBeamWidth; /**< Beam width for the beam search */

    friend class UnitTestNameSpace::clsUnitTest;

};

}
}
}
}

#endif // TARGOMAN_CORE_PRIVATE_SEARCHGRAPHBUILDER_CLSCARDINALITY_H
