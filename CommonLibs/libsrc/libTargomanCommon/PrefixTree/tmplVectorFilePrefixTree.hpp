#ifndef TARGOMAN_COMMON_PREFIXTREE_TMPLVECTORPREFIXFILETREE_HPP
#define TARGOMAN_COMMON_PREFIXTREE_TMPLVECTORPREFIXFILETREE_HPP

#include <istream>
#include <ostream>
#include <fstream>
#include <vector>
#include "libTargomanCommon/PrefixTree/tmplAbstractPrefixTree.hpp"
#include "libTargomanCommon/PrefixTree/tmplVectorPrefixTree.hpp"
#include "libTargomanCommon/PrefixTree/tmplAbstractFilePrefixTree.hpp"

namespace Targoman {
namespace Common {
namespace PrefixTree {
    /**
     * @brief A tmplPrefixTreeVectorNode implementation that can work with file based trees
     *
     * This class supports loading trees on demand.
     *
     * @see tmplPrefixTreeVectorNode
     */
    template <class DataClass> class tmplFilePrefixTreeVectorNode : public tmplPrefixTreeVectorNode<DataClass>, public tmplFilePrefixTreeAbstractNode<unsigned, DataClass>
    {
    protected:
        typedef tmplPrefixTreeAbstractNode<unsigned, DataClass> AbstractNode_;
        typedef tmplPrefixTreeVectorNode<DataClass> VectorNode_;
        typedef tmplFilePrefixTreeAbstractNode<unsigned, DataClass> AbstractFileNode_;
        typedef tmplFilePrefixTreeVectorNode<DataClass> VectorFileNode_;
        typedef typename AbstractNode_::AbstractWeakIterator AbstractWeakIterator_;

        typedef unsigned Index_;
        typedef typename std::pair<Index_, std::streampos> 							LodChildrenPair_;
        typedef typename std::pair<Index_, typename AbstractFileNode_::VirtualChild> VirtualChildrenPair_;
        typedef typename VectorNode_::Container_::iterator 							ContainerIterator_;
        typedef typename VectorNode_::Container_::const_iterator 					ContainerConstIterator_;
        typedef typename std::vector<LodChildrenPair_>::iterator 					LodChildrenIterator_;
        typedef typename std::vector<LodChildrenPair_>::const_iterator 				LodChildrenConstIterator_;
        typedef typename std::vector<VirtualChildrenPair_>::iterator 				VirtualChildrenIterator_;
        typedef typename std::vector<VirtualChildrenPair_>::const_iterator 			VirtualChildrenConstIterator_;

        typedef typename AbstractFileNode_::LodChildAbstractIterator 				LodChildAbstractIterator_;
        typedef typename AbstractFileNode_::VirtualChildAbstractIterator 			VirtualChildAbstractIterator_;

    public:
        tmplFilePrefixTreeVectorNode(Index_ index, AbstractNode_ *predecessor) : AbstractNode_(index, predecessor), VectorNode_(index, predecessor), AbstractFileNode_(index, predecessor) {}
        tmplFilePrefixTreeVectorNode(Index_ index, AbstractNode_ *predecessor, const DataClass &data) : AbstractNode_(index, predecessor, data), VectorNode_(index, predecessor, data), AbstractFileNode_(index, predecessor) {}
        ~tmplFilePrefixTreeVectorNode()
        {
            // discard all temporary files
            VirtualChildrenIterator_ it;
            for(it = vVirtChildren_.begin(); it != vVirtChildren_.end(); ++it)
                remove(it->second.filename.c_str());
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
                //case AbstractFileNode_::NT_FullVectorNode:
                //    ret = dynamic_cast<AbstractFileNode_*>(new FullVectorFileNode_(index, dynamic_cast<AbstractNode_*>(parent)));
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
         * tmplFilePrefixTreeVectorNode::vLodChildren_. If the index is not found in the first map this method will search
         * for it in the second map. In case it is listed there it will be loaded into the first map. Otherwise this method
         * returns 0.
         *
         * @param nextIndex The next index on the path, it will identify the child node to be returned by this method
         * @return The child node identified by \e nextIndex or 0 if there is no such node
         * @see tmplFilePrefixTreeAbstractNode::createAndLoadNode_
         */
        AbstractNode_ *follow(const Index_ &nextIndex) const
        {
            ContainerConstIterator_ position = VectorNode_::binarySearch_(nextIndex);
            AbstractNode_ *returnPointer;
            if (position == VectorNode_::successors_.end() || (*position)->getIndex() != nextIndex)
            {
                // obviously the node has not been loaded into memory yet, so try loading it on demand
                returnPointer = const_cast<VectorFileNode_*>(this)->loadChild_(nextIndex);
                // failed to load it on demand, so look for it in the list of virtual children
                if(returnPointer == 0)
                    returnPointer = const_cast<VectorFileNode_*>(this)->loadVirtualChild_(nextIndex);
            }else{
                returnPointer = *position;
            }
            return returnPointer;
        }

        AbstractNode_ *followOrExpand(const Index_ &nextIndex) {
            ContainerIterator_ position = VectorNode_::binarySearch_(nextIndex);
            if (position == VectorNode_::successors_.end() || (*position)->getIndex() != nextIndex) {
                // try to load the missing successor from the file first
                AbstractNode_ *returnPointer = this->loadChild_(nextIndex);
                if(returnPointer != 0)
                    return returnPointer;
                // failed to load it on demand, so look for it in the list of virtual children
                returnPointer = this->loadVirtualChild_(nextIndex);
                if(returnPointer != 0)
                    return returnPointer;

                size_t offset = position - VectorNode_::successors_.begin(); // insert probably invalidates the iterator
                tmplFilePrefixTreeVectorNode *newNode = new tmplFilePrefixTreeVectorNode(nextIndex, this);
                VectorNode_::successors_.insert(position, newNode);
                position = VectorNode_::successors_.begin() + offset;
            }
            return *position;
        }

        AbstractNode_ *followOrExpand(const Index_ &nextIndex, const DataClass &standardValue) {
            ContainerIterator_ position = VectorNode_::binarySearch_(nextIndex);
            if (position == VectorNode_::successors_.end() || (*position)->getIndex() != nextIndex) {
                // try to load the missing successor from the file first
                AbstractNode_ *returnPointer = this->loadChild_(nextIndex);
                if(returnPointer != 0)
                    return returnPointer;
                // failed to load it on demand, so look for it in the list of virtual children
                returnPointer = this->loadVirtualChild_(nextIndex);
                if(returnPointer != 0)
                    return returnPointer;

                size_t offset = position - VectorNode_::successors_.begin(); // insert probably invalidates the iterator
                tmplFilePrefixTreeVectorNode *newNode = new tmplFilePrefixTreeVectorNode(nextIndex, this, standardValue);
                VectorNode_::successors_.insert(position, newNode);
                position = VectorNode_::successors_.begin() + offset;
            }
            return *position;
        }

        /**
         * @brief Returns true if this node is a leaf node
         *
         * This method will return true if there are no successors. That means that both the successor list as well as the
         * list of loadable successors in the file are empty.
         */
        bool isLeaf() const
        {
            return vLodChildren_.empty() && VectorNode_::successors_.empty();
        }

        void compact()
        {
            VectorNode_::compact();
            tmplShrinkToFitVector(vLodChildren_);
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
         * @brief Initializes this node by reading its contents from the stream
         *
         * The Data class used in the template needs to implement
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
                insertLod(index, std::streampos(iOffsetNode));
            }

            // and try to load the data
            quint32 dataLength;
            iStream.read((char*)&dataLength, sizeof(quint32));	// not used here - for now
            AbstractNode_::data_.readBinary(iStream);
        }

        /**
         * @brief This method returns the node type
         *
         * See tmplFilePrefixTreeAbstractNode::FilePrefixTreeNodeType for further reference.
         */
        unsigned char getType()
        {
            return AbstractFileNode_::NT_VectorNode;
        }

#if 0
        size_t getSize()
        {
            // size of myself and the data size
            size_t mySize = sizeof(*this) + AbstractNode_::getData().getSize();

            // size of the structure holding the special child nodes
            mySize += vLodChildren_.size() * sizeof(LodChildrenPair_);
            mySize += vVirtChildren_.size() * sizeof(VirtualChildrenPair_);
            // add the memory occupied by the strings
            VirtualChildAbstractIterator_ *it_v = beginVirtual(), *end_v = endVirtual();
            for(; !it_v->equals(end_v); it_v->increase())
                mySize += it_v->dereference().second.filename.size() + AbstractFileNode_::iMallocOverhead;	// is the string influenced by the malloc overhead?
            delete it_v;
            delete end_v;

            // finally add the child nodes
            AbstractWeakIterator_ *it = VectorNode_::weakBegin(), *end = VectorNode_::weakEnd();
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
         */
        class VectorVirtualChildIterator : public VirtualChildAbstractIterator_
        {
            public:
                VectorVirtualChildIterator() {}
                VectorVirtualChildIterator(const VirtualChildAbstractIterator_ &other) : iter(other.iter) {}
                VectorVirtualChildIterator(const VirtualChildrenIterator_ &iter) : iter(iter) {}
                ~VectorVirtualChildIterator() {}

                VirtualChildAbstractIterator_ *increase()
                {
                    ++iter;
                    return this;
                }

                VirtualChildAbstractIterator_ *decrease()
                {
                    --iter;
                    return this;
                }

                VirtualChildrenPair_ dereference() const
                {
                    return *iter;
                }

                bool equals(VirtualChildAbstractIterator_ *other) const
                {
                    return iter == ((VectorVirtualChildIterator*)other)->iter;
                }

                VirtualChildAbstractIterator_ *operator=(const VirtualChildAbstractIterator_ &other)
                {
                    iter = ((VectorVirtualChildIterator*)&other)->iter;
                    return this;
                }

            protected:
                VirtualChildrenIterator_ iter;		///< This iterator is wrapping an iterator
        };

        VirtualChildAbstractIterator_ *beginVirtual()
        {
            return new VectorVirtualChildIterator(vVirtChildren_.begin());
        }

        VirtualChildAbstractIterator_ *endVirtual()
        {
            return new VectorVirtualChildIterator(vVirtChildren_.end());
        }

        VirtualChildAbstractIterator_ *findVirtual(const Index_ &key)
        {
            VirtualChildrenIterator_ f = binaryVirtualChildSearch_(key);

            if(f != vVirtChildren_.end() && f->first == key)	// f must neither be the end iterator nor pointing to "the wrong" key
                return new VectorVirtualChildIterator(f);
            else
                return new VectorVirtualChildIterator(vVirtChildren_.end());
        }

        bool insertVirtual(const Index_ &key, const typename AbstractFileNode_::VirtualChild &virtChild)
        {
            VirtualChildrenIterator_ f = binaryVirtualChildSearch_(key);

            if(f != vVirtChildren_.end() && f->first == key)		// f might point an existing node
                return false;	// there is already a node with that key -> no insertion done

            vVirtChildren_.insert(f, VirtualChildrenPair_(key, virtChild));
            return true;
        }

        void eraseVirtual(const Index_ &key)
        {
            VirtualChildrenIterator_ f = binaryVirtualChildSearch_(key);
            if(f != vVirtChildren_.end() && f->first == key)		// test if f is actually pointing to the right node
                vVirtChildren_.erase(f);
        }

        void clearVirtual()
        {
            VirtualChildrenIterator_ it;
            for(it = vVirtChildren_.begin(); it != vVirtChildren_.end(); ++it)
                remove(it->second.filename.c_str());
            vVirtChildren_.clear();
        }

        bool emptyVirtual()
        {
            return vVirtChildren_.empty();
        }

        /**
         * @brief Implementation of the load on demand child iterator for the vector node class
         */
        class VectorLodChildIterator : public LodChildAbstractIterator_
        {
            public:
                VectorLodChildIterator() {}
                VectorLodChildIterator(const LodChildAbstractIterator_ &other) : iter(other.iter) {}
                VectorLodChildIterator(const LodChildrenIterator_ &iter) : iter(iter) {}
                ~VectorLodChildIterator() {}

                LodChildAbstractIterator_ *increase()
                {
                    ++iter;
                    return this;
                }

                LodChildAbstractIterator_ *decrease()
                {
                    --iter;
                    return this;
                }

                LodChildrenPair_ dereference() const
                {
                    return *iter;
                }

                bool equals(LodChildAbstractIterator_ *other) const
                {
                    return iter == ((VectorLodChildIterator*)other)->iter;
                }

                LodChildAbstractIterator_ *operator=(const LodChildAbstractIterator_ &other)
                {
                    iter = ((VectorLodChildIterator*)&other)->iter;
                    return this;
                }

            protected:
                LodChildrenIterator_ iter;		///< This iterator is wrapping an iterator
        };

        LodChildAbstractIterator_ *beginLod()
        {
            return new VectorLodChildIterator(vLodChildren_.begin());
        }

        LodChildAbstractIterator_ *endLod()
        {
            return new VectorLodChildIterator(vLodChildren_.end());
        }

        LodChildAbstractIterator_ *findLod(const Index_ &key)
        {
            LodChildrenIterator_ f = binaryChildSearch_(key);

            if(f != vLodChildren_.end() && f->first == key)
                return new VectorLodChildIterator(f);
            else
                return new VectorLodChildIterator(vLodChildren_.end());
        }

        bool insertLod(const Index_ &key, const std::streampos &position)
        {
            LodChildrenIterator_ f = binaryChildSearch_(key);

            if(f != vLodChildren_.end() && f->first == key)
                return false;	// the key already exists so we cannot insert it

            vLodChildren_.insert(f, LodChildrenPair_(key, position));
            return true;
        }

        void eraseLod(const Index_ &key)
        {
            LodChildrenIterator_ f = binaryChildSearch_(key);

            if(f != vLodChildren_.end() && f->first == key)
                vLodChildren_.erase(f);
        }

        void clearLod()
        {
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

        /**
         * @brief Binary search in the load on demand child nodes held by vLodChildren_
         */
        LodChildrenIterator_ binaryChildSearch_(const Index_ &index)
        {
            LodChildrenIterator_ low = vLodChildren_.begin();
            LodChildrenIterator_ high = vLodChildren_.end();
            while (low < high) {
                LodChildrenIterator_ mid = low + (high - low)/2;
                if ((*mid).first < index){
                    low = mid + 1;
                } else {
                    high = mid;
                }
            }
            return low;
        }

        /**
         * @brief Binary search in the virtual child nodes held by vVirtChildren_
         */
        VirtualChildrenIterator_ binaryVirtualChildSearch_(const Index_ &index)
        {
            VirtualChildrenIterator_ low = vVirtChildren_.begin();
            VirtualChildrenIterator_ high = vVirtChildren_.end();
            while (low < high) {
                VirtualChildrenIterator_ mid = low + (high - low)/2;
                if ((*mid).first < index){
                    low = mid + 1;
                } else {
                    high = mid;
                }
            }
            return low;
        }

    protected:
        std::vector<LodChildrenPair_> vLodChildren_;		///< A vector holding the load on demand child nodes
        std::vector<VirtualChildrenPair_> vVirtChildren_;	///< A vector holding the virtual children
    };

    /**
     * @brief Like GVectorPrefixTree but supports working on file based trees
     */
    template <class DataClass> class tmplVectorFilePrefixTree :
        public tmplAbstractFilePrefixTree<tmplFilePrefixTreeVectorNode<DataClass> >
    {
    public:
        typedef tmplAbstractFilePrefixTree< tmplFilePrefixTreeVectorNode<DataClass> > AbstractFileTree_;

    public:
        tmplVectorFilePrefixTree() : AbstractFileTree_() {};
        tmplVectorFilePrefixTree(const char *strFilename) : AbstractFileTree_(strFilename) {};
        tmplVectorFilePrefixTree(std::istream &inStream) : AbstractFileTree_(inStream) {};
    };
}
}
}

/**
 * @var Translation::tmplFilePrefixTreeVectorNode::vLodChildren_
 * @brief Holds the nodes load on demand children
 *
 * The prefix tree can be bound to a file. It will then load the nodes child nodes once follow demands them. Of course
 * these child nodes are not in memory, so they cannot be hold in the normal successors vector. Instead this variable
 * stores their index as well as their file offset, so the various follow methods know where to begin reading when the
 * node with this index is requested.
 */

/**
 * @var Translation::tmplFilePrefixTreeVectorNode::vVirtChildren_
 * @brief Holds the virtual child nodes, nodes that have been moved to a file
 *
 * There are 3 types of nodes. Nodes that are in memory are hold in the parent classes successor vector. Nodes that can
 * be load on demand, as the parents follow method requests this node, are stored in vLodChildren_. There is a third type
 * of nodes: Nodes that have been moved from memory to a file. While the first type of node is used both when creating
 * and writing a tree, as well as when reading from a file and using it the second type, the load on demand children,
 * are only used when the tree is being loaded. The virtual children are the nodes that are used when the tree is being
 * created. If you run out of memory you can just move some nodes to the tree file and continue the trees creation.
 * These nodes can not be stored with the other successor nodes, since they are not in memory and can no longer be accessed.
 */

#endif
