#ifndef TARGOMAN_COMMON_PREFIXTREE_TMPLABSTRACTPREFIXTREE_HPP
#define TARGOMAN_COMMON_PREFIXTREE_TMPLABSTRACTPREFIXTREE_HPP

#include <algorithm>
#include <map>
#include <vector>

namespace Targoman {
namespace Common {
namespace PrefixTree {
    //! Abstract node class, from which all the other node classes will be descendant from
    /**
     * The key point is that several node classes, each with a different
     * storage class, will be able to coexist in a prefix tree. This allows for
     * example to have a "fullSize"-vector node as root of the prefix tree,
     * while having vector- or map-nodes for the next levels.
     *
     * The key functions to be redefined are follow() and followAndExpand().
     * The first one is a const function and follows the tree down when the
     * nodes exist. followOrExpand() does the same, but creating nodes as it
     * progresses.
     */
    template <typename IndexClass, typename DataClass> class tmplPrefixTreeAbstractNode {
    public:
        typedef IndexClass Index;
        typedef DataClass Data;
        typedef tmplPrefixTreeAbstractNode<Index, Data> AbstractNode;

        //! \todo Allow for standard element?
        tmplPrefixTreeAbstractNode(const Index &index, tmplPrefixTreeAbstractNode *predecessor) : predecessor_(predecessor), myIndex_(index), data_() { }
        tmplPrefixTreeAbstractNode(const Index &index, tmplPrefixTreeAbstractNode *predecessor, const Data &data) : predecessor_(predecessor), myIndex_(index), data_(data) { }
        virtual ~tmplPrefixTreeAbstractNode() {}

        Data &setData(const Data &data) { data_ = data; return data_; }
        Data &getData() { return data_; }
        const Data &getData() const { return data_; }

        const Index &getIndex() const { return myIndex_; }

        virtual tmplPrefixTreeAbstractNode<Index, Data> *follow(const Index &nextIndex) const = 0;
        virtual tmplPrefixTreeAbstractNode<Index, Data> *followOrExpand(const Index &nextIndex) = 0;
        virtual tmplPrefixTreeAbstractNode<Index, Data> *followOrExpand(const Index &nextIndex, const Data &standardValue) = 0;

        tmplPrefixTreeAbstractNode *up() const { return predecessor_; }

        //! Returns is this node is a leaf in the tree.
        virtual bool isLeaf() const = 0;

        //! A memory optimization function
        /**
         * It depends of the derived class (and the container within) if it
         * makes sense to do a memory optimization. The typical operation is a
         * shrink-to-fit of a vector.
         */
        virtual void compact() {};

        //! Iterator stuff
        /**
         * The goal is to have an iterator class in the tree class, which
         * traverses all nodes. For each node we will store the point in the
         * container where we are. As the containers are different for each
         * kind of node, what we will store will be an instance of this class,
         * which is a wrapper around the actual container iterator in each
         * node. However, what we will store will be pointers to this class
         * (which is completely abstract). In order to stress this point we do
         * NOT define the standard iterator operations. Specially, only
         * functions which alter this class state are provided.
         */
        class AbstractWeakIterator {
        public:
            AbstractWeakIterator () {}
            virtual ~AbstractWeakIterator() {}

            virtual AbstractWeakIterator *increase() = 0;
            virtual AbstractWeakIterator *decrease() = 0;

            virtual tmplPrefixTreeAbstractNode *dereference() const = 0;

            virtual bool equals(const AbstractWeakIterator *otherWeakIterator) const = 0;

            //! Return a new copy of the iterator
            virtual AbstractWeakIterator *getCopy() const = 0;
        };

        //! \warning Medium memory leak danger.
        virtual AbstractWeakIterator *weakBegin() = 0;
        //! \warning High memory leak danger: do not compare with node->weakEnd() without storing the pointer!!!
        virtual AbstractWeakIterator *weakEnd() = 0;

        /**
         * @brief Inserts the given node into the list of successors
         *
         * Using the index \e index the node given as \e node will be inserted into the list of successors. This
         * method returns true if an insertion was made.
         *
         * @param index The new nodes index
         * @param node The node itself
         * @return True if an insertion was made, false otherwise
         */
        virtual bool insert(const Index &index, AbstractNode *node) = 0;

        /**
         * @brief Removes the node with the given index from the list of successors
         *
         * Just like the STL erase this one does not delete the node that is erased. It is the callers reponsibility to
         * free any memory allocated by the node that is being erased here.
         *
         * @param index The index of the node that is to be removed
         */
        virtual void erase(const Index &index) = 0;

        /**
         * @brief Delete all successor nodes
         *
         * This method will remove all successor nodes from memory. It will not modify the list of index and file offsets
         * of loadable successor nodes though. For a complete documentation see GAbstractPrefixTree::clear.
         *
         * @see see GAbstractPrefixTree::clear
         */
        virtual void clear() = 0;

    protected:
        tmplPrefixTreeAbstractNode *predecessor_;
        Index myIndex_;
        Data data_;
    };

