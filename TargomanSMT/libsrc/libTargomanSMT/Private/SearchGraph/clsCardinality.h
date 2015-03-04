/*************************************************************************
 * Copyright © 2012-2015, Targoman.com
 *
 * Published under the terms of TCRL(Targoman Community Research License)
 * You can find a copy of the license file with distributed source or
 * download it from http://targoman.com/License.txt
 *
 *************************************************************************/
/**
 @author S. Mohammad M. Ziabary <smm@ziabary.com>
 @author Behrooz Vedadian <vedadian@gmail.com>
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
namespace SearchGraph {

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
    clsCardinalityHypothesisContainerData(){
        this->WorstLexicalHypothesis = NULL;
        this->BestLexicalHypothesis = NULL;
        this->TotalSearchGraphNodeCount = 0;
        this->WorstCostLimit = INFINITY;
    }

    clsCardinalityHypothesisContainerData(const clsCardinalityHypothesisContainerData& _other) :
        QSharedData(_other),
        LexicalHypothesisContainer(_other.LexicalHypothesisContainer),
        TotalSearchGraphNodeCount(_other.TotalSearchGraphNodeCount),
        BestCoverage(_other.BestCoverage),
        BestLexicalHypothesis(_other.BestLexicalHypothesis),
        WorstCoverage(_other.WorstCoverage),
        WorstLexicalHypothesis(_other.WorstLexicalHypothesis),
        WorstCostLimit(_other.WorstCostLimit)
    {}

    ~clsCardinalityHypothesisContainerData(){}

public:
    CoverageLexicalHypothesisMap_t  LexicalHypothesisContainer;
    size_t                          TotalSearchGraphNodeCount;
    Coverage_t                      BestCoverage;
    clsLexicalHypothesisContainer*  BestLexicalHypothesis;
    Coverage_t                      WorstCoverage;
    clsLexicalHypothesisContainer*  WorstLexicalHypothesis;
    Common::Cost_t                  WorstCostLimit;
};

/**
 * @brief The clsCardinalityHypothesisContainer class   managing unit for hypothesis covering the same number of source words
 */
class clsCardinalityHypothesisContainer
{
public:
    clsCardinalityHypothesisContainer();
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
     * @brief insertNewHypothesis   inserts the new hypothesis into its corresponding lexical hypothesis container and updates its internal parameters (worst node, total number of hypothesis and etc)
     * @param _node                 the new translation hypothesis to be inserted
     * @return
     */
    bool insertNewHypothesis(clsSearchGraphNode& _node);

    /**
     * @brief isPruningNecessary
     * @return  returns true if expanding this container makes pruning necessary
     */
    inline bool isPruningNecessary() const {
        if (clsCardinalityHypothesisContainer::MaxCardinalityContainerSize.value() == 0 ||
            this->Data->TotalSearchGraphNodeCount <
                clsCardinalityHypothesisContainer::MaxCardinalityContainerSize.value())
            return false;
        return true;
    }

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
        auto LexHypoContainerIter = this->Data->LexicalHypothesisContainer.begin();
        while(LexHypoContainerIter != this->Data->LexicalHypothesisContainer.end()) {
            if(LexHypoContainerIter->nodes().isEmpty())
                LexHypoContainerIter = this->Data->LexicalHypothesisContainer.erase(LexHypoContainerIter);
            else
                ++LexHypoContainerIter;
        }
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
    void pruneAndUpdateBestAndWorstNode(const Coverage_t& _coverage, clsLexicalHypothesisContainer& _container, const clsSearchGraphNode &_node);

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
    static Common::Configuration::tmplConfigurable<quint8>  MaxCardinalityContainerSize; /**< Maximum number of nodes contained in a cardinality hypothesis container*/
    static Common::Configuration::tmplConfigurable<quint8>  PrimaryCoverageShare; /**< Primary share of each coverage from the cardinality nodes.*/
    static Common::Configuration::tmplConfigurable<double>  SearchBeamWidth; /**< Beam width for the beam search */
};

}
}
}
}

#endif // TARGOMAN_CORE_PRIVATE_SEARCHGRAPHBUILDER_CLSCARDINALITY_H
