#ifndef TARGOMAN_COMMON_PREFIXTREE_TMPLABSTRACTPREFIXFILETREE_HPP
#define TARGOMAN_COMMON_PREFIXTREE_TMPLABSTRACTPREFIXFILETREE_HPP

#include <deque>
#include <iostream>
#include <unistd.h>

#include "libTargomanCommon/PrefixTree/tmplAbstractPrefixTree.hpp"

#define HEADER_FILEVERSION "GAbstractPrefixTree v1.0"

namespace Targoman {
namespace Common {
namespace PrefixTree {
    /**
     * @brief Extends the tmplPrefixTreeAbstractNode by file accessing methods
     *
     * Using multiple inheritance this class is attached to the nodes of file trees.
     *
     * @see GMapFilePrefixTree
     */
    template <class Index, class Data> class tmplFilePrefixTreeAbstractNode : virtual public tmplPrefixTreeAbstractNode<Index, Data>
    {
    public:
        typedef tmplFilePrefixTreeAbstractNode<Index, Data> AbstractFileNode;

    protected:
        typedef tmplPrefixTreeAbstractNode<Index, Data> AbstractNode_;
        typedef tmplFilePrefixTreeAbstractNode<Index, Data> AbstractFileNode_;

    protected:
        /**
         * @brief A structure representing a virtual child node
         *
         * Whenever moveToFile is used a virtual child node is created to take the child nodes place. Instead of holding
         * the data it only refers to a temporary file or to an offset in the tree file, depending on whether isFile is set.
         */
        class VirtualChild
        {
        public:
            VirtualChild(const VirtualChild &other) : isFile(other.isFile), filename(other.filename), pos(other.pos) {};
            VirtualChild(const std::string &filename) : isFile(true), filename(filename), pos(0) {};
            VirtualChild(const std::streampos &pos) : isFile(false), filename(""), pos(pos) {};

            bool isFile;			///< True if this child node is stored in a temporary file, the filename member is valid then
            std::string filename;	///< If isFile is true this member is holding the filename of the temporary file holding the node ("" otherwise)
            std::streampos pos;		///< If isFile is false this member is holding the offset in the tree file to the node (0 otherwise)
        };

    public:
        /**
         * @brief Enumeration listing the different file types, used to identify them when reading from disc
         */
        enum FilePrefixTreeNodeType
        {
            NT_VectorNode = 1,		///< The node is of type tmplPrefixTreeVectorNode
            NT_FullVectorNode,		///< The node is of type tmplPrefixTreeFullVectorNode
            NT_MapNode,				///< The node is of type tmplPrefixTreeMapNode
        };

        /**
         * @brief A structure holding variables set by the tree that change the behaviour of tree nodes
         *
         * The tree needs to set a number of variables to modify the nodes behaviour depending on the way the tree class is used.
         */
        struct TreeControlData
        {
            bool writeOnly;			///< True if nodes can only be written. moveToFile will write them to the tree file then, not a temporary file.
            std::ofstream *outputStream;	///< If writeOnly is true this is the open tree file, otherwise 0.
            std::istream *inputStream;		///< If this node is part of a file based tree this variable will hold the input stream used to load child nodes.
            bool closeFile;                     ///< Close the file
        };

    public:
        tmplFilePrefixTreeAbstractNode(const Index &index, AbstractNode_ *predecessor) : AbstractNode_(index, predecessor)
        {
            if(predecessor != 0)
            {
                controlData_ = dynamic_cast<AbstractFileNode_*>(predecessor)->controlData_;
            }else{
                controlData_ = new TreeControlData;
                controlData_->writeOnly = true;
                controlData_->inputStream = 0;
                controlData_->outputStream = 0;
                controlData_->closeFile = false;
            }
        }

        tmplFilePrefixTreeAbstractNode(const Index &index, AbstractNode_ *predecessor, const Data &data) : AbstractNode_(index, predecessor, data)
        {
            if(predecessor != 0)
            {
                controlData_ = predecessor->controlData_;
            }else{
                controlData_ = new TreeControlData;
                controlData_->writeOnly = true;
                controlData_->inputStream = 0;
                controlData_->outputStream = 0;
                controlData_->closeFile = 0;
            }
        }

        virtual ~tmplFilePrefixTreeAbstractNode()
        {
            if(AbstractNode_::up() == 0)
                delete controlData_;
        }

        /**
         * @brief Simply returns a pointer to the tree control data structure
         */
        TreeControlData *GetTreeControlData()
        {
            return controlData_;
        }

    public:
        /**
         * @brief Returns the memory usage of this nodes subtree
         *
         * This method calculates the memory usage of the subtree below this node (including the node itself).
         * Be aware that this method is slow since the whole tree will be traversed each time it is called. However, if it
         * is not used it does not introduce any overhead.
         *
         * This method is pure virtual because every type of node handles it child nodes in a different way introducing
         * a different amount of memory usage. Also, to capture all vtables properly, it is usefull to use sizeof on the
         * most specific class in the class hierarchy.
         *
         * This method will call the getSize member of the Data class:
         * \code
         * size_t getSize() const
         * \endcode
         * Since the data object is stored in the tmplPrefixTreeAbstractNode class itself there is no need for this method to
         * return sizeof(*this), since this size is already captured by the nodes getSize method. Only sum up memory
         * usage resulting from pointers and other members that introduce memory usage that is not inherited to the class
         * holding the Data as a member variable.
         *
         * @return Size of the sub tree in bytes
         */
#if 0
        virtual size_t getSize() = 0;
#endif

        /**
         * @brief Initializes this node by reading its contents from the stream
         *
         * @param iStream An input stream holding the node. Notice that the nodes type is already read when this method is called.
         */
        virtual void readBinary(std::istream &iStream)
        {
            Q_UNUSED(iStream);
        }

        /**
         * @brief Writes the given index to an output stream
         *
         * When writing a tree to a stream there is only one thing that differs between the implementations: the node index.
         * tmplPrefixTreeMapNode uses a general index class while the tmplPrefixTreeVectorNode uses a simple unsigned. Therefore
         * every node is responsible to store the indices of its children.
         *
         * The default implementation does nothing, reimplement it in the derived classes.
         *
         * The map implementation requires the index class to implement:
         * \code
         * void writeBinary(std::ostream &oStream);
         * \endcode
         *
         * @param pNode The index that has to be written
         * @param oStream Write to this stream
         */
        virtual void writeIndex(const Index &index, std::ostream &oStream) const = 0;

        /**
         * @brief Reads an index from an input stream
         *
         * The default implementation does nothing, reimplement it in the derived classes.
         *
         * The map implementation requires the index class to implement:
         * \code
         * void readBinary(std::ostream &oStream);
         * \endcode
         *
         * @param pNode The index that has to be read
         * @param iStream Read from this stream
         */
        virtual void readIndex(Index &index, std::istream &iStream) const = 0;

        /**
         * @brief This method returns the node type
         *
         * See tmplFilePrefixTreeAbstractNode::FilePrefixTreeNodeType for further reference.
         *
         * Reimplement this method when deriving a class.
         */
        virtual unsigned char getType() = 0;

        /**
         * @brief Writes this node to the given stream
         *
         * The node first writes the indices of all its children to the file. They are equipped with placeholders for the
         * file offset of the child nodes, which are unknown till now. They will be filled in once the corresponding node
         * is written. So make sure to traverse the nodes in an order that will write each node before its children.
         *
         * \code
         * Node format:
         *  1. [quint8]         node type
         *  2. [quint32]        number of Children
         *  3.              (list of children)
         *     a. [Index]   this childs index
         *     b. [quint64]     file position of the corresponding node
         *  4. [quint32]        the length of the data block
         *  5. [Data]       this nodes data
         * \endcode
         *
         * Notice that all node classes use exactly the same format, only the representation in memory differs. Normaly
         * this method does not need to be overwritten, since all nodes work the same way, except for the nodes index.
         * The index is handled by tmplFilePrefixTreeAbstractNode::writeIndex.
         *
         * For this method to work the Data element of the template needs to implement
         * \code
         * void writeBinary(std::ostream &oStream);
         * \endcode
         *
         * @param oStream The file stream to write to
         * @param mOffsets A map holding the nodes that still have to write their file offset to some index headers
         */
        virtual void writeBinary(std::ostream &oStream, std::multimap<AbstractFileNode_*, std::streampos> &mOffsets)
        {
            // store the current position to return to it and to store it in the index headers
            std::streampos spOffsetNode = oStream.tellp();	// tells us where the current node should be stored
            quint64 iOffset = (quint64)spOffsetNode;

            // do we have to write our location to some index headers of other nodes in the file?
            typename std::multimap<AbstractFileNode_*, std::streampos>::iterator it;
            for(it = mOffsets.lower_bound(this); it != mOffsets.upper_bound(this); ++it)
            {
                oStream.seekp(it->second);
                oStream.write((char*)&iOffset, sizeof(quint64));
            }
            mOffsets.erase(this);

            // return to the file position to store ourself
            oStream.seekp(iOffset);

            // write the node type
            quint8 iType = getType();
            oStream.write((char*)&iType, sizeof(quint8));

            // write the number of children to the file, otherwise we don't know where the indices end and the data begins
            // unfortunately we don't know yet how many children will have to be stored
            std::streampos spOffsetIndexSize = oStream.tellp();	// tells us where to write the number of children
            quint32 iChildren = 0;									// number of children is counted by the weak iterator
            oStream.write((char*)&iChildren, sizeof(quint32));

            // traverse all children
            // for each child, write its index and a placeholder for its position in the file
            typename AbstractNode_::AbstractWeakIterator *b = this->weakBegin(), *e = this->weakEnd();
            for(; !b->equals(e); b->increase(), iChildren++)
            {
                AbstractNode_ *pNode = b->dereference();

                // ask the derived node class to store the index
                writeIndex(pNode->getIndex(), oStream);

                // insert the current file position in the map; this is the place where the node referenced by pNode
                // has to store its offset once it is being written to the file
                mOffsets.insert(std::pair<AbstractFileNode_*, std::streampos>(dynamic_cast<AbstractFileNode_*>(pNode), oStream.tellp()));

                // leave some place for the file offset and register it in the map
                quint64 null = 0; // init the placeholder with zero
                oStream.write((char*)&null, sizeof(quint64));
            }
            delete b;
            delete e;

            // the iterator (see above) will only operate on children currently in memory, so handle the virtual child nodes separately
            std::multimap<Index, std::streampos> mVirtualOffsets;	// index table entries still to be set by virtual child nodes
            importVirtualChildNodeOffsets(oStream, iChildren, mVirtualOffsets);

            // now we know the number of children, so write it to the file
            spOffsetNode = oStream.tellp();		// remember the current position (the data will be written in this place)
            oStream.seekp(spOffsetIndexSize);	// go to the place where to write the offset
            oStream.write((char*)&iChildren, sizeof(quint32)); // write
            oStream.seekp(spOffsetNode);		// return to the place where the data has to be written

            // finally write the node data
            quint32 size = 0;
            quint64 begin, end;
            begin = oStream.tellp();
            oStream.write((char*)&size, sizeof(quint32));
            AbstractNode_::getData().writeBinary(oStream);
            end = oStream.tellp();
            size = end - begin - sizeof(quint32);
            // and its size
            oStream.seekp(begin);
            oStream.write((char*)&size, sizeof(quint32));
            oStream.seekp(end);

            // after this node has been written completely we can import the virtual child nodes
            importVirtualChildNodes(oStream, mVirtualOffsets);
        }

        /**
         * @brief Moves this node and all its children to a file
         *
         * The nodes are written to a temporary file and removed from memory. They are written to the tree file in case
         * its tmplAbstractFilePrefixTree::writeBinary method is called. Its destructor discards these files.
         *
         * Notice that all pointers to this node and nodes below this one become invalid and can no longer be used.
         * Do not use this method on the root node. Instead call writeBinary on the tree itself. Otherwise there will be
         * many temporary files holding the complete tree, but no one knows how to put them together to actually load it.
         *
         * @warning If you crash the program after calling moveToFile and before calling the parent nodes destructor,
         * 			a temporary file will be left behind in your /tmp/ folder. So clean it from time to time.
         * @see tmplAbstractFilePrefixTree::initVirtualNodes
         */
        void moveToFile()
        {
            if(controlData_->writeOnly)
            {
                // directly write to the target file
                // make the nodes write themself and their children
                std::multimap<AbstractFileNode_*, std::streampos> mOffsets;
                std::streampos pos = controlData_->outputStream->tellp();
                moveToFileRecursive(*controlData_->outputStream, mOffsets);

                // with this node getting removed from the tree the parent still needs to be notified of the change
                dynamic_cast<AbstractFileNode_*>(AbstractNode_::up())->movedToFile(this, pos);
            }else{
                // create a new temporary file
                // using tmpnam to be able to close the file without having it deleted
                //const char *filename = tmpnam(0);	// <- if you manage to crash the program this file will still exist in /tmp/
                char filename[] = ".tmpTreeXXXXXX";
                int fd = mkstemp(filename);
                close(fd); // This could be a security risk, but probably it's not so important for our goals
                           // Some SW engineering guru probably would kill me for such a comment
                std::ofstream tmpfile(filename, std::ofstream::out | std::ofstream::binary);

                // make the nodes write themself and their children
                std::multimap<AbstractFileNode_*, std::streampos> mOffsets;
                moveToFileRecursive(tmpfile, mOffsets);
                tmpfile.close();

                // with this node getting removed from the tree the parent still needs to be notified of the change
                dynamic_cast<AbstractFileNode_*>(AbstractNode_::up())->movedToFile(this, filename);
            }

            delete this;	// deletes ourself
        }

        /**
         * @name Iterator classes and methods
         * These classes and methods abstract the way the children are stored, since this depends on the actual
         * implementation while the more general methods of tmplFilePrefixTreeAbstractNode should not.
         */
        //@{
        /**
         * @brief An iterator class for virtual childern
         *
         * This iterator introduces an abstraction layer to hide the implementation details of the virtual child
         * storage methods. Virtual children are child nodes that have been moved to a file. See moveToFile for
         * further details.
         */
        class VirtualChildAbstractIterator
        {
        public:
            VirtualChildAbstractIterator() {}
            VirtualChildAbstractIterator(const VirtualChildAbstractIterator &other) {}
            virtual ~VirtualChildAbstractIterator() {}

            virtual VirtualChildAbstractIterator *increase() = 0;
            virtual VirtualChildAbstractIterator *decrease() = 0;
            virtual std::pair<Index, typename AbstractFileNode_::VirtualChild> dereference() const = 0;

            virtual bool equals(VirtualChildAbstractIterator *other) const = 0;
            virtual VirtualChildAbstractIterator *operator=(const VirtualChildAbstractIterator &other) = 0;
        };

        /**
         * @brief Returns an iterator pointing to the first virtual child node
         *
         * @warning Memory Leak danger: As for weakBegin/weakEnd you have to delete the pointer returned by this method
         * 			once it is no longer needed.
         */
        virtual VirtualChildAbstractIterator *beginVirtual() = 0;

        /**
         * @brief Returns an iterator pointing behind the last virtual child node
         *
         * @warning Memory Leak danger: As for weakBegin/weakEnd you have to delete the pointer returned by this method
         * 			once it is no longer needed.
         */
        virtual VirtualChildAbstractIterator *endVirtual() = 0;

        /**
         * @brief Finds a virtual child node based on its key
         *
         * Returns endVirtual() in case it is not found.
         *
         * @warning Memory Leak danger: As for weakBegin/weakEnd you have to delete the pointer returned by this method
         * 			once it is no longer needed.
         */
        virtual VirtualChildAbstractIterator *findVirtual(const Index &key) = 0;

        /**
         * @brief Inserts a new virtual child node into the list of children
         *
         * This method returns true if an insertion was made
         * @param key The key of the new element
         * @param virtChild The virtual children storage is based on the VirtualChild class
         * @return true if an insertion was made, false otherwise
         */
        virtual bool insertVirtual(const Index &key, const typename AbstractFileNode_::VirtualChild &virtChild) = 0;

        /**
         * @brief Finds and removes the element with the given key
         *
         * This method does nothing if there is no element with that key
         */
        virtual void eraseVirtual(const Index &key) = 0;

        /**
         * @brief Removes all virtual child nodes from memory
         */
        virtual void clearVirtual() = 0;

        /**
         * @brief Returns true if there are no virtual child nodes
         */
        virtual bool emptyVirtual() = 0;

        /**
         * @brief An iterator class for load on demand children
         *
         * This iterator introduces an abstraction layer to hide the implementation details of the load on demand
         * child storage methods. Load on demand children are child nodes that are stored in a file and have not
         * been accessed since the tree was bound to the file. See the various tree constructors for further details.
         */
        class LodChildAbstractIterator
        {
        public:
            LodChildAbstractIterator() {}
            LodChildAbstractIterator(const LodChildAbstractIterator &other) {}
            virtual ~LodChildAbstractIterator() {}

            virtual LodChildAbstractIterator *increase() = 0;
            virtual LodChildAbstractIterator *decrease() = 0;
            virtual std::pair<Index, std::streampos> dereference() const = 0;

            virtual bool equals(LodChildAbstractIterator *other) const = 0;
            virtual LodChildAbstractIterator *operator=(const LodChildAbstractIterator &other) = 0;
        };

        /**
         * @brief Returns an iterator pointing to the first load on demand child node
         *
         * @warning Memory Leak danger: As for weakBegin/weakEnd you have to delete the pointer returned by this method
         * 			once it is no longer needed.
         */
        virtual LodChildAbstractIterator *beginLod() = 0;

        /**
         * @brief Returns an iterator pointing behind the last load on demand child node
         *
         * @warning Memory Leak danger: As for weakBegin/weakEnd you have to delete the pointer returned by this method
         * 			once it is no longer needed.
         */
        virtual LodChildAbstractIterator *endLod() = 0;

        /**
         * @brief Finds a load on demand child node based on its key
         *
         * Returns endLod() in case it is not found.
         *
         * @warning Memory Leak danger: As for weakBegin/weakEnd you have to delete the pointer returned by this method
         * 			once it is no longer needed.
         */
        virtual LodChildAbstractIterator *findLod(const Index &key) = 0;

        /**
         * @brief Inserts a new load on demand child node into the list of children
         *
         * This method returns true if an insertion was made
         * @param key The key of the new element
         * @param position Load on demand nodes are stored using their position in the file
         * @return true if an insertion was made, false otherwise
         */
        virtual bool insertLod(const Index &key, const std::streampos &position) = 0;

        /**
         * @brief Finds and removes the element with the given key
         *
         * This method does nothing if there is no element with that key
         */
        virtual void eraseLod(const Index &key) = 0;

        /**
         * @brief Removes all load on demand child nodes from memory
         */
        virtual void clearLod() = 0;

        /**
         * @brief Returns true if there are no load on demand childs
         */
        virtual bool emptyLod() = 0;
        //@}

    protected:
        /**
         * @brief Creates a new node
         *
         * Creating a new node is not as simple as it seems. The child node needs to be of the proper class. The class is
         * stored in the file, using an unsigned char. However when creating the class the proper template parameters have
         * to be used. They cannot be stored in the file though. For example a vector node cannot create a map node, since
         * the vector node can only provide an unsigned integer as Index class, which does not implement FromBinary or
         * ToBinary. Therefore the node creation itself is implemented in the derived node classes. So a node will inherit
         * its template parameters to its child nodes.
         *
         * The reimplementation of createNode_ is not directly creating the node but calling a static version of the method
         * called sCreateNode. The reason for this is that not only nodes create nodes, but also the tree needs to create
         * a node, the root node. It is not possible to call a member function without an object (the root does not have a
         * parent, therefore there is no object), so this is what the static method is used for.
         *
         * @param pParent The parent node, can be 0 to create a root node (only used with the static version of this method)
         * @param index The new nodes index
         * @param iStream The file based prefix trees input stream
         * @param spOffset File offset of the new node
         * @see tmplFilePrefixTreeMapNode::sCreateNode
         * @see tmplFilePrefixTreeVectorNode::sCreateNode
         * @see tmplFilePrefixTreeFullVectorNode::sCreateNode
         */
        virtual AbstractFileNode_ *createNode_(AbstractNode_ *pParent, const Index &index, std::istream &iStream, const std::streampos &spOffset) const = 0;

        /**
         * @brief Creates a new node and loads its data from the file
         *
         * The type of the new node is stored in the file. It is read and the corresponding node is created. Then
         * the rest of its data is read from the file. The node is also equipped with the index taken from the parents
         * child offset table and the file pointer which will allow the new node to create further child nodes.
         * Do not forget to attach the returned node to the list of children. This cannot be done here, since every node
         * handels its children differently.
         *
         * The node still remembers the input stream from its own creation, and will pass it to its children. This way
         * follow does not need to cope with streams and can focus on the childs index.
         *
         * @param index The new nodes index
         * @param iOffset The file index where to begin reading
         * @return A new node, store it in its parents successor list
         */
        virtual AbstractNode_ *createAndLoadNode_(const Index &index, const std::streampos &spOffset) const
        {
            AbstractFileNode_ *pNode = createNode_((AbstractNode_*)this, index, *controlData_->inputStream, spOffset);
            if(pNode == 0)
                return 0;

            // load the node
            pNode->readBinary(*controlData_->inputStream);
            return dynamic_cast<AbstractNode_*>(pNode);
        }

        /**
         * @brief Tries to load a child node from the internal list of loadable successors
         *
         * The child node is identified by its index. If there is a node in the list of loadable successor nodes with
         * that index it will be created and attached as new child node.
         *
         * @param nextIndex The index of the successor node that requested by the caller
         * @return Either a pointer to a new node or zero if there is no successor with that index
         */
        AbstractNode_ *loadChild_(const Index &nextIndex)
        {
            if(emptyLod())
                return 0;

            // try to find the child node in the list of loadable children
            LodChildAbstractIterator *f = findLod(nextIndex), *end = endLod();
            if(f->equals(end))
            {
                delete f;
                delete end;
                return 0;
            }
            delete end;

            // create the node from the file and attach it as child
            AbstractNode_ *newNode = AbstractFileNode_::createAndLoadNode_(f->dereference().first, f->dereference().second);
            this->insert(nextIndex, newNode);

            delete f;
            return newNode;
        }

        /**
         * @brief Load the virtual child node given by \e nextIndex
         *
         * When read only mode is deactivated the user can use follow or followOrExpand to get into virtual child nodes
         * to modify them after they have been moved to a file using moveToFile. In that case the virtual node refered to
         * by \e nextIndex is load from the temporary file and thus becomes accessible. All child nodes of that virtual node
         * will not be loaded into memory, they will instead be moved to individual temporary files, one per node. Then
         * they will be attached to the root node, as virtual children. This way only a single node gets reloaded and the
         * process can be used recursive.
         *
         * @warning This method introduces some overhead by moving data from files to files. Use write only mode for trees
         * 			whenever possible. When you have to, try to keep the number of moveToFile and follow operations on the same
         * 			node (or subtree) as small as possible.
         *
         * @param nextIndex The index of the child node that is to be followed to
         */
        AbstractNode_ *loadVirtualChild_(const Index &nextIndex)
        {
            if(emptyVirtual())
                return 0;

            VirtualChildAbstractIterator *f = findVirtual(nextIndex), *fEnd = endVirtual();
            if(f->equals(fEnd))
            {
                delete f;
                delete fEnd;
                return 0;	// there is no child node with this index, not even in the list of virtual children
            }
            delete fEnd;

            if(!f->dereference().second.isFile)
                return 0;	// nodes in the tree file are not modifiable (since this would require to move a lot of data around the file) (disable write only mode if you got this one)

            // open the temporary file
            std::ifstream iStream(f->dereference().second.filename.c_str(), std::ifstream::in | std::ifstream::binary);
            if(iStream.fail())
            {
                delete f;
                return 0;
            }

            // create and load the root node from the file
            AbstractFileNode_ *pSubTreeRoot = createNode_((AbstractNode_*)this, nextIndex, iStream, 0);
            if(pSubTreeRoot == 0)
            {
                delete f;
                return 0;
            }
            pSubTreeRoot->readBinary(iStream);

            // now copy all child nodes of the sub tree root node into their own temporary file - making them all root nodes of their
            // own subtree (thus making the whole process recursive)
            LodChildAbstractIterator *it = pSubTreeRoot->beginLod(), *itEnd = pSubTreeRoot->endLod();
            for(; !it->equals(itEnd); it->increase())
            {
                // create a new temporary file
                // using tmpnam to be able to close the file without having it deleted
                // const char *filename = tmpnam(0);   // <- if you manage to crash the program this file will still exist in /tmp/
                char filename[] = ".tmpTreeXXXXXX";
                int fd = mkstemp(filename);
                close(fd); // This could be a security risk, but probably it's not so important for our goals
                           // Some SW engineering guru probably would kill me for such a comment
                std::ofstream oStream(filename, std::ofstream::out | std::ofstream::binary);
                // this maps node positions in the input file to index table offsets in the second file
                // so if you begin reading a node from a position listed as key in the map, make sure to
                // write the current position in the output stream to the position given by the data field
                // of the map (write to the output stream, this is where the index table needs to be updated)
                std::map<std::streampos, std::streampos> mChildren;

                // copy the tree using a queue
                // update file offsets to compensate for the new position in the tree file
                std::deque<std::streampos> queue;	// the queue is holding a number of nodes that have to be copied
                queue.push_back(it->dereference().second);
                while(!queue.empty())
                {
                    std::streampos currentNodeInputPos = queue.front();
                    iStream.seekg(currentNodeInputPos);
                    queue.pop_front();

                    // update the index table of some other node if necessary
                    std::map<std::streampos, std::streampos>::iterator f = mChildren.find(currentNodeInputPos);
                    if(f != mChildren.end())
                    {
                        quint64 currentNodeOutputPos = oStream.tellp();
                        oStream.seekp(f->second);
                        oStream.write((char*)&currentNodeOutputPos, sizeof(quint64));
                        oStream.seekp(currentNodeOutputPos);
                    }

                    // all variables we need to copy a node
                    quint8 nodeType;
                    quint32 numberOfChildren;
                    Index childIndex;
                    quint64 childOffset;
                    quint32 dataLength;
                    char *data;

                    // node type and number of children do not change
                    iStream.read((char*)&nodeType, sizeof(quint8));
                    if(iStream.eof())
                        break;
                    oStream.write((char*)&nodeType, sizeof(quint8));
                    iStream.read((char*)&numberOfChildren, sizeof(quint32));
                    oStream.write((char*)&numberOfChildren, sizeof(quint32));

                    // copy the index table
                    for(quint32 i = 0; i < numberOfChildren; ++i)
                    {
                        // index does not change, the offset however does
                        readIndex(childIndex, iStream);
                        writeIndex(childIndex, oStream);

                        // for now, we copy the index unmodified; the mChildren map will take care of the true index once the child node is copied
                        iStream.read((char*)&childOffset, sizeof(quint64));
                        queue.push_back(std::streampos(childOffset));
                        mChildren.insert(std::pair<std::streampos, std::streampos>(childOffset, oStream.tellp()));
                        oStream.write((char*)&childOffset, sizeof(quint64));
                    }

                    // creating a data object and reading/writing it from/to the file might have side effects and overhead
                    // better just copy it binary
                    iStream.read((char*)&dataLength, sizeof(quint32));
                    data = new char[dataLength];
                    iStream.read(data, dataLength);
                    oStream.write((char*)&dataLength, sizeof(quint32));
                    oStream.write(data, dataLength);
                    delete [] data;
                }
                oStream.close();

                // attach the temporary file as new virtual child node
                //dynamic_cast<MapFileNode_*>(pSubTreeRoot)->registerVirtualChild(it->first, filename);
                pSubTreeRoot->insertVirtual(it->dereference().first, typename AbstractFileNode_::VirtualChild(filename));
            }
            delete it;
            delete itEnd;

            // remove the load-on-demand-children (we moved them to temporary files and are not going to load them this way)
            pSubTreeRoot->clearLod();
            // attach the new node to the list of successors
            this->insert(nextIndex, pSubTreeRoot);

            iStream.close();
            remove(f->dereference().second.filename.c_str());
            delete f;
            // const_cast due to the const declaration of follow in GAbstractPrefixTree.hh and the non-const declaration of eraseVirtual in tmplAbstractFilePrefixTree.hh
            eraseVirtual(nextIndex);

            return dynamic_cast<AbstractNode_*>(pSubTreeRoot);
        }

        /**
         * @brief Moves this node and all its children to the given file
         *
         * This method implements the recursive part of the method moveToFile.
         *
         * @param tmpfile The stream to be written to
         * @param mOffsets The file offset map, used to set all necessary file indices
         */
        void moveToFileRecursive(std::ostream &oStream, std::multimap<AbstractFileNode_*, std::streampos> &mOffsets)
        {
            writeBinary(oStream, mOffsets);

            // move all nodes to the file
            typename AbstractNode_::AbstractWeakIterator *it = this->weakBegin(), *end = this->weakEnd();
            for(; !it->equals(end); it->increase())
                dynamic_cast<tmplFilePrefixTreeAbstractNode*>(it->dereference())->moveToFileRecursive(oStream, mOffsets);
            delete it;
            delete end;
        }

        /**
         * @brief This notification is called by the child node when it is done with writing itself to a file
         *
         * The child node is removed from the list of successors and instead moved to a different list holding nodes
         * together with the temporary file they were written to. When the tree is written these nodes will be copied
         * from the file.
         *
         * @param child The child node that is calling this method and going to remove itself
         * @param filename The temporary file the node is stored in
         */
        void movedToFile(AbstractFileNode_ *child, const char *filename)
        {
            // remove the child node (depends on the nodes successor implementation)
            this->erase(child->getIndex());

            // register the filename with the corresponding index (depends also on the nodes successor implementation)
            // registerVirtualChild(child->getIndex(), std::string(filename));
            insertVirtual(child->getIndex(), VirtualChild(filename));
        }

        /**
         * @brief Like the other movedToFile except that it refers to a position in the tree file, not a temporary file
         */
        void movedToFile(AbstractFileNode_ *child, const std::streampos &pos)
        {
            // remove the child node (depends on the nodes successor implementation)
            this->erase(child->getIndex());

            // register the filename with the corresponding index (depends also on the nodes successor implementation)
            // registerVirtualChild(child->getIndex(), pos);
            insertVirtual(child->getIndex(), VirtualChild(pos));
        }

        /**
         * @brief Writes index table entries for all virtual nodes
         *
         * Using the moveToFile method some nodes might have been moved from memory to a file. That
         * might be the tree file itself or some temporary file. This method will link it to the parent node, either
         * direct when the node was written to the tree file (file offset is already known) or by inserting its index table entry
         * into the \e mVirtualOffsets map (thus making the method importVirtualChildNodes set the index table entries properly).
         * When this method is called the parent nodes writeBinary is currently writing the index table, so this is how
         * linking to the parent works.
         *
         * When the method is called \e iChildren is holding the number of child node indices written till now. Make sure
         * to increase this number for every written child.
         *
         * @param oStream The output stream refering to the tree file
         * @param iChildren The number of child node indices written till now; increase it for every written child node
         * @param mVirtualOffsets Stores the index table elements that need to be written once the virtual child nodes are moved to the tree file
         * @see writeBinary
         */
        void importVirtualChildNodeOffsets(std::ostream &oStream, quint32 &iChildren, std::multimap<Index, std::streampos> &mVirtualOffsets)
        {
            VirtualChildAbstractIterator *it = beginVirtual(), *end = endVirtual();
            for(; !it->equals(end); it->increase(), ++iChildren)
            {
                // write the child nodes index
                writeIndex(it->dereference().first, oStream);

                // if the child node is already in the tree file refer to its index
                if(it->dereference().second.isFile == false)
                {
                    // its already in the tree file
                    quint64 pos = it->dereference().second.pos;
                    oStream.write((char*)&pos, sizeof(quint64));
                }else{
                    // its not in the tree file yet; store a placeholder zero and register it in the offset map
                    std::streampos offset = oStream.tellp();
                    quint64 null = 0;
                    oStream.write((char*)&null, sizeof(quint64));

                    mVirtualOffsets.insert(std::pair<Index, std::streampos>(it->dereference().first, offset));
                }
            }
            delete it;
            delete end;
        }

        /**
         * @brief Moves the virtual child nodes from temporary files into the tree file
         *
         * If there are any child nodes written to a temporary file they will be moved to the main tree file using this method.
         * It is called after writeBinary of the parent node completely wrote its index table and data. So for its (already written)
         * index table to refer to the child nodes that will be written now some of its offset pointers need to be adjusted.
         * This is done using the map mVirtualOffsets, which is holding the child nodes index and corresponding index table offset
         * for all nodes that still need to be attached to their parent node. This does not apply to virtual nodes that were written
         * to the tree node in the first place. Their index was already known to the parent node when it wrote itself.
         *
         * The temporary files will be deleted.
         *
         * @param oStream The output stream refering to the tree file
         * @param mVirtualOffsets The offset map filled by importVirtualChildNodeOffets
         */
        void importVirtualChildNodes(std::ostream &oStream, std::multimap<Index, std::streampos> &mVirtualOffsets)
        {
            VirtualChildAbstractIterator *it = beginVirtual(), *end = endVirtual();
            for(; !it->equals(end); it->increase())
            {
                if(it->dereference().second.isFile == false)
                    continue;

                // the first node in the file is the root node (of that sub tree) holding the index given as it->first
                std::streampos rootNodeStreamOffset = oStream.tellp();
                quint64 rootNodeOffset = rootNodeStreamOffset;

                // set the index table entry if necessary
                typename std::multimap<Index, std::streampos>::iterator f = mVirtualOffsets.find(it->dereference().first);
                if(f != mVirtualOffsets.end())
                {
                    oStream.seekp(f->second);
                    oStream.write((char*)&rootNodeOffset, sizeof(quint64));
                    oStream.seekp(rootNodeOffset);
                    mVirtualOffsets.erase(f);
                }

                // copy the tree in flat mode, one node at a time
                // update file offsets to compensate for the new position in the tree file
                std::ifstream iStream(it->dereference().second.filename.c_str(), std::ifstream::in | std::ifstream::binary);
                while(!iStream.eof())
                {
                    // all variables we need to copy a node
                    quint8 nodeType;
                    quint32 numberOfChildren;
                    Index childIndex;
                    quint64 childOffset;
                    quint32 dataLength;
                    char *data;

                    // node type and number of children do not change
                    iStream.read((char*)&nodeType, sizeof(quint8));
                    if(iStream.eof())
                        break;
                    oStream.write((char*)&nodeType, sizeof(quint8));
                    iStream.read((char*)&numberOfChildren, sizeof(quint32));
                    oStream.write((char*)&numberOfChildren, sizeof(quint32));

                    // copy the index table
                    for(quint32 i = 0; i < numberOfChildren; ++i)
                    {
                        // index does not change, the offset however does
                        readIndex(childIndex, iStream);
                        writeIndex(childIndex, oStream);

                        iStream.read((char*)&childOffset, sizeof(quint64));
                        childOffset += rootNodeOffset;
                        oStream.write((char*)&childOffset, sizeof(quint64));
                    }

                    // creating a data object and reading/writing it from/to the file might have side effects and overhead
                    // better just copy it binary
                    iStream.read((char*)&dataLength, sizeof(quint32));
                    data = new char[dataLength];
                    iStream.read(data, dataLength);
                    oStream.write((char*)&dataLength, sizeof(quint32));
                    oStream.write(data, dataLength);
                    delete [] data;
                }
                iStream.close();
            }
            delete it;
            delete end;
            clearVirtual(); // make sure the virtual children are not released twice (see destructor)
        }

    public:
        static quint64 iMallocOverhead;		///< The malloc overhead per data block
    protected:
        TreeControlData *controlData_;	///< Variables set by the tree controlling the behaviour of the node; inherited to child nodes
    };

