/*************************************************************************
 * Copyright © 2012-2014, Targoman.com
 *
 * Published under the terms of TCRL(Targoman Community Research License)
 * You can find a copy of the license file with distributed source or
 * download it from http://targoman.com/License.txt
 *
 *************************************************************************/
/**
  @author S. Mohammad M. Ziabary <smm@ziabary.com>
  @author Behrooz Vedadian <vedadian@aut.ac.ir>
 */

#ifndef TARGOMAN_NLPLIBS_PRIVATE_CLSVOCAB_HPP
#define TARGOMAN_NLPLIBS_PRIVATE_CLSVOCAB_HPP

#ifdef LM_USE_GOOGLE_SPARSE_HASH
#include <google/dense_hash_map>
#endif

#include <QHash>
#include <QStringList>
#include "../Definitions.h"



//#define STDMAP
//#include <unordered_map>

using namespace Targoman::Common;

namespace Targoman {
namespace NLPLibs {
namespace Private {
const uint PrimeNumbers2[] = { 13, 5, 53, 17, 25, 19, 31, 37, 41, 43, 19, 53, 11, 3, 7 };

class clsMyHasher{
public :

    uint operator()(const std::string& _key) const{
        uint A = qHash(QString(_key.c_str()));
        qDebug()<<"'"<<_key.c_str()<<"'"<<"-->"<<A;
      return A;
    }
};
class clsVocab
{

public:
    clsVocab(){
#ifdef LM_USE_GOOGLE_SPARSE_HASH
        this->WordIndexes.set_empty_key(" ");
        qDebug()<<"\\n will be added";
        this->WordIndexes["\n"] = 0;
        qDebug()<<"\\n added";
#endif
        //Following three are defined as static/constant Keywords in Definitions.h
        //LM_UNKNOWN_WINDEX           = this->addWord(LM_UNKNOWN_WORD);
        this->WordIndexes[LM_UNKNOWN_WORD] = 1;
        qDebug()<<LM_UNKNOWN_WORD<<"-->"<<this->getIndex(LM_UNKNOWN_WORD)<<" : "<<LM_UNKNOWN_WINDEX;
        qDebug()<<LM_UNKNOWN_WORD<<"-->"<<this->getIndex(LM_UNKNOWN_WORD)<<" : "<<LM_UNKNOWN_WINDEX;
        qDebug()<<LM_UNKNOWN_WORD<<"-->"<<this->getIndex(LM_UNKNOWN_WORD)<<" : "<<LM_UNKNOWN_WINDEX;
        qDebug()<<LM_UNKNOWN_WORD<<"-->"<<this->getIndex(LM_UNKNOWN_WORD)<<" : "<<LM_UNKNOWN_WINDEX;

        //LM_BEGIN_SENTENCE_WINDEX    = this->addWord(LM_BEGIN_SENTENCE);
        this->WordIndexes[LM_BEGIN_SENTENCE] = 2;//(WordIndex_t)this->WordIndexes.size();
        qDebug()<<LM_UNKNOWN_WORD<<"-->"<<this->getIndex(LM_UNKNOWN_WORD)<<" : "<<LM_UNKNOWN_WINDEX;
        qDebug()<<LM_UNKNOWN_WORD<<"-->"<<this->getIndex(LM_UNKNOWN_WORD)<<" : "<<LM_UNKNOWN_WINDEX;

       // qDebug()<<LM_BEGIN_SENTENCE<<"-->"<<this->getIndex(LM_BEGIN_SENTENCE)<<" : "<<LM_BEGIN_SENTENCE_WINDEX;

        //LM_END_SENTENCE_WINDEX      = this->addWord(LM_END_SENTENCE);
        this->WordIndexes[LM_END_SENTENCE] = 3;//(WordIndex_t)this->WordIndexes.size();

        //qDebug()<<LM_END_SENTENCE<<"-->"<<this->getIndex(LM_END_SENTENCE)<<" : "<<LM_END_SENTENCE_WINDEX;

        qDebug()<<LM_UNKNOWN_WORD<<"-->"<<this->getIndex(LM_UNKNOWN_WORD)<<" : "<<LM_UNKNOWN_WINDEX;
        qDebug()<<LM_BEGIN_SENTENCE<<"-->"<<this->getIndex(LM_BEGIN_SENTENCE)<<" : "<<LM_BEGIN_SENTENCE_WINDEX;
        qDebug()<<LM_END_SENTENCE<<"-->"<<this->getIndex(LM_END_SENTENCE)<<" : "<<LM_END_SENTENCE_WINDEX;
        qDebug()<<LM_UNKNOWN_WORD<<"-->"<<this->getIndex(LM_UNKNOWN_WORD)<<" : "<<LM_UNKNOWN_WINDEX;
        qDebug()<<LM_BEGIN_SENTENCE<<"-->"<<this->getIndex(LM_BEGIN_SENTENCE)<<" : "<<LM_BEGIN_SENTENCE_WINDEX;
        qDebug()<<LM_END_SENTENCE<<"-->"<<this->getIndex(LM_END_SENTENCE)<<" : "<<LM_END_SENTENCE_WINDEX;
        qDebug()<<LM_UNKNOWN_WORD<<"-->"<<this->getIndex(LM_UNKNOWN_WORD)<<" : "<<LM_UNKNOWN_WINDEX;
        qDebug()<<LM_BEGIN_SENTENCE<<"-->"<<this->getIndex(LM_BEGIN_SENTENCE)<<" : "<<LM_BEGIN_SENTENCE_WINDEX;
        qDebug()<<LM_END_SENTENCE<<"-->"<<this->getIndex(LM_END_SENTENCE)<<" : "<<LM_END_SENTENCE_WINDEX;

exit(0);
    }

    inline WordIndex_t getIndex(const std::string& _word) const {
        auto Iter = this->WordIndexes.find(_word);

        return (Iter != this->WordIndexes.end()) ? Iter->second : -1;
    }

/*    inline WordIndex_t getIndex(const QString& _word) const {
        return this->WordIndexes.value(_word, -1);
    }*/

    inline const QString& getWord(const WordIndex_t _index) const{
    /*    return Q_UNLIKELY(_index < 0 || _index > this->Words.size()) ?
                    LM_UNKNOWN_WORD : this->Words.at(_index);*/
        return "";
    }

    inline WordIndex_t addWord(const std::string& _word){
        WordIndex_t Index = this->getIndex(_word);
        if (Index >= 0)
            return Index;

        Index = (WordIndex_t)this->WordIndexes.size();
        this->WordIndexes[_word] = Index;

        //return this->getIndex(_word);
        return Index;
    }

    /*inline WordIndex_t addWord(const QString& _word){
        WordIndex_t Index = this->getIndex(_word);
        if (Index >= 0)
            return Index;

     //   this->Words.append(_word);
#ifdef STDMAP
        this->WordIndexes.insert(_word.toUtf8().constData());
#else
        this->WordIndexes.insert(_word, this->WordIndexes.size());
#endif
        return this->WordIndexes.size() - 1;
    }*/

private:
    Q_DISABLE_COPY(clsVocab)

private:
#ifdef STDMAP
    std::unordered_map<std::string, WordIndex_t> WordIndexes;
#elif defined(LM_USE_GOOGLE_SPARSE_HASH)


    google::dense_hash_map<std::string, WordIndex_t, clsMyHasher> WordIndexes;
#else
    QHash<QString, WordIndex_t> WordIndexes;
#endif
    QStringList Words;
};

}
}
}

#endif // TARGOMAN_NLPLIBS_PRIVATE_CLSVOCAB_HPP
