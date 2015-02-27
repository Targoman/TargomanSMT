#ifndef TARGOMAN_COMMON_PREFIXTREE_TMPLFULLVECTORPREFIXFILETREE_HPP
#define TARGOMAN_COMMON_PREFIXTREE_TMPLFULLVECTORPREFIXFILETREE_HPP

#include <istream>
#include <ostream>
#include <fstream>
#include <map>
#include "libTargomanCommon/PrefixTree/tmplAbstractPrefixTree.hpp"
#include "libTargomanCommon/PrefixTree/tmplFullVectorPrefixTree.hpp"
#include "libTargomanCommon/PrefixTree/tmplVectorFilePrefixTree.hpp"
#include "libTargomanCommon/PrefixTree/tmplFullVectorPrefixTree.hpp"
#include "libTargomanCommon/PrefixTree/tmplFullVectorFilePrefixTree.hpp"

namespace Targoman {
namespace Common {
namespace PrefixTree {
    /**
     * @brief A tmplPrefixTreeFullVectorNode implementation that can work with file based trees
     *
     * This class supports loading trees on demand.
     *
     * @see GFullVectorFilePrefixTree
     */
    template <typename DataClass> class tmplFilePrefixTreeFullVectorNode : public tmplPrefixTreeFullVectorNode<DataClass>, public tmplFilePrefixTreeAbstractNode<unsigned, DataClass>
    {
    protected:
        typedef tmplPrefixTreeAbstractNode<unsigned, DataClass> AbstractNode_;
        typedef tmplPrefixTreeVectorNode<DataClass> VectorNode_;
        typedef tmplPrefixTreeFullVectorNode<DataClass> FullVectorNode_;
        typedef tmplFilePrefixTreeVectorNode<DataClass> VectorFileNode_;
        typedef tmplFilePrefixTreeAbstractNode<unsigned, DataClass> AbstractFileNode_;
        typedef tmplFilePrefixTreeFullVectorNode<DataClass> FullVectorFileNode_;
        typedef typename FullVectorNode_::Container_::iterator ContainerIterator_;
        typedef typename FullVectorNode_::Container_::const_iterator ContainerConstIterator_;
        typedef typename AbstractNode_::AbstractWeakIterator AbstractWeakIterator_;

        typedef unsigned Index_;
        typedef typename std::pair<Index_, std::streampos>                                      LodChildrenPair_;
        typedef typename std::pair<Index_, typename AbstractFileNode_::VirtualChild>            VirtualChildrenPair_;
        typedef typename std::vector<std::streampos*>::iterator 	    	                LodChildrenIterator_;
        typedef typename std::vector<std::streampos*>::const_iterator 		                LodChildrenConstIterator_;
        typedef typename std::vector<typename AbstractFileNode_::VirtualChild*>::iterator       VirtualChildrenIterator_;
        typedef typename std::vector<typename AbstractFileNode_::VirtualChild*>::const_iterator VirtualChildrenConstIterator_;

        typedef typename AbstractFileNode_::LodChildAbstractIterator 				LodChildAbstractIterator_;
        typedef typename AbstractFileNode_::VirtualChildAbstractIterator 			VirtualChildAbstractIterator_;

    public:
        tmplFilePrefixTreeFullVectorNode(const Index_ &index, AbstractNode_ *predecessor) :
            AbstractNode_(index, predecessor), FullVectorNode_(index, predecessor), AbstractFileNode_(index, predecessor) {}
        tmplFilePrefixTreeFullVectorNode(const Index_ &index, AbstractNode_ *predecessor, const DataClass &data) :
            AbstractNode_(index, predecessor, data), FullVectorNode_(index, predecessor, data), AbstractFileNode_(index, predecessor) {}
        ~tmplFilePrefixTreeFullVectorNode()
        {
            // child nodes are stored by writing them to the vector position that corresponds to their index
            // therefore some positions might be free and the pointer they store is zero
            LodChildrenIterator_ it_ch;
            for(it_ch = vLodChildren_.begin(); it_ch != vLodChildren_.end(); ++it_ch)
                if(*it_ch != 0)
                    delete *it_ch;
            vLodChildren_.clear();

            // discard all temporary files
            VirtualChildrenIterator_ it_v;
            for(it_v = vVirtChildren_.begin(); it_v != vVirtChildren_.end(); ++it_v)
            {
                if(*it_v != 0)
                {
                    remove((*it_v)->filename.c_str());
                    delete *it_v;
                }
            }
            vVirtChildren_.clear();
        }

        /**
         * @brief Creates a new node
         *
         * The node type will be read from the stream iStream at position spOffset. The new node will have the index \e index
         * and its parent will be \e pParent. Passing 0 for \e pParent will create a new root node. Only the node will
         * be created, this method will neither read the nodes children from the stream nor its data. However the get pointer
         * position in the input stream will be set to the index table, so calling tmplFilePrefixTreeAbstractNode::readBinary
         * on the newly created node will properly load its data.
         *
         * @note
         * This method is implemented static to allow creation of new nodes without a parent node.
         *
         * @param pParent The nodes parent node, can be 0 for the root node
         * @param index The new nodes index, can be Index() for the root node
         * @param iStream A stream that holds the nodes data, with random get pointer position
         * @param spOffset The file offset of this nodes data
         * @return A new node with uninitialized index table and data
         * @see tmplFilePrefixTreeAbstractNode::createAndLoadNode_
         * @see tmplFilePrefixTreeAbstractNode::createNode_
         */
        static AbstractFileNode_ *sCreateNode(AbstractNode_ *parent, const Index_ &index, std::istream &iStream, const std::streampos &spOffset)
        {
            // read the node type (See tmplPrefixTreeAbstractFileNode::FilePrefixTreeNodeType)
            quint8 iType;
            iStream.seekg(spOffset);
            iStream.read((char*)&iType, sizeof(quint8));

            // create the proper node type
            switch(iType)
            {
                case AbstractFileNode_::NT_VectorNode:
                    return dynamic_cast<AbstractFileNode_*>(new VectorFileNode_(index, dynamic_cast<AbstractNode_*>(parent)));
                case AbstractFileNode_::NT_FullVectorNode:
                    return dynamic_cast<AbstractFileNode_*>(new FullVectorFileNode_(index, dynamic_cast<AbstractNode_*>(parent)));
                case AbstractFileNode_::NT_MapNode:
                    // not necessarey till now
                    break;
                default:
                    break;
            }

            return 0;
        }

        /**
         * @brief Follows the path given by \e nextIndex
         *
         * There are two lists of successor nodes that can be indexed by \e nextIndex. One is the index to pointer map
         * defined as tmplPrefixTreeMapNode::successors_, the other is the index to file offset map defined as
         * tmplFilePrefixTreeFullVectorNode::vLodChildren_. If the index is not found in the first map this method will search
         * for it in the second map. In case it is listed there it will be loaded into the first map. Otherwise this method
         * returns 0.
         *
         * @param nextIndex The next index on the path, it will identify the child node to be returned by this method
         * @return The child node identified by \e nextIndex or 0 if there is no such node
         * @see tmplFilePrefixTreeAbstractNode::createAndLoadNode_
         */
        AbstractNode_ *follow(const Index_ &nextIndex) const {
            AbstractNode_ *returnPointer;
            if (FullVectorNode_::successors_.size() <= nextIndex)
                returnPointer = 0;
            else
                returnPointer = FullVectorNode_::successors_[nextIndex];

            // if no successor is found try to load it from the file
            if(returnPointer == 0)
                returnPointer = const_cast<FullVectorFileNode_*>(this)->loadChild_(nextIndex);

            // failed to load it on demand, so look for it in the list of virtual children
            if(returnPointer == 0)
                returnPointer = const_cast<FullVectorFileNode_*>(this)->loadVirtualChild_(nextIndex);

            return returnPointer;
        }

        AbstractNode_ *followOrExpand(const Index_ &nextIndex) {
            if (FullVectorNode_::successors_.size() <= nextIndex)
                FullVectorNode_::successors_.resize(nextIndex+1, 0);

            typename FullVectorNode_::ContainerElementReference_ reference = FullVectorNode_::successors_[nextIndex];
            if (!reference)
            {
                // try to load on demand
                reference = dynamic_cast<VectorFileNode_*>(this->loadChild_(nextIndex));
                if(!reference) // if failed try to load a virtual node
                    reference = dynamic_cast<VectorFileNode_*>(this->loadVirtualChild_(nextIndex));
                if(!reference) // if this failed again try to create a new node
                    reference = new tmplFilePrefixTreeVectorNode<DataClass>(nextIndex, this); // Note: this is NOT a fullVectorNode
            }
            return reference;
        }

        AbstractNode_ *followOrExpand(const Index_ &nextIndex, const DataClass &standardValue) {
            if (FullVectorNode_::successors_.size() <= nextIndex)
                FullVectorNode_::successors_.resize(nextIndex+1, 0);

            typename FullVectorNode_::ContainerElementReference_ reference = FullVectorNode_::successors_[nextIndex];
            if (!reference)
            {
                // try to load on demand
                reference = dynamic_cast<VectorFileNode_*>(this->loadChild_(nextIndex));
                if(!reference) // if failed try to load a virtual node
                    reference = dynamic_cast<VectorFileNode_*>(this->loadVirtualChild_(nextIndex));
                if(!reference) // if this failed again try to create a new node
                    reference = new tmplFilePrefixTreeVectorNode<DataClass>(nextIndex, this, standardValue); // Note: this is NOT a fullVectorNode
            }
            return reference;
        }

        /**
         * @brief Returns true if this node is a leaf node
         *
         * This method will return true if there are no successors. That means that both the successor list as well as the
         * list of loadable successors in the file are empty.
         */
        bool isLeaf() const
        {
            return vLodChildren_.empty() && FullVectorNode_::successors_.empty();
        }

        void compact()
        {
            FullVectorNode_::compact();
           tmplShrinkToFitVector(vLodChildren_);
        }

        /**
         * @brief Initializes this node by reading its contents from the stream
         *
         * The Data class used in the template need to implement
         * \code
         * void readBinary(std::istream &iStream);
         * \endcode
         */
        void readBinary(std::istream &iStream)
        {
            // Call the base class implemention to make this node remember the stream for further use
            AbstractFileNode_::readBinary(iStream);

            // read the number of children
            quint32 iChildren;
            iStream.read((char*)&iChildren, sizeof(quint32));

            // now read the children, but do not store them in the usual successors list
            for(quint32 i = 0; i < iChildren; ++i)
            {
                quint64 index;
                quint64 iOffsetNode;

                // read the index and file offset
                iStream.read((char*)&index, sizeof(quint64));
                iStream.read((char*)&iOffsetNode, sizeof(quint64));

                // and store them
                insertLod((Index_)index, std::streampos(iOffsetNode));
            }

            // and try to load the data
            quint32 dataLength;
            iStream.read((char*)&dataLength, sizeof(quint32));	// not used here - for now
            AbstractNode_::data_.readBinary(iStream);
        }

        void writeIndex(const Index_ &index, std::ostream &oStream) const
        {
            quint64 index_quint64 = (quint64)(unsigned)index;
            oStream.write((char*)&index_quint64, sizeof(quint64));
        }

        void readIndex(Index_ &index, std::istream &iStream) const
        {
            quint64 index_quint64;
            iStream.read((char*)&index_quint64, sizeof(quint64));
            index = index_quint64;
        }

        /**
         * @brief This method returns the node type
         *
         * See tmplFilePrefixTreeAbstractNode::FilePrefixTreeNodeType for further reference.
         */
        unsigned char getType()
        {
            return AbstractFileNode_::NT_FullVectorNode;
        }

#if 0
        size_t getSize()
        {
            // size of myself and the data size
            size_t mySize = sizeof(*this) + AbstractNode_::getData().getSize();

            // size of the structure holding the special child nodes
            mySize += vLodChildren_.size() * (sizeof(std::streampos*) + sizeof(std::streampos) + AbstractFileNode_::iMallocOverhead);
            mySize += vVirtChildren_.size() * (sizeof(typename AbstractFileNode_::VirtualChild*) + sizeof(typename AbstractFileNode_::VirtualChild) + AbstractFileNode_::iMallocOverhead);
            // add the memory occupied by the strings
            VirtualChildAbstractIterator_ *it_v = beginVirtual(), *end_v = endVirtual();
            for(; !it_v->equals(end_v); it_v->increase())
                mySize += it_v->dereference().second.filename.size() + AbstractFileNode_::iMallocOverhead;	// is the string influenced by the malloc overhead?
            delete it_v;
            delete end_v;

            // finally add the child nodes
            AbstractWeakIterator_ *it = FullVectorNode_::weakBegin(), *end = FullVectorNode_::weakEnd();
            for(; !it->equals(end); it->increase())
                mySize += dynamic_cast<AbstractFileNode_*>(it->dereference())->getSize() + AbstractFileNode_::iMallocOverhead;
            delete it;
            delete end;

            return mySize;
        }
#endif

        /**
         * @name Iterator classes and methods
         * These classes and methods abstract the way the children are stored, since this depends on the actual
         * implementation while the more general methods of tmplFilePrefixTreeAbstractNode should not.
         */
        //@{
        /**
         * @brief Implementation of the virtual child iterator for the vector node class
         *
         * The constructors \e begin iterator is needed by the dereference member method. It has to return an index, even
         * though no index is stored. However it can easily be calculated from the difference of the current iterator
         * and the begin iterator.
         */
        class FullVectorVirtualChildIterator : public VirtualChildAbstractIterator_
        {
        public:
            FullVectorVirtualChildIterator(const VirtualChildrenIterator_ &begin, const VirtualChildrenIterator_ &end) : begin(begin), end(end) {}
            FullVectorVirtualChildIterator(const VirtualChildrenIterator_ &begin, const VirtualChildrenIterator_ &end, const VirtualChildAbstractIterator_ &other) :
                begin(begin), end(end), iter(other.iter) {}
            FullVectorVirtualChildIterator(const VirtualChildrenIterator_ &begin, const VirtualChildrenIterator_ &end, const VirtualChildrenIterator_ &iter) :
                begin(begin), end(end), iter(iter) {}
            ~FullVectorVirtualChildIterator() {}

            VirtualChildAbstractIterator_ *increase()
            {
                // increase to the next valid element. The user must not notice that this is a "full vector" node
                do{
                    ++iter;
                }while(*iter == 0 && iter != end); // do not move beyond the end iterator
                return this;
            }

            VirtualChildAbstractIterator_ *decrease()
            {
                // decrease to the next valid element. The user must not notice that this is a "full vector" node
                do{
                    --iter;
                }while(*iter == 0 && iter != begin); // do not move in front of the first iterator
                return this;
            }

            VirtualChildrenPair_ dereference() const
            {
                return VirtualChildrenPair_((Index_)(iter - begin), **iter);
            }

            bool equals(VirtualChildAbstractIterator_ *other) const
            {
                return iter == ((FullVectorVirtualChildIterator*)other)->iter;
            }

            VirtualChildAbstractIterator_ *operator=(const VirtualChildAbstractIterator_ &other)
            {
                iter = ((FullVectorVirtualChildIterator*)&other)->iter;
                return this;
            }

        protected:
            VirtualChildrenIterator_ begin;		///< When dereferencing this iterator the begin iterator is needed to calculate the index. Also it limits decrease.
            VirtualChildrenIterator_ end;		///< Used to limit increase and to prevent increasing beyond the end of the vector
            VirtualChildrenIterator_ iter;		///< This iterator is wrapping an iterator
        };

        VirtualChildAbstractIterator_ *beginVirtual()
        {
            // the user must not notice that this full vector node is holding empty child node entries
            // so we do not simply return the first element - which might be a null-pointer - but we increase it until
            // we find the first non empty element
            VirtualChildrenIterator_ f = vVirtChildren_.begin();
            while(f != vVirtChildren_.end() && *f == 0)
                ++f;
            return new FullVectorVirtualChildIterator(vVirtChildren_.begin(), vVirtChildren_.end(), f);
        }

        VirtualChildAbstractIterator_ *endVirtual()
        {
            return new FullVectorVirtualChildIterator(vVirtChildren_.begin(), vVirtChildren_.end(), vVirtChildren_.end());
        }

        VirtualChildAbstractIterator_ *findVirtual(const Index_ &key)
        {
            // test if key is small enough to be in the vector
            if(key >= vVirtChildren_.size())
                return new FullVectorVirtualChildIterator(vVirtChildren_.begin(), vVirtChildren_.end(), vVirtChildren_.end());

            // test if key is pointing to an element that is holding a valid value
            VirtualChildrenIterator_ f = vVirtChildren_.begin() + key;
            if(*f == 0)
                return new FullVectorVirtualChildIterator(vVirtChildren_.begin(), vVirtChildren_.end(), vVirtChildren_.end());

            return new FullVectorVirtualChildIterator(vVirtChildren_.begin(), vVirtChildren_.end(), f);
        }

        bool insertVirtual(const Index_ &key, const typename AbstractFileNode_::VirtualChild &virtChild)
        {
            // increase the size
            if(key >= vVirtChildren_.size())
                vVirtChildren_.resize(key + 1, 0);

            // test if there is already an element using this key
            if(vVirtChildren_[key] != 0)
                return false;	// no insertion made

            vVirtChildren_[key] = new typename AbstractFileNode_::VirtualChild(virtChild);
            return true;
        }

        void eraseVirtual(const Index_ &key)
        {
            if(vVirtChildren_[key] != 0)
            {
                delete vVirtChildren_[key];
                vVirtChildren_[key] = 0;
            }
        }

        void clearVirtual()
        {
            VirtualChildrenIterator_ it;
            for(it = vVirtChildren_.begin(); it != vVirtChildren_.end(); ++it)
            {
                if(*it != 0)
                {
                    remove((*it)->filename.c_str());
                    delete *it;
                }
            }
            vVirtChildren_.clear();
        }

        bool emptyVirtual()
        {
            return vVirtChildren_.empty();
        }

        /**
         * @brief Implementation of the load on demand child iterator for the vector node class
         *
         * The constructors \e begin iterator is needed by the dereference member method. It has to return an index, even
         * though no index is stored. However it can easily be calculated from the difference of the current iterator
         * and the begin iterator.
         */
        class FullVectorLodChildIterator : public LodChildAbstractIterator_
        {
        public:
            FullVectorLodChildIterator(const LodChildrenIterator_ &begin) : begin(begin) {}
            FullVectorLodChildIterator(const LodChildrenIterator_ &begin, const LodChildAbstractIterator_ &other) : begin(begin), iter(other.iter) {}
            FullVectorLodChildIterator(const LodChildrenIterator_ &begin, const LodChildrenIterator_ &iter) : begin(begin), iter(iter) {}
            ~FullVectorLodChildIterator() {}

            LodChildAbstractIterator_ *increase()
            {
                // increase to the next valid element. The user must not notice that this is a "full vector" node
                do{
                    ++iter;
                }while(*iter == 0);
                return this;
            }

            LodChildAbstractIterator_ *decrease()
            {
                // decrease to the next valid element. The user must not notice that this is a "full vector" node
                do{
                    --iter;
                }while(*iter == 0);
                return this;
            }

            LodChildrenPair_ dereference() const
            {
                return LodChildrenPair_((Index_)(iter - begin), **iter);
            }

            bool equals(LodChildAbstractIterator_ *other) const
            {
                return iter == ((FullVectorLodChildIterator*)other)->iter;
            }

            LodChildAbstractIterator_ *operator=(const LodChildAbstractIterator_ &other)
            {
                iter = ((FullVectorLodChildIterator*)&other)->iter;
                return this;
            }

        protected:
            LodChildrenIterator_ begin;	///< When dereferencing this iterator the begin iterator is needed to calculate the index
            LodChildrenIterator_ iter;	///< This iterator is wrapping an iterator
        };

        LodChildAbstractIterator_ *beginLod()
        {
            return new FullVectorLodChildIterator(vLodChildren_.begin(), vLodChildren_.begin());
        }

        LodChildAbstractIterator_ *endLod()
        {
            return new FullVectorLodChildIterator(vLodChildren_.begin(), vLodChildren_.end());
        }

        LodChildAbstractIterator_ *findLod(const Index_ &key)
        {
            // test if key is small enough to be in the vector
            if(key >= vLodChildren_.size())
                return new FullVectorLodChildIterator(vLodChildren_.begin(), vLodChildren_.end());

            // test if key is pointing to an element that is holding a valid value
            LodChildrenIterator_ f = vLodChildren_.begin() + key;
            if(*f == 0)
                return new FullVectorLodChildIterator(vLodChildren_.begin(), vLodChildren_.end());

            return new FullVectorLodChildIterator(vLodChildren_.begin(), f);
        }

        bool insertLod(const Index_ &key, const std::streampos &position)
        {
            // increase the size
            if(key >= vLodChildren_.size())
                vLodChildren_.resize(key + 1, 0);

            // test if there is already an element using this key
            if(vLodChildren_[key] != 0)
                return false;	// no insertion made

            vLodChildren_[key] = new std::streampos(position);
            return true;
        }

        void eraseLod(const Index_ &key)
        {
            if(vLodChildren_[key] != 0)
            {
                delete vLodChildren_[key];
                vLodChildren_[key] = 0;
            }
        }

        void clearLod()
        {
            LodChildrenIterator_ it;
            for(it = vLodChildren_.begin(); it != vLodChildren_.end(); ++it)
                if(*it != 0)
                    delete *it;
            vLodChildren_.clear();
        }

        bool emptyLod()
        {
            return vLodChildren_.empty();
        }
        //@}

    protected:
        AbstractFileNode_ *createNode_(AbstractNode_ *pParent, const Index_ &index, std::istream &iStream, const std::streampos &spOffset) const
        {
            return sCreateNode(pParent, index, iStream, spOffset);
        }

    protected:
        std::vector<std::streampos*> vLodChildren_; ///< A vector holding the load on demand child nodes
        std::vector<typename AbstractFileNode_::VirtualChild*> vVirtChildren_; ///< A vector holding the virtual children
    };