    /**
     * @var tmplFilePrefixTreeAbstractNode::iMallocOverhead
     * @brief The malloc overhead per data block
     * @todo Find a better way to calculate the malloc overhead.
     *
     * To properly calculate the tree size we also need to keep track of the malloc overhead. To calculate it allocate
     * a huge array of pointers and use new to initialize each of them, then calculate the size of the array and add
     * the size of all the pointers as well as the size of what the pointers are pointing to. Compile it using your
     * compiler and compare the size returned by the process table with the size calculated by your code. The difference
     * divided by the number of pointers in the array will yield an approximation for the overhead.
     *
     * \code
     *	unsigned long long length = 25000000, i, size;
     *	int **spam = new int*[length];
     *	for(i = 0; i \< length; ++i)
     *		spam[i] = new int;
     *
     *	size = sizeof(spam) + length * sizeof(int*) + length * sizeof(int); // + length * 12; // <-- add the overhead to see if it matches
     *
     *	std::cout << "Size of Array: " << (size / 1024) << " KB" << std::endl;
     *	char tmp[1024];
     *	std::cin.getline(tmp, 1023);
     *
     *	for(i = 0; i \< length; ++i)
     *		delete spam[i];
     *	delete [] spam;
     * \endcode
     *
     * This returned approximately 12 for Jod (32 Bit) and 28 for technetium (64 Bit). Feel free to find a better way to do this.
     */
#ifndef HAS_64BIT
    // 32 bit
    template <class Index, class Data> quint64 tmplFilePrefixTreeAbstractNode<Index, Data>::iMallocOverhead = 12;	// calculated on Jod (32 Bit)
#else
    // 64 bit
    template <class Index, class Data> quint64 tmplFilePrefixTreeAbstractNode<Index, Data>::iMallocOverhead = 24;	// calculated on Technetium (64 Bit)
#endif