    //! Alternative implementation of a prefix tree, more general and probably cleaner that the original, but incompatible with it.
    /**
     * The interface is perhaps a bit harsh in the sense that we give node
     * pointers directly to the user.
     *
     * It is parameterized with the RootNode class, from which it takes the Data
     * and Index classes when needed. Note that the RootNodeClass can be
     * further template-parameterized, typical example: root node is a
     * FullVectorNode, the other nodes are VectorNodes.
     *
     * Since C++ does not (yet?) support template typedefs, we will define dumb
     * inherited classes from this abstract. This can cause a small overhead
     * (and Thomas says we shouldn't do it). Probably is more a religious
     * matter, therefore in every derived class the typedef equivalent is
     * shown.
     */
    template <class RootNodeClass> class tmplAbstractPrefixTree {
    public:
        typedef typename RootNodeClass::AbstractNode Node;

        tmplAbstractPrefixTree() : rootNode_(new RootNodeClass(typename Node::Index(), 0)) {}
        virtual ~tmplAbstractPrefixTree() { delete rootNode_; }

        Node *rootNode() { return rootNode_; }
        const Node *rootNode() const { return rootNode_; }

        Node *getOrCreateNode(const std::vector<typename Node::Index> &indexPath) {
            Node *currentNode = rootNode();
            for (typename std::vector<typename Node::Index>::const_iterator i = indexPath.begin(); i != indexPath.end(); ++i)
                currentNode = currentNode->followOrExpand(*i);
            return currentNode;
        }

        Node *getOrCreateNode(const std::vector<typename Node::Index> &indexPath, const typename Node::Data &standardElement) {
            Node *currentNode = rootNode();
            for (typename std::vector<typename Node::Index>::const_iterator i = indexPath.begin(); i != indexPath.end(); ++i)
                currentNode = currentNode->followOrExpand(*i, standardElement);
            return currentNode;
        }
        //! Carry out a memory optimization of the tree
        /**
         * It depends of the actual implementation of the nodes if this really does something or not.
         */
        void compact() {
            rootNode_->compact();
        }

        class iterator {
        private:
            typedef typename Node::AbstractWeakIterator WeakIterator_;
        public:
            //! Initalize an end iterator
            iterator() : currentNode_(0), weakIteratorPath_(), sentinels_() {}

            //! Initialize the iterator starting at a node.
            iterator(Node *n) : currentNode_(n), weakIteratorPath_(), sentinels_() {}

            //! Copy constructor
            iterator(const iterator &otherIterator) : weakIteratorPath_(), sentinels_() {
                copyFromIterator_(otherIterator);
            }

            ~iterator() {
                clear_();
            }

            iterator &operator=(const iterator &otherIterator) {
                currentNode_ =  otherIterator.currentNode_;
                clear_();
                copyFromIterator_(otherIterator);
                return *this;
            }

            iterator &operator++() {
                increase_();
                return *this;
            }

            iterator operator++(int) {
                iterator returnIterator(*this);
                increase_();
                return returnIterator;
            }

            bool operator==(const iterator &otherIterator) const {
                return currentNode_ == otherIterator.currentNode_;
            }

            bool operator!=(const iterator &otherIterator) const {
                return !operator==(otherIterator);
            }

            Node *operator->() {
                return currentNode_;
            }

            Node *operator*() {
                return currentNode_;
            }

            std::vector<typename Node::Index> getIndexPath() const {
                std::vector<typename Node::Index> reversePath;
                for (Node *n = currentNode_; n->up(); n = n->up())
                    reversePath.push_back(n->getIndex());
                std::reverse(reversePath.begin(), reversePath.end());
                return reversePath;
            }

        private:
            Node *currentNode_;
            std::vector<WeakIterator_ *> weakIteratorPath_;
            std::vector<WeakIterator_ *> sentinels_;

            //! weakIteratorPath_ should be empty
            void copyFromIterator_(const iterator &otherIterator) {
                currentNode_ = otherIterator.currentNode_;
                for (typename std::vector<WeakIterator_ *>::const_iterator w = otherIterator.weakIteratorPath_.begin(); w != otherIterator.weakIteratorPath_.end(); ++w)
                    weakIteratorPath_.push_back((*w)->getCopy());
                for(typename std::vector<WeakIterator_ *>::const_iterator s = otherIterator.sentinels_.begin(); s != otherIterator.sentinels_.end(); ++s)
                    weakIteratorPath_.push_back((*s)->getCopy());
            }

            void clear_() {
                for (typename std::vector<WeakIterator_ *>::iterator n = weakIteratorPath_.begin(); n != weakIteratorPath_.end(); ++n)
                    delete *n;
                for (typename std::vector<WeakIterator_ *>::iterator s = sentinels_.begin(); s != sentinels_.end(); ++s)
                    delete *s;
            }

            void increase_() {
                WeakIterator_ *nextIterator = currentNode_->weakBegin();
                WeakIterator_ *endIterator = currentNode_->weakEnd();
                if (!nextIterator->equals(endIterator)) {
                    weakIteratorPath_.push_back(nextIterator);
                    sentinels_.push_back(endIterator);
                    currentNode_ = nextIterator->dereference();
                } else {
                    delete nextIterator;
                    delete endIterator;
                    while (!weakIteratorPath_.empty() && (weakIteratorPath_.back()->increase()->equals(sentinels_.back()))) {
                        delete weakIteratorPath_.back();
                        delete sentinels_.back();
                        weakIteratorPath_.pop_back();
                        sentinels_.pop_back();
                    }
                    if (weakIteratorPath_.empty()) // We are at the end
                        currentNode_ = 0;
                    else
                        currentNode_ = weakIteratorPath_.back()->dereference();
                }
            }
        };

        iterator begin() { return iterator(rootNode_); }
        iterator end() { return iterator(); }

        /**
         * @brief Removes all children of the root node from memory
         *
         * This method will remove all nodes from memory (except for the root node). Use this method to clear the tree
         * when loading trees on demand. This way the memory usage can be kept small.
         *
         * Prefix trees are always equipped with a root node. Therefore this method will merely make it delete all its child
         * nodes. When using the file based prefix trees this also makes sure that the root nodes children index and file
         * offset table does not have to be reread.
         */
        void clear() { rootNode_->clear(); }

    protected:
        Node *rootNode_;
    };
}
}
}
#endif
