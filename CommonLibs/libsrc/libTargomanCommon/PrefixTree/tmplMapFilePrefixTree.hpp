#ifndef TARGOMAN_COMMON_PREFIXTREE_TMPLMAPPREFIXFILETREE_HH
#define TARGOMAN_COMMON_PREFIXTREE_TMPLMAPPREFIXFILETREE_HH

#include <istream>
#include <ostream>
#include <fstream>
#include <map>
#include <deque>
#include "libTargomanCommon/PrefixTree/tmplAbstractPrefixTree.hpp"
#include "libTargomanCommon/PrefixTree/tmplMapPrefixTree.hpp"
#include "libTargomanCommon/PrefixTree/tmplAbstractFilePrefixTree.hpp"

namespace Targoman {
namespace Common {
namespace PrefixTree {
    /**
     * @brief A tmplPrefixTreeMapNode implementation that can work with file based trees
     *
     * This class supports loading trees on demand.
     *
     * @see GMapFilePrefixTree
     */
    template <class Index, class Data, class Compare=std::less<Index> > class tmplFilePrefixTreeMapNode : public tmplPrefixTreeMapNode<Index, Data, Compare>, public tmplFilePrefixTreeAbstractNode<Index, Data>
    {
    protected:
        typedef tmplPrefixTreeAbstractNode<Index, Data> AbstractNode_;
        typedef tmplPrefixTreeMapNode<Index, Data, Compare> MapNode_;
        typedef tmplFilePrefixTreeAbstractNode<Index, Data> AbstractFileNode_;
        typedef tmplFilePrefixTreeMapNode<Index, Data> MapFileNode_;
        typedef typename MapNode_::Container_::iterator ContainerIterator_;
        typedef typename MapNode_::Container_::const_iterator ContainerConstIterator_;
        typedef typename AbstractNode_::AbstractWeakIterator AbstractWeakIterator_;

        typedef typename std::pair<Index, std::streampos> 											LodChildrenPair_;
        typedef typename std::pair<Index, typename AbstractFileNode_::VirtualChild> 				VirtualChildrenPair_;
        typedef typename std::map<Index, std::streampos>::iterator 									LodChildrenIterator_;
        typedef typename std::map<Index, std::streampos>::const_iterator 							LodChildrenConstIterator_;
        typedef typename std::map<Index, typename AbstractFileNode_::VirtualChild>::iterator 		VirtualChildrenIterator_;
        typedef typename std::map<Index, typename AbstractFileNode_::VirtualChild>::const_iterator 	VirtualChildrenConstIterator_;

        typedef typename AbstractFileNode_::LodChildAbstractIterator 					LodChildAbstractIterator_;
        typedef typename AbstractFileNode_::VirtualChildAbstractIterator 				VirtualChildAbstractIterator_;

    public:
        tmplFilePrefixTreeMapNode(const Index &index, AbstractNode_ *predecessor) : AbstractNode_(index, predecessor), MapNode_(index, predecessor), AbstractFileNode_(index, predecessor) {}
        tmplFilePrefixTreeMapNode(const Index &index, AbstractNode_ *predecessor, const Data &data) : AbstractNode_(index, predecessor, data), MapNode_(index, predecessor, data), AbstractFileNode_(index, predecessor) {}
        ~tmplFilePrefixTreeMapNode()
        {
            // discard all temporary files
            VirtualChildrenIterator_ it;
            for(it = mVirtChildren_.begin(); it != mVirtChildren_.end(); ++it)
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
        static AbstractFileNode_ *sCreateNode(AbstractNode_ *parent, const Index &index, std::istream &iStream, const std::streampos &spOffset)
        {
            // read the node type (See tmplPrefixTreeAbstractFileNode::FilePrefixTreeNodeType)
            quint8 iType;
            iStream.seekg(spOffset);
            iStream.read((char*)&iType, sizeof(quint8));

            // create the proper node type
            switch(iType)
            {
                case AbstractFileNode_::NT_VectorNode:
                    // not necessarey till now
                    break;
                case AbstractFileNode_::NT_FullVectorNode:
                    // not necessarey till now
                    break;
                case AbstractFileNode_::NT_MapNode:
                    return dynamic_cast<AbstractFileNode_*>(new MapFileNode_(index, dynamic_cast<AbstractNode_*>(parent)));
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
         * tmplFilePrefixTreeMapNode::mLodChildren_. If the index is not found in the first map this method will search
         * for it in the second map. In case it is listed there it will be loaded into the first map. Otherwise this method
         * returns 0.
         *
         * @param nextIndex The next index on the path, it will identify the child node to be returned by this method
         * @return The child node identified by \e nextIndex or 0 if there is no such node
         * @see tmplFilePrefixTreeAbstractNode::createAndLoadNode_
         */
        AbstractNode_ *follow(const Index &nextIndex) const
        {
            ContainerConstIterator_ i = MapNode_::successors_.find(nextIndex);
            AbstractNode_ *returnPointer;
            if (i == MapNode_::successors_.end())
            {
                // obviously the node has not been loaded into memory yet
                returnPointer = const_cast<MapFileNode_*>(this)->loadChild_(nextIndex);
                if(returnPointer == 0)
                    returnPointer = const_cast<MapFileNode_*>(this)->loadVirtualChild_(nextIndex);
            }else
                returnPointer = i->second;

            return returnPointer;
        }

        /**
         * @brief Tries to follow the path and creates new nodes as necessary
         *
         * Reimplemented because this method needs to create objects of type tmplFilePrefixTreeMapNode.
         */
        AbstractNode_ *followOrExpand(const Index &nextIndex)
        {
            ContainerConstIterator_ i = MapNode_::successors_.find(nextIndex);
            AbstractNode_ *returnPointer;
            if (i != MapNode_::successors_.end())
                returnPointer = i->second;
            else {
                // try to load the file from the file first
                returnPointer = this->loadChild_(nextIndex);
                if(returnPointer == 0)
                    returnPointer = this->loadVirtualChild_(nextIndex);
                if(returnPointer == 0)
                {
                    tmplFilePrefixTreeMapNode *newNode = new tmplFilePrefixTreeMapNode(nextIndex, this);
                    this->successors_[nextIndex] = newNode;
                    returnPointer =  newNode;
                }
            }
            return returnPointer;
        }

        /**
         * @brief Tries to follow the path and creates new nodes as necessary
         *
         * Reimplemented because this method needs to create objects of type tmplFilePrefixTreeMapNode.
         */
        AbstractNode_ *followOrExpand(const Index &nextIndex, const Data &standardValue)
        {
            ContainerConstIterator_ i = MapNode_::successors_.find(nextIndex);
            AbstractNode_ *returnPointer;
            if (i != MapNode_::successors_.end())
                returnPointer = i->second;
            else {
                // try to load the file from the file first
                returnPointer = this->loadChild_(nextIndex);
                if(returnPointer == 0)
                    returnPointer = this->loadVirtualChild_(nextIndex);
                if(returnPointer == 0)
                {
                    tmplFilePrefixTreeMapNode *newNode = new tmplFilePrefixTreeMapNode(nextIndex, this, standardValue);
                    MapNode_::successors_[nextIndex] = newNode;
                    returnPointer =  newNode;
                }
            }
            return returnPointer;
        }

        /**
         * @brief Returns true if this node is a leaf node
         *
         * This method will return true if there are no successors. That means that both the successor list as well as the
         * list of loadable successors in the file are empty.
         */
        bool isLeaf() const
        {
            return mLodChildren_.empty() && MapNode_::successors_.empty();
        }

        /**
         * @brief Initializes this node by reading its contents from the stream
         *
         * Both the Index and Data class used in the template need to implement
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
            for(unsigned long i = 0; i < iChildren; ++i)
            {
                Index index;
                quint64 iOffsetNode;

                // read the index and file offset
                index.readBinary(iStream);
                iStream.read((char*)&iOffsetNode, sizeof(quint64));

                // and store them
                mLodChildren_.insert(LodChildrenPair_(index, std::streampos(iOffsetNode)));
            }

            // and try to load the data
            quint32 dataLength;
            iStream.read((char*)&dataLength, sizeof(quint32));	// not used here - for now
            AbstractNode_::data_.readBinary(iStream);
        }

        void writeIndex(const Index &index, std::ostream &oStream) const
        {
            index.writeBinary(oStream);
        }

        void readIndex(Index &index, std::istream &iStream) const
        {
            index.readBinary(iStream);
        }

        /**
         * @brief This method returns the node type
         *
         * See tmplFilePrefixTreeAbstractNode::FilePrefixTreeNodeType for further reference.
         */
        unsigned char getType()
        {
            return AbstractFileNode_::NT_MapNode;
        }

#if 0
        size_t getSize()
        {
            // size of myself and the data size
            size_t mySize = sizeof(*this) + AbstractNode_::getData().getSize();

            // size of the structure holding the special child nodes
            mySize += mLodChildren_.size() * (sizeof(Index) + sizeof(std::streampos));
            mySize += mVirtChildren_.size() * (sizeof(Index) + sizeof(typename AbstractFileNode_::VirtualChild));
            // add the memory occupied by the strings
            VirtualChildAbstractIterator_ *it_v = beginVirtual(), *end_v = endVirtual();
            for(; !it_v->equals(end_v); it_v->increase())
                mySize += it_v->dereference().second.filename.size() + AbstractFileNode_::iMallocOverhead;	// is the string influenced by the malloc overhead?
            delete it_v;
            delete end_v;

            // finally add the child nodes
            AbstractWeakIterator_ *it = MapNode_::weakBegin(), *end = MapNode_::weakEnd();
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
         * @brief Implementation of the virtual child iterator for the map node class
         */
        class MapVirtualChildIterator : public VirtualChildAbstractIterator_
        {
        public:
            MapVirtualChildIterator() {}
            MapVirtualChildIterator(const VirtualChildAbstractIterator_ &other) : iter(other.iter) {}
            MapVirtualChildIterator(const VirtualChildrenIterator_ &iter) : iter(iter) {}
            ~MapVirtualChildIterator() {}

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
                return VirtualChildrenPair_(iter->first, iter->second);
            }

            bool equals(VirtualChildAbstractIterator_ *other) const
            {
                return iter == ((MapVirtualChildIterator*)other)->iter;
            }

            VirtualChildAbstractIterator_ *operator=(const VirtualChildAbstractIterator_ &other)
            {
                iter = ((MapVirtualChildIterator*)&other)->iter;
                return this;
            }

        protected:
            VirtualChildrenIterator_ iter;		///< This iterator is wrapping an iterator
        };

        VirtualChildAbstractIterator_ *beginVirtual()
        {
            return new MapVirtualChildIterator(mVirtChildren_.begin());
        }

        VirtualChildAbstractIterator_ *endVirtual()
        {
            return new MapVirtualChildIterator(mVirtChildren_.end());
        }

        VirtualChildAbstractIterator_ *findVirtual(const Index &key)
        {
            return new MapVirtualChildIterator(mVirtChildren_.find(key));
        }

        bool insertVirtual(const Index &key, const typename AbstractFileNode_::VirtualChild &virtChild)
        {
            return mVirtChildren_.insert(VirtualChildrenPair_(key, virtChild)).second;
        }

        void eraseVirtual(const Index &key)
        {
            mVirtChildren_.erase(key);
        }

        void clearVirtual()
        {
            VirtualChildrenIterator_ it;
            for(it = mVirtChildren_.begin(); it != mVirtChildren_.end(); ++it)
                remove(it->second.filename.c_str());
            mVirtChildren_.clear();
        }

        bool emptyVirtual()
        {
            return mVirtChildren_.empty();
        }

        /**
         * @brief Implementation of the load on demand child iterator for the map node class
         */
        class MapLodChildIterator : public LodChildAbstractIterator_
        {
        public:
            MapLodChildIterator() {}
            MapLodChildIterator(const LodChildAbstractIterator_ &other) : iter(other.iter) {}
            MapLodChildIterator(const LodChildrenIterator_ &iter) : iter(iter) {}
            ~MapLodChildIterator() {}

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
                return LodChildrenPair_(iter->first, iter->second);
            }

            bool equals(LodChildAbstractIterator_ *other) const
            {
                return iter == ((MapLodChildIterator*)other)->iter;
            }

            LodChildAbstractIterator_ *operator=(const LodChildAbstractIterator_ &other)
            {
                iter = ((MapLodChildIterator*)&other)->iter;
                return this;
            }

        protected:
            LodChildrenIterator_ iter;		///< This iterator is wrapping an iterator
        };

        LodChildAbstractIterator_ *beginLod()
        {
            return new MapLodChildIterator(mLodChildren_.begin());
        }

        LodChildAbstractIterator_ *endLod()
        {
            return new MapLodChildIterator(mLodChildren_.end());
        }

        LodChildAbstractIterator_ *findLod(const Index &key)
        {
            return new MapLodChildIterator(mLodChildren_.find(key));
        }

        bool insertLod(const Index &key, const std::streampos &position)
        {
            return mLodChildren_.insert(LodChildrenPair_(key, position)).second;
        }

        void eraseLod(const Index &key)
        {
            mLodChildren_.erase(key);
        }

        void clearLod()
        {
            mLodChildren_.clear();
        }

        bool emptyLod()
        {
            return mLodChildren_.empty();
        }
        //@}

    protected:
        AbstractFileNode_ *createNode_(AbstractNode_ *pParent, const Index &index, std::istream &iStream, const std::streampos &spOffset) const
        {
            return sCreateNode(pParent, index, iStream, spOffset);
        }

    protected:
        std::map<Index, std::streampos> mLodChildren_;									///< A map holding the load on demand child nodes
        std::map<Index, typename AbstractFileNode_::VirtualChild> mVirtChildren_;	///< A map holding the virtual children
    };

    /**
     * @brief Like GMapPrefixTree but supports working on file based trees
     */
    template <class Index, class Data, class Compare=std::less<Index> > class tmplMapFilePrefixTree :
        public tmplAbstractFilePrefixTree<tmplFilePrefixTreeMapNode<Index, Data, Compare> >
    {
    public:
        typedef tmplAbstractFilePrefixTree< tmplFilePrefixTreeMapNode<Index, Data, Compare> > AbstractFileTree_;

    public:
        tmplMapFilePrefixTree() : AbstractFileTree_() {};
        tmplMapFilePrefixTree(const char *strFilename) : AbstractFileTree_(strFilename) {};
        tmplMapFilePrefixTree(std::istream &inStream) : AbstractFileTree_(inStream) {};
    };
}
}
}

/**
 * @var Translation::tmplFilePrefixTreeVectorNode::mLodChildren_
 * @brief Holds the nodes load on demand children
 *
 * The prefix tree can be bound to a file. It will then load the nodes child nodes once follow demands them. Of course
 * these child nodes are not in memory, so they cannot be hold in the normal successors vector. Instead this variable
 * stores their index as well as their file offset, so the various follow methods know where to begin reading when the
 * node with this index is requested.
 */

/**
 * @var Translation::tmplFilePrefixTreeVectorNode::mVirtChildren_
 * @brief Holds the virtual child nodes, nodes that have been moved to a file
 *
 * There are 3 types of nodes. Nodes that are in memory are hold in the parent classes successor vector. Nodes that can
 * be load on demand, as the parents follow method requests this node, are stored in vChildren_. There is a third type
 * of nodes: Nodes that have been moved from memory to a file. While the first type of node is used both when creating
 * and writing a tree, as well as when reading from a file and using it the second type, the load on demand children,
 * are only used when the tree is being loaded. The virtual children are the nodes that are used when the tree is being
 * created. If you run out of memory you can just move some nodes to the tree file and continue the trees creation.
 * These nodes can not be stored with the other successor nodes, since they are not in memory and can no longer be accessed.
 */

#endif