    /**
     * @brief Like GAbstractPrefixTree but supports working on file based trees
     *
     * The file based prefix tree introduces some new features to the prefix trees:
     * - writing a tree to a file
     * - reading tree nodes on demand from a file
     * - writing tree nodes to a file while building up the tree
     *   - without the option to modify it or its children
     *   - with the option to modify it or its children
     *
     * \section secWrite Writing a tree to a file
     * When you are done building the tree using the follorOrExpand methods it can be written to a file by calling the
     * tmplAbstractFilePrefixTree::writeBinary method. Make sure your Index and Data class offer the following methods
     * \code
     *	void readBinary(std::istream &iStream)
     *	void writeBinary(std::ostream &oStream) const
     * \endcode
     * Both methods are supposed to read or write your data/index to the current get/put pointer in the stream.
     * Your Data class also has to offer a method that returns the size of memory it allocated plus the malloc overhead
     * for each of the pointers/arrays it uses. It must not return its own size (sizeof(*this)), since this is already
     * taken into account by the parent node holding the data/index.
     * \code
     *	size_t getSize() const
     * \endcode
     *
     * \section secRead Reading tree nodes on demand from a file
     * You have to bind the tree to a file by using the proper constructor, e.g.
     * tmplAbstractFilePrefixTree::tmplAbstractFilePrefixTree(const char *strFilename). When this is done you can use the
     * tree nodes tmplFilePrefixTreeAbstractNode::follow method to follow into nodes that may or may not have been loaded into
     * memory, the node will load them itself if they are in the file and not in memory. It will return 0 if it cannot
     * find the node. I'm not quite sure what happens if you write a load on demand tree using the
     * tmplAbstractFilePrefixTree::writeBinary method. It is likely that it will "forget" about all nodes that are in the source
     * tree file but not in memory. If you need to do this check out the tmplFilePrefixTreeAbstractNode::writeBinary method,
     * it has to be extended to import load on demand childs (as it already does with the virtual child nodes). The
     * tmplPrefixTreeAbstractNode::clear and tmplFilePrefixTreeAbstractNode::getSize methods will be usefull with load on demand trees.
     *
     * @note The nodes that can be load from the file as the user requests them are called "load on demand"
     * 		 and are abbreviated as "Lod".
     *
     * \section secMove Writing tree nodes to a file while building up the tree
     * You can move nodes to a file as you build up the tree to make sure you don't run short of memory. The nodes are normally
     * written in depth-first order - however, if you decide to use tmplFilePrefixTreeAbstractNode::moveToFile this order is corrupted.
     * The node and all its children will be stored in the tree file at once, using depth-first order, and will be connected to its
     * parent node as it is written.
     *
     * Whenever possible move nodes to a file only if you are sure that you do not need to modify them in the future. In that
     * case some optimizations will allow for a smaller overhead. However, if you have to, you can choose the second method to
     * reload them into memory and modify them.
     *
     * @note The nodes that have been moved to a file are called "virtual"
     * 		 and are abbreviated as "Virtual".
     *
     *
     * \subsection secMoveNoModify Moving nodes using the write only mode
     * This mode is much faster then the other. If you decide to use the tmplFilePrefixTreeAbstractNode::moveToFile method you
     * have to call tmplAbstractFilePrefixTree::initVirtualNodes once, before using moveToFile for the first time (e.g. directly
     * after creating the tree). As parameters use true for writeOnly and the tree file name for filename. It will be opened
     * and all nodes go in there as their moveToFile method is called. To finalize the file call
     * tmplAbstractFilePrefixTree::writeBinary, but this time without any parameter. This will write all nodes that still reside
     * in memory and finally close the file. The target file is already known so it is not needed.
     *
     * \subsection secMoveModify Moving nodes to a file allowing later changes
     * This method is not recommended since it can be quite slow. As with the other mode you call
     * tmplAbstractFilePrefixTree::initVirtualNodes once to initialize it. However, this time you pass false as parameter writeOnly
     * and leave the filename zero. If you now use tmplFilePrefixTreeAbstractNode::moveToFile on a node this will create a
     * temporary file and store the subtree below the node and the node itself in it. This will not only make sure the
     * main tree file will be written in strict depth-first order, it will also enable you to reload nodes back into memory
     * as they are needed. If you tmplFilePrefixTreeAbstractNode::follow into a node that has been written to a file the method
     * will no longer return 0, but it will instead load the node from the file and attach it as child. The temporary file
     * is then split and all child nodes of the node that was just reloaded will be moved to their very own temporary file. This
     * makes the whole process recursive. Maybe this should be modified depending on how these methods are used.
    *
        * The tree and node classes have also been extended by a getSize and a clear method. The getSize method will return the
        * memory usage of the whole tree or subtree if used on a node in bytes, provided that you properly choose a value for
        * tmplFilePrefixTreeAbstractNode::iMallocOverhead. The clear method will remove all successor nodes from memory. Use it with
        * load on demand trees. You do not need to use it relation with moveToFile, it will handle successors itself.
        *
        * \section secOverhead Overhead
        * \subsection secOverheadFollow Follow and FollowOrExpand overhead
        * The file based prefix tree extension is designed to introduce as little overhead as possible if it is not used.
        * Have a look at tmplFilePrefixTreeMapNode::follow :
        * \code
        if (i == MapNode_::successors_.end())
        {
            // obviously the node has not been loaded into memory yet
            returnPointer = const_cast\<MapFileNode_*\>(this)->loadChild_(nextIndex);
            if(returnPointer == 0)
                returnPointer = const_cast\<MapFileNode_*\>(this)->loadVirtualChild_(nextIndex);
        }else
            * \endcode
                * If it cannot find the successor node it will attempt to load it from the tree file, if that failed again it will
                * try to load it from the virtual successor nodes.
                * \code
                if(emptyLod())
                    return 0;
        * \endcode
            * \code
            if(emptyVirtual())
                return 0;
        * \endcode
            * Both methods will first check their successor vectors/maps if they hold any elements, so if you are not using any
            * of the new features they will be empty and the methods return at once.
            *
            * \subsection secOverheadWrite Write overhead
            * Make sure to use the write only mode for moveToFile whenever possible, this will prevent creation of temporary files
            * and a lot of data movement.
            *
            * \subsection secOverheadGetSize getSize overhead
            * tmplFilePrefixTreeAbstractNode::getSize does not introduce overhead. This makes calls to this method slow, so if it is
            * used a lot an incremental version might be useful.
            */
    template <class RootNodeClass> class tmplAbstractFilePrefixTree :
        public tmplAbstractPrefixTree<RootNodeClass>
    {
    public:
        typedef typename RootNodeClass::AbstractFileNode FileNode;

    protected:
        typedef tmplAbstractPrefixTree<RootNodeClass> AbstractTree_;
        typedef tmplFilePrefixTreeAbstractNode<typename RootNodeClass::Index, typename RootNodeClass::Data> AbstractFileNode_;

    public:
        /**
         * @brief Default constructor
         *
         * The prefix tree will not be attached to a stream. Use this constructor if you want to create a tree
         * (that can be stored on the hard drive).
         */
        tmplAbstractFilePrefixTree() : ownInputStream_(0), inputStream_(0), initialPosition_(0)
        {
            controlData_ = dynamic_cast<AbstractFileNode_*>(this->rootNode())->GetTreeControlData();
        }

        /**
         * @brief Create a file based prefix tree
         *
         * The file given as \e strFilename will be opened and the tree will try to load missing nodes as follow demands them.
         * The destructor of this class instance will close the file.
         *
         * The constructor will create and load the root node from the file, its type might differ from the type
         * given by this tree.
         *
         * @param strFilename The name of the file holding the tree
         */
        tmplAbstractFilePrefixTree(const char *strFilename)
        {
            ownInputStream_ = new std::ifstream(strFilename, std::ios_base::in);
            if(ownInputStream_->fail())
            {
                delete ownInputStream_;
                ownInputStream_ = 0;
                inputStream_ = 0;
                std::cerr << "File not found: '" << strFilename << "' (in tmplAbstractFilePrefixTree(const char *strFilename))" << std::endl;
                return;
            }

            initialPosition_ = 0;
            if(!verifyHeader_(*ownInputStream_))
            {
                delete ownInputStream_;
                ownInputStream_ = 0;
                inputStream_ = 0;
                std::cerr << "Invalid file version: '" << strFilename << "' (in tmplAbstractFilePrefixTree(const char *strFilename))" << std::endl;
                return;
            }

            inputStream_ = (std::istream*)ownInputStream_;

            createAndLoadRoot_();
        }

        /**
         * @brief Create a prefix tree and attach it to a stream
         *
         * The input stream is supposed to be open and the get pointer set to the first byte of this tree.
         * The tree will load missing nodes as follow demands them. The destructor of this class instance will not close the file.
         *
         * The constructor will create and load the root node from the file, its type might differ from the type
         * given by this tree.
         *
         * @param inStream A reference to an open input stream
         */
        tmplAbstractFilePrefixTree(std::istream &inStream) : ownInputStream_(0), inputStream_(&inStream), initialPosition_(inputStream_->tellg())
        {
            if(!verifyHeader_(*inputStream_))
            {
                inputStream_ = 0;
                std::cerr << "Invalid file version (in tmplAbstractFilePrefixTree(std::istream &inStream))" << std::endl;
                return;
            }

            createAndLoadRoot_();
        }

        void readBinary(std::istream &inStream)
        {
            ownInputStream_ = 0;
            inputStream_ = &inStream;
            initialPosition_ = inputStream_->tellg();
            if(!verifyHeader_(*inputStream_))
            {
                inputStream_ = 0;
                std::cerr << "Invalid file version (in tmplAbstractFilePrefixTree(std::istream &inStream))" << std::endl;
                return;
            }

            createAndLoadRoot_();
        }

        /**
         * @brief Destructor
         *
         * The destructor will close the input file stream if it was opened using the
         * GMapFilePrefixTree(const char *) constructor.
         */
        ~tmplAbstractFilePrefixTree()
        {
            if(ownInputStream_ != 0)
            {
                ownInputStream_->close();
                delete ownInputStream_;
            }
        }

    public:
        /**
         * @brief Returns the memory usage of this tree
         *
         * This method calculates the memory usage in bytes by calling tmplFilePrefixTreeAbstractNode::getSize on the root node.
         * Be aware that this method is slow since the whole tree will be traversed each time it is called. However, if it
         * is not used it does not introduce any overhead.
         *
         * It is possible to calculate the size of a subtree by calling tmplFilePrefixTreeAbstractNode::getSize on its root node.
         *
         * @return Size of the whole tree in bytes
         */
#if 0
        size_t getSize()
        {
            size_t mySize = sizeof(*this);
            if(ownInputStream_ != 0)
                mySize += sizeof(*ownInputStream_) + AbstractFileNode_::iMallocOverhead;
            if(controlData_ != 0)
                mySize += sizeof(*controlData_) + AbstractFileNode_::iMallocOverhead;
            // the size of the member inputStream_ does not matter, it is stored "outside" of this tree and maintained by the caller

            if(AbstractTree_::rootNode() == 0)
                return mySize;
            return mySize + dynamic_cast<AbstractFileNode_*>(AbstractTree_::rootNode())->getSize() + AbstractFileNode_::iMallocOverhead;
        }
#endif

        /**
         * @brief Writes the tree to a stream
         *
         * Like writeBinary(std::ostream &oStream), but opens the stream for you. If you called initVirtualNodes and specified
         * writeOnly as true then the filename is not used and you can pass an empty string if you like. Instead it will write
         * to the file passed to initVirtualNodes.
         *
         * @param strFilename The name of the file to write to. The file is overwritten if it exists.
         * @see writeBinary(std::ostream &oStream)
         */
        void writeBinary(const std::string &filename)
        {
            writeBinary(filename.c_str());
        }

        /**
         * @brief Writes the tree to a stream
         *
         * Like writeBinary(std::ostream &oStream), but opens the stream for you. If you specified writeOnly as true when calling
         * initVirtualNodes, the parameter strFilename is not used and may be 0. Instead this method will write to the file passed
         * to initVirtualNodes.
         *
         * @param strFilename The name of the file to write to. The file is overwritten if it exists.
         * @see writeBinary(std::ostream &oStream)
         */
        void writeBinary(const char *strFilename = 0)
        {
            if(controlData_->outputStream == 0)
            {
                // nothing has been written till now, open a new file
                std::ofstream out(strFilename, std::ios_base::out | std::ios_base::binary);
                if(out.fail())
                    return;
                writeBinary(out);
                out.close();
            }else{
                // some nodes are already written, finalize the file
                writeBinary(*controlData_->outputStream);

                if (controlData_->closeFile) {
                    // this stream was opened by initVirtualNodes
                    controlData_->outputStream->close();
                    delete controlData_->outputStream;
                    controlData_->outputStream = 0;
                }
            }
        }

        /**
         * @brief Writes the tree to a stream
         *
         * The tree is written in binary format to the stream given as \e oStream. The file begins with the file header.
         * The file header consists of the string stored in HEADER_FILEVERSION followed by the offset to the root node.
         *
         * \code
         *  1. [char 24]    "GAbstractPrefixTree v1.0"
         *  2. [quint64]        offset to the root node
         * \endcode
         * After the header the nodes are written one by one, each using the format specified in
         * tmplFilePrefixTreeAbstractNode::writeBinary:
         * \code
         * Node format:
         *  1. [quint8]         node type
         *  2. [quint32]        number of Children
         *  3.              (list of children)
         *     a. [Index]   this childs index
         *     b. [quint64]     file position of the corresponding node
         *  4. [quint32]        the length of the data block
         *  5. [Data]       this nodes data
         * \endcode
         *
         * Usually they are stored in depth first order, hopefully increasing read spead. However, when some nodes have
         * been written using tmplFilePrefixTreeAbstractNode::moveToFile this is no longer true and the nodes are only
         * partially ordered. Unless, of course, the nodes were written to temporary files. This enables the tree to
         * write them in depth first order. See tmplFilePrefixTreeAbstractNode::initVirtualNodes for details on this.
         *
         * @warning If you set writeOnly to true when calling initVirtualNodes then \e do \e not pass a different stream to this
         * 			method then the one opened by initVirtualNodes and stored in the controlData_ structure - in fact do not call
         * 			this method at all. Use writeBinary(const char *strFilename) and pass 0 as strFilename or
         * 			writeBinary(const std::string &filename) with an empty string.
         *
         * @param oStream A std:ostream object to write to. The stream should be open.
         */
        void writeBinary(std::ostream &oStream)
        {
            if(controlData_->outputStream == 0)	// if outputStream is not 0 some nodes have already been written to the main tree file
            {
                // no nodes have been written to this stream yet (that does not mean that the stream got its put pointer at 0)
                initialPosition_ = oStream.tellp();
                writeHeader_(oStream);
            }

            // the first node that is going to be written is the root node (notice that this does not necessarily
            // make the root the first node in the file, since other subtrees might already be written)
            // get its position and store it to the file header
            quint64 posRoot = oStream.tellp();
            oStream.seekp((quint64)initialPosition_ + (quint64)strlen(HEADER_FILEVERSION));
            oStream.write((char*)&posRoot, sizeof(quint64));
            oStream.seekp(posRoot);

            // a multimap holding all unset file offsets
            // nodes are stored in depth-first order, so they are refering to nodes (their children) that have not
            // been written to the file yet. That's why they don't have a file offset yet and have to store it once
            // they are being written
            std::multimap<AbstractFileNode_*, std::streampos> mOffsets;

            // traverse the tree in depth first order
            typename AbstractTree_::iterator it;
            for(it = AbstractTree_::begin(); it != AbstractTree_::end(); ++it)
            {
                // since only the node itself has access to its successors and their indices it has to write itself
                dynamic_cast<AbstractFileNode_*>(*it)->writeBinary(oStream, mOffsets);
            }
        }

        /**
         * @brief This method initializes virtual nodes and enables moveToFile
         *
         * There are 3 ways to save a tree. First is to use writeBinary to write it to a file in one go. Second is to
         * use moveToFile on some nodes in the tree. They are immediately removed from memory and written to a file.
         * Then call the trees writeBinary method to finalize the file. Third method is to call moveToFile on some nodes
         * but then use the follow methods to regain access to these nodes and modify them. After modifying them they can
         * be stored in a file using moveToFile again, or they can remain in memory. The second and third approach
         * use virtual nodes, so if you want to use them call this methods with the proper parameters.
         *
         * A virtual node is just like a normal node, except that it is stored in a file and cannot be modified without
         * reloading it into memory. Nodes can only be reload into memory if the parameter writeOnly is set to false.
         *
         * With write only mode enabled, the nodes are stored in the tree output file and can no longer be modified.
         * This is fast and does not require additional file operations beyond the ones used to store the tree using
         * writeBinary. However you have to pick a filename for the tree and pass it as \e filename.
         * If write only mode is disabled \e filename is not used.
         *
         * @warning Do not enable or disable virtual nodes after you used moveToFile for the first time. It might have
         * 			unexpected results.
         * @param writeOnly True if you want fast trees without the option to modify nodes moved to a file,
         * 					False if you need to modify tree nodes that have been moved to a file
         * @param filename The filename of the tree file on the hard drive, not used if writeOnly is false
         * 				   (uses temporary files instead then)
         */
        void initVirtualNodes(bool writeOnly, std::ofstream &file) {
            controlData_->writeOnly = writeOnly;

            if(writeOnly == true)
            {
                controlData_->outputStream = &file;
                initialPosition_ = file.tellp();
                writeHeader_(file);
            }
        }

        void initVirtualNodes(bool writeOnly, const char *filename = 0)
        {
            if(writeOnly == true && filename == 0)
                return;		// bad parameters: no target filename

            controlData_->writeOnly = writeOnly;

            if(writeOnly == true)
            {
                // This stream will be closed and deleted by the next call to the trees writeBinary method
                std::ofstream *file = new std::ofstream(filename, std::ofstream::out | std::ofstream::binary);
                if(file->fail()) // could not open file
                {
                    delete file;
                    return;
                }
                writeHeader_(*file);

                controlData_->outputStream = file;
                initialPosition_ = 0;	// we created the file, so start at 0
            }
        }

    private:
        /**
         * @brief Writes the file header to the given stream
         *
         * The current Version is stored in HEADER_FILEVERSION.
         *
         * @note The header has constant length of 18 characters
         * @param oStream The output stream
         */
        void writeHeader_(std::ostream &oStream)
        {
            std::string header(HEADER_FILEVERSION);			// store the file header, fixed length
            oStream.write(header.c_str(), header.size());	// without trailing zero
            quint64 offsetRoot = 0;								// store a placeholder for the root node offset
            oStream.write((char*)&offsetRoot, sizeof(quint64));
        }

        /**
         * @brief Verifies that the file given as input is of the same version used to compile this program
         *
         * The current Version is 1.0.
         *
         * The current Version is stored in HEADER_FILEVERSION.
         *
         * @note The header has constant length of HEADER_FILEVERSION_SIZE characters
         * @param iStream The input stream
         * @return true if the given file has the same version, false otherwise
         */
        bool verifyHeader_(std::istream &iStream)
        {
            bool ret = true;
            iStream.seekg(initialPosition_);

            int len = strlen(HEADER_FILEVERSION);
            char *header = new char[len + 1];
            iStream.read(header, len);
            header[len] = 0;
            if(strcmp(header, HEADER_FILEVERSION) != 0)
                ret = false;

            delete [] header;
            return ret;
        }

        /**
         * @brief Load the root node from the file
         *
         * The prefix tree constructor is equipping the prefix tree with a root node. Unfortunately this root node does
         * not have the proper type and cannot be used in a file based tree. Therefore it has to be replaced by a new
         * root node.
         */
        void createAndLoadRoot_()
        {
            if(inputStream_ == 0)
                return;

            // The GAbstractPrefixTree already created a root node of some type. That is not the type of node that can
            // be used in file based prefix trees. The "solution" is to delete the root node and create our own one,
            // read from the file
            if(this->rootNode_ != 0)
                delete this->rootNode_;

            // read the root node file offset
            quint64 offset = (quint64)initialPosition_ + (quint64)strlen(HEADER_FILEVERSION);
            inputStream_->seekg(offset);
            inputStream_->read((char*)&offset, sizeof(quint64));

            // create a new root node of the type given in the input file
            AbstractFileNode_ *pNewRoot = RootNodeClass::sCreateNode(0, typename RootNodeClass::Index(), *inputStream_, offset);
            controlData_ = pNewRoot->GetTreeControlData();
            controlData_->inputStream = inputStream_;
            pNewRoot->readBinary(*controlData_->inputStream);
            this->rootNode_ = dynamic_cast<typename AbstractTree_::Node*>(pNewRoot);
        }

    private:
        std::ifstream *ownInputStream_;		///< If we opened an input stream ourselfs this pointer refers to it and is used to close it
        std::istream *inputStream_;			///< If this is not null the follow method of nodes will try to load from this stream
        std::streampos initialPosition_; 	///< Position to start reading in the file (i.e. if the prefix tree is part of a bigger file)
        typename AbstractFileNode_::TreeControlData *controlData_;	///< A pointer to the control data structure. Used to change the behavior of tree nodes
    };
}
}
}
#endif
