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


#include "clsLanguageModel.h"
#include "Private/clsLanguageModel_p.h"
#include "Private/ARPAManager.h"

/// Different Language Models
#include "Private/clsProbingModel.h"


namespace Targoman {
namespace NLPLibs {

using namespace Private;

clsLanguageModel::clsLanguageModel() :
    pPrivate(new clsLanguageModelPrivate)
{
}

clsLanguageModel::~clsLanguageModel()
{
    //Just to suppress Compiler error when using QScopped Poiter
}

quint8 clsLanguageModel::init(const QString &_filePath, const stuLMConfigs &_configs)
{
    if (this->pPrivate->isBinary(_filePath)){

    }else{
        this->pPrivate->Model = new clsProbingModel(new clsVocab);
        this->pPrivate->Model->setUnknownWordDefaults(_configs.UnknownWordDefault.Prob, _configs.UnknownWordDefault.Backoff);
        this->pPrivate->Order = ARPAManager::instance().load(_filePath, this->pPrivate->Model);
    }
    return this->pPrivate->Order;
}

void clsLanguageModel::convertBinary(enuMemoryModel::Type _model, const QString &_binFilePath)
{

}

quint8 clsLanguageModel::order() const
{
    return this->pPrivate->Order;
}

WordIndex_t clsLanguageModel::getIndex(const QString &_word) const
{
    return this->pPrivate->Model->vocab().getIndex(_word);
}

LogP_t clsLanguageModel::lookupNGram(QList<WordIndex_t> &_ngram, quint8& _foundedGram) const
{
    return this->pPrivate->Model->lookupNGram(_ngram, _foundedGram);
}

//////////////////////////////////////////////////////////////////////////////////////////////
//Defined here initialized in Private/Vocab.hpp
WordIndex_t LM_UNKNOWN_WINDEX;
WordIndex_t LM_BEGIN_SENTENCE_WINDEX;
WordIndex_t LM_END_SENTENCE_WINDEX;
//////////////////////////////////////////////////////////////////////////////////////////////

Private::clsLanguageModelPrivate::clsLanguageModelPrivate()
{
}

bool clsLanguageModelPrivate::isBinary(const QString &_file)
{
    return false;
}

}
}

