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

#include <QHash>
#include <QStringList>
#include "../Definitions.h"

namespace Targoman {
namespace NLPLibs {
namespace Private {

class clsVocab
{
public:
    clsVocab(){
        //Following three are defined as static/constant Keywords in Definitions.h
        LM_UNKNOWN_WINDEX           = this->addWord(LM_UNKNOWN_WORD);
        LM_BEGIN_SENTENCE_WINDEX    = this->addWord(LM_BEGIN_SENTENCE);
        LM_END_SENTENCE_WINDEX      = this->addWord(LM_END_SENTENCE);
    }

    inline WordIndex_t getIndex(const QString& _word) const {
        return this->WordIndexes.value(_word, -1);
    }

    inline const QString& getWord(const WordIndex_t _index) const{
        return Q_UNLIKELY(_index < 0 || _index > this->Words.size()) ?
                    LM_UNKNOWN_WORD : this->Words.at(_index);
    }

    inline WordIndex_t addWord(const QString& _word){
        WordIndex_t Index = this->getIndex(_word);
        if (Index >= 0)
            return Index;

        this->Words.append(_word);
        this->WordIndexes.insert(_word, this->Words.size() - 1);
        return this->Words.size() - 1;
    }

private:
    Q_DISABLE_COPY(clsVocab)

private:
    QHash<QString, WordIndex_t> WordIndexes;
    QStringList Words;
};

}
}
}

#endif // TARGOMAN_NLPLIBS_PRIVATE_CLSVOCAB_HPP