    /**
     * @brief Like GFullVectorPrefixTree but supports working on file based trees
     */
    template <class DataClass> class tmplFullVectorFilePrefixTree :
        public tmplAbstractFilePrefixTree<tmplFilePrefixTreeFullVectorNode<DataClass> >
    {
    public:
        typedef tmplAbstractFilePrefixTree< tmplFilePrefixTreeFullVectorNode<DataClass> > AbstractFileTree_;

    public:
        tmplFullVectorFilePrefixTree() : AbstractFileTree_() {};
        tmplFullVectorFilePrefixTree(const char *strFilename) : AbstractFileTree_(strFilename) {};
        tmplFullVectorFilePrefixTree(std::istream &inStream) : AbstractFileTree_(inStream) {};
    };
}
}
}

/**
 * @var Translation::tmplFilePrefixTreeFullVectorNode::vLodChildren_
 * @brief Holds the nodes load on demand children
 *
 * The prefix tree can be bound to a file. It will then load the nodes child nodes once follow demands them. Of course
 * these child nodes are not in memory, so they cannot be hold in the normal successors vector. Instead they are stored
 * in this vector. The index of a vector element refers to the nodes index (which is unsigned) and the vector element
 * itself is holding the file offset.
 *
 * The full vector holds all elements up to the largest seen key so far, even though this will introduce some overhead
 * due to empty elements in the vector. Just as done in the parent class this vector is holding pointers to the actual
 * elements, marking invalid entries by using a zero pointer.
 */

/**
 * @var Translation::tmplFilePrefixTreeFullVectorNode::vVirtChildren_
 * @brief Holds the virtual child nodes, nodes that have been moved to a file
 *
 * There are 3 types of nodes. Nodes that are in memory are hold in the parent classes successor vector. Nodes that can
 * be load on demand, as the parents follow method requests this node, are stored in vLodChildren_. There is a third type
 * of nodes: Nodes that have been moved from memory to a file. While the first type of node is used both when creating
 * and writing a tree, as well as when reading from a file and using it, the second type, the load on demand children,
 * are only used when the tree is being loaded. The virtual children are the nodes that are used when the tree is being
 * created. If you run out of memory you can just move some nodes to the tree file and continue the trees creation.
 * These nodes can not be stored with the other successor nodes, since they are not in memory and can no longer be accessed.
 *
 * The full vector holds all elements up to the largest seen key so far, even though this will introduce some overhead
 * due to empty elements in the vector. Just as done in the parent class this vector is holding pointers to the actual
 * elements, marking invalid entries by using a zero pointer.
 */

#endif
