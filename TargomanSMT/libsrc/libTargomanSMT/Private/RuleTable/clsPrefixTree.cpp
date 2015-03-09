/*************************************************************************
 * Copyright © 2012-2015, Targoman.com
 *
 * Published under the terms of TCRL(Targoman Community Research License)
 * You can find a copy of the license file with distributed source or
 * download it from http://targoman.com/License.txt
 *
 *************************************************************************/
/**
 @author Saeed Torabzadeh <saeed.torabzadeh@gmail.com>
 @author Behrooz Vedadian <vedadian@gmail.com>
 */

#include "clsPrefixTree.h"
#include "Private/RuleTable/FStreamExtended.h"

namespace Targoman {
namespace SMT {
namespace Private {
namespace RuleTable {

using namespace Common;

clsPrefixTreeNode::clsPrefixTreeNode(clsIFStreamExtended *_inputStream) :
    Data(new clsPrefixTreeNodeData(_inputStream))
{
    int ChildCount = _inputStream->read<int>();
    for(int i = 0; i < ChildCount; ++i) {
        WordIndex_t Key = _inputStream->read<WordIndex_t>();
        PosType_t Position = _inputStream->read<PosType_t>();
        this->Data->ChildPositionInStream[Key] = Position;
    }
    this->Data->RuleNode.readBinary(*static_cast<std::istream*>(this->Data->InputStream));
}

void clsPrefixTreeNode::writeBinary(std::ostream& _stream) const
{
    clsOFStreamExtended& Stream = static_cast<clsOFStreamExtended&>(_stream);
    PosType_t StartPosition = Stream.tellp();
    PosType_t NullPosition = 0;
    Stream.write(this->Data->Children.size());
    for(auto Iterator = this->Data->Children.begin(); Iterator != this->Data->Children.end(); ++Iterator) {
        Stream.write(Iterator.key());
        Stream.write(NullPosition);
    }
    QMap<WordIndex_t, PosType_t> ChildPositions;
    this->Data->RuleNode.writeBinary(_stream);
    for(auto Iterator = this->Data->Children.begin(); Iterator != this->Data->Children.end(); ++Iterator) {
        ChildPositions[Iterator.key()] = Stream.tellp();
        Iterator->writeBinary(_stream);
    }
    PosType_t EndPosition = Stream.tellp();
    Stream.seekp(StartPosition + sizeof(int), std::ios_base::beg);
    for(auto Iterator = this->Data->Children.begin(); Iterator != this->Data->Children.end(); ++Iterator) {
        Stream.seekp(sizeof(WordIndex_t), std::ios_base::cur);
        Stream.write(ChildPositions[Iterator.key()]);
    }
    Stream.seekp(EndPosition, std::ios_base::beg);
}

void clsPrefixTree::readBinary(std::istream& _stream)
{
    clsIFStreamExtended* pStream = static_cast<clsIFStreamExtended*>(&_stream);
    this->RootNode = clsPrefixTreeNode(pStream);
}

}
}
}
}
