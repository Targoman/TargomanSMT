#ifndef TARGOMAN_COMMON_PREFIXTREE_TMPLMAP_PREFIX_TREE_HH
#define TARGOMAN_COMMON_PREFIXTREE_TMPLMAP_PREFIX_TREE_HH

#include "libTargomanCommon/PrefixTree/tmplAbstractPrefixTree.hpp"

namespace Targoman {
namespace Common {
namespace PrefixTree {
    template <class Index, class Data, class Compare=std::less<Index> > class tmplPrefixTreeMapNode : virtual public tmplPrefixTreeAbstractNode<Index, Data> {
    protected:
        typedef std::map<Index, tmplPrefixTreeAbstractNode<Index, Data> *, Compare> Container_;
        typedef typename Container_::iterator ContainerIterator_;
        typedef typename Container_::const_iterator ContainerConstIterator_;
        typedef tmplPrefixTreeAbstractNode<Index, Data> AbstractNode_;
        typedef typename AbstractNode_::AbstractWeakIterator AbstractWeakIterator_;
    public:
        tmplPrefixTreeMapNode(const Index &index, AbstractNode_ *predecessor) : AbstractNode_(index, predecessor) {}
        tmplPrefixTreeMapNode(const Index &index, AbstractNode_ *predecessor, const Data &data) : AbstractNode_(index, predecessor, data) {}
        ~tmplPrefixTreeMapNode() {
            for (ContainerIterator_ i = successors_.begin(); i != successors_.end(); ++i)
                delete i->second;
        }

        AbstractNode_ *follow(const Index &nextIndex) const {
            ContainerConstIterator_ i = successors_.find(nextIndex);
            AbstractNode_ *returnPointer;
            if (i == successors_.end())
                returnPointer = 0;
            else
                returnPointer = i->second;
            return returnPointer;
        }

        AbstractNode_ *followOrExpand(const Index &nextIndex) {
            ContainerConstIterator_ i = this->successors_.find(nextIndex);
            AbstractNode_ *returnPointer;
            if (i != this->successors_.end())
                returnPointer = i->second;
            else {
                tmplPrefixTreeMapNode *newNode = new tmplPrefixTreeMapNode(nextIndex, this);
                this->successors_[nextIndex] = newNode;
                returnPointer =  newNode;
            }
            return returnPointer;
        }

        AbstractNode_ *followOrExpand(const Index &nextIndex, const Data &standardValue) {
            ContainerConstIterator_ i = this->successors_.find(nextIndex);
            AbstractNode_ *returnPointer;
            if (i != this->successors_.end())
                returnPointer = i->second;
            else {
                tmplPrefixTreeMapNode *newNode = new tmplPrefixTreeMapNode(nextIndex, this, standardValue);
                this->successors_[nextIndex] = newNode;
                returnPointer =  newNode;
            }
            return returnPointer;
        }

        virtual bool isLeaf() const {
            return successors_.empty();
        }

        class WeakIterator : public AbstractWeakIterator_ {
        public:
            WeakIterator() : AbstractWeakIterator_(), successorsIterator_() {}
            WeakIterator(const ContainerIterator_ &successorsIterator) : AbstractWeakIterator_(), successorsIterator_(successorsIterator) {}

            AbstractWeakIterator_ *increase() { ++successorsIterator_; return this; }
            AbstractWeakIterator_ *decrease() { --successorsIterator_; return this; }

            AbstractNode_ *dereference() const { return successorsIterator_->second; }

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

        bool insert(const Index &index, AbstractNode_ *node) { return successors_.insert(std::pair<Index, AbstractNode_*>(index, node)).second; }
        void erase(const Index &index) { successors_.erase(index); }

        void clear()
        {
            for (ContainerIterator_ i = successors_.begin(); i != successors_.end(); ++i)
                delete i->second;
            successors_.clear();
        }

    protected:
        mutable Container_ successors_;
    };

    //! A Prefix Tree where the container is a map
    /**
     * The data is stored in a map. Space and time efficiency are then those of
     * a map. The Index can be arbitrary.
     *
     * Template equivalent: <tt>GAbstractPrefixTree<tmplPrefixTreeMapNode<Index, Data, Compare> ></tt>
     * \sa GAbstractPrefixTree
     */
    template <class Index, class Data, class Compare=std::less<Index> > class tmplMapPrefixTree :
        public tmplAbstractPrefixTree<tmplPrefixTreeMapNode<Index, Data, Compare> > {};
}
}
}

#endif
