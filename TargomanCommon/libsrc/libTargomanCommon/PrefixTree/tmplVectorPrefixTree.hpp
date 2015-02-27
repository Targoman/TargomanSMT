#ifndef TARGOMAN_COMMON_PREFIXTREE_TMPLVECTORPREFIXTREE_HPP
#define TARGOMAN_COMMON_PREFIXTREE_TMPLVECTORPREFIXTREE_HPP

#include "libTargomanCommon/PrefixTree/tmplAbstractPrefixTree.hpp"

namespace Targoman {
namespace Common {
namespace PrefixTree {
    //! Shrink-to-fit a vector
    template<class T> inline void tmplShrinkToFitVector(std::vector<T> &v) {
        std::vector<T>(v).swap(v);
    }

    //! Node where the successors are stored in a vector
    /**
     * The index for this type of nodes is fixed to be an unsigned. If more
     * general capabilities are needed, use the MapNodes.
     *
     * The vector for storing the data is kept sorted. Insertion can therefore
     * be time consuming.
     *
     * \todo This should also work with a more general comparison operator. But
     * to do it rightly we should create a static comparison object, and I am
     * not in the mood of fighting templates about it right now.
     */
    template <class DataClass> class tmplPrefixTreeVectorNode : virtual public tmplPrefixTreeAbstractNode<unsigned, DataClass> {
    protected:
        typedef std::vector<tmplPrefixTreeAbstractNode<unsigned, DataClass> *> Container_;
        typedef typename Container_::iterator ContainerIterator_;
        typedef typename Container_::const_iterator ContainerConstIterator_;
        typedef tmplPrefixTreeAbstractNode<unsigned, DataClass> AbstractNode_;
        typedef typename AbstractNode_::AbstractWeakIterator AbstractWeakIterator_;
    public:
        tmplPrefixTreeVectorNode(unsigned index, AbstractNode_ *predecessor) : AbstractNode_(index, predecessor) {}
        tmplPrefixTreeVectorNode(unsigned index, AbstractNode_ *predecessor, const DataClass &data) : AbstractNode_(index, predecessor, data) {}
        ~tmplPrefixTreeVectorNode() {
            for (ContainerIterator_ i = successors_.begin(); i != successors_.end(); ++i)
                delete *i;
        }

        AbstractNode_ *follow(const unsigned &nextIndex) const {
            ContainerConstIterator_ position = binarySearch_(nextIndex);
            AbstractNode_ *returnPointer;
            if (position == successors_.end() || (*position)->getIndex() != nextIndex) {
                returnPointer = 0;
            } else {
                returnPointer = *position;
            }
            return returnPointer;
        }

        AbstractNode_ *followOrExpand(const typename AbstractNode_::Index &nextIndex) {
            ContainerIterator_ position = binarySearch_(nextIndex);
            if (position == successors_.end() || (*position)->getIndex() != nextIndex) {
                size_t offset = position - successors_.begin(); // insert probably invalidates the iterator
                successors_.insert(position, new tmplPrefixTreeVectorNode(nextIndex, this));
                position = successors_.begin() + offset;
            }
            return *position;
        }

        AbstractNode_ *followOrExpand(const typename AbstractNode_::Index &nextIndex, const DataClass &standardValue) {
            ContainerIterator_ position = binarySearch_(nextIndex);
            if (position == successors_.end() || (*position)->getIndex() != nextIndex) {
                size_t offset = position - successors_.begin(); // insert probably invalidates the iterator
                successors_.insert(position, new tmplPrefixTreeVectorNode(nextIndex, this, standardValue));
                position = successors_.begin() + offset;
            }
            return *position;
        }

        /**
         * It is assumed that the vector is only initialized "on demand". This
         * is true in the current implementation but it could change!! In this
         * case we probably would have to do a traversal of the vector in order
         * to check for succesors.
         */
        virtual bool isLeaf() const {
            return successors_.empty();
        }

        virtual void compact() {
            tmplShrinkToFitVector(successors_);
        }

        class WeakIterator : public AbstractWeakIterator_ {
        public:
            WeakIterator() : AbstractWeakIterator_(), successorsIterator_() {}
            WeakIterator(const ContainerIterator_ &successorsIterator) : AbstractWeakIterator_(), successorsIterator_(successorsIterator) {}

            AbstractWeakIterator_ *increase() { ++successorsIterator_; return this; }
            AbstractWeakIterator_ *decrease() { --successorsIterator_; return this; }

            AbstractNode_ *dereference() const { return *successorsIterator_; }

            virtual bool equals(const AbstractWeakIterator_ *otherWeakIterator) const {
                const WeakIterator *castedIterator = static_cast<const WeakIterator *>(otherWeakIterator);
                return castedIterator->successorsIterator_ == successorsIterator_;
            }

            AbstractWeakIterator_ *getCopy() const {
                return new WeakIterator(successorsIterator_);
            }

        private:
            ContainerIterator_ successorsIterator_;
        };

        AbstractWeakIterator_ *weakBegin() { return new WeakIterator(successors_.begin()); }
        AbstractWeakIterator_ *weakEnd() { return new WeakIterator(successors_.end()); }

        bool insert(const unsigned &index, AbstractNode_ *node)
        {
            ContainerIterator_ f = binarySearch_(index);
            if(f != successors_.end() && (*f)->getIndex() == index)
                return false;

            successors_.insert(f, node);
            return true;
        }

        void erase(const unsigned &index)
        {
            ContainerIterator_ f = binarySearch_(index);
            if(f != successors_.end() && (*f)->getIndex() == index)
                successors_.erase(f);
        }

        void clear()
        {
            for (ContainerIterator_ i = successors_.begin(); i != successors_.end(); ++i)
                delete *i;
            successors_.clear();
        }

    protected:
        mutable Container_ successors_;

        //! A simple binary search
        /**
         * We cannot use std::lower_bound because of the types to compare.
         */
        ContainerConstIterator_ binarySearch_(const unsigned &index) const {
            ContainerConstIterator_ low = successors_.begin();
            ContainerConstIterator_ high = successors_.end();
            while (low < high) {
                ContainerConstIterator_ mid = low + (high - low)/2;
                if ((*mid)->getIndex() < index){
                    low = mid + 1;
                } else {
                    high = mid;
                }
            }
            return low;
        }

        //! The same, but without const
        /**
         * Talk about code duplication
         */
        ContainerIterator_ binarySearch_(const unsigned &index) {
            ContainerIterator_ low = successors_.begin();
            ContainerIterator_ high = successors_.end();
            while (low < high) {
                ContainerIterator_ mid = low + (high - low)/2;
                if ((*mid)->getIndex() < index){
                    low = mid + 1;
                } else {
                    high = mid;
                }
            }
            return low;
        }
    };

    //! A Prefix Tree where the container is an (ordered) vector
    /**
     * The data is stored in a (sorted) vector. Indexes are limited to
     * unsigned. Retrieval is done using binary search (i.e. it should be
     * efficient), but insertion is of course inefficient. Use it only for
     * sparse populated trees.
     *
     * Template equivalent: <tt>GAbstractPrefixTree<tmplPrefixTreeVectorNode<DataClass> ></tt>
     * \sa GAbstractPrefixTree
     */
    template <class DataClass> class tmplVectorPrefixTree : public tmplAbstractPrefixTree<tmplPrefixTreeVectorNode<DataClass> > {};
}
}
}

#endif
