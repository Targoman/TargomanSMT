/*************************************************************************
 * Copyright © 2012-2015, Targoman.com
 *
 * Published under the terms of TCRL(Targoman Community Research License)
 * You can find a copy of the license file with distributed source or
 * download it from http://targoman.com/License.txt
 *
 *************************************************************************/
/**
  @author S. Mohammad M. Ziabary <smm@ziabary.com>
 @author Behrooz Vedadian <vedadian@gmail.com>
 */


#include "clsLanguageModel.h"
#include "Private/clsLanguageModel_p.h"
#include "Private/ARPAManager.h"
#include "libTargomanCommon/Configuration/Validators.hpp"

/// Different Language Models
#include "Private/clsStringBasedProbingModel.h"
#include "Private/clsIndexBasedProbingModel.h"


using namespace Targoman::Common;

namespace Targoman {
namespace NLPLibs {

using namespace Private;
using namespace Targoman::Common::Configuration;

tmplConfigurable<QString> clsLanguageModel::FilePath("/TargomanLM/FilePath",
                                                     "File path of ARPA or binary models",
                                                     "",
                                                     Validators::tmplPathAccessValidator<
                                                     (enuPathAccess::Type)(enuPathAccess::File | enuPathAccess::Readable)>);

tmplConfigurable<double>  clsLanguageModel::DeafultUnknownProb("/TargomanLM/DeafultUnknownProb",
                                                               "Default value for unknown words probability when not defined in model file",
                                                               -100);
tmplConfigurable<double>  clsLanguageModel::DeafultUnknownBackoff("/TargomanLM/DeafultUnknownBackoff",
                                                                  "Default value for unknown words backoff when not defined in model file",
                                                                  0);
Targoman::Common::Configuration::tmplConfigurable<bool> clsLanguageModel::UseIndexBasedModel("/TargomanLM/UseIndexBasedModel",
                                                                                             "Wheter use Index-Based or String-Based model",
                                                                                             true);

clsLanguageModel::clsLanguageModel() :
    pPrivate(new clsLanguageModelPrivate)
{
}

clsLanguageModel::~clsLanguageModel()
{
    //Just to suppress Compiler error when using QScopped Poiter
}

quint8 clsLanguageModel::init()
{
    return this->init(clsLanguageModel::FilePath.value(),
                      stuLMConfigs(
                          clsLanguageModel::DeafultUnknownProb.value(),
                          clsLanguageModel::DeafultUnknownBackoff.value(),
                          clsLanguageModel::UseIndexBasedModel.value()));
}

/**
 * @brief Initialize and instantiates a model.
 *
 * If our model is already initialized, it just returns the order of language model.
 * This function decides to instantiates language model between index based or string based probing model based on UseIndexBasedModel data memeber of input #_configs.
 * Using ARPAManager class, this function loads language model file and initializes the model data member of #pPrivate.
 * Finally, this function sets unknown word default probability and backoff.
 *
 *
 * @param[in] _filePath Address of language model file.
 * @param[in] _configs  Structure to set unknownWordDefault probablity and backoffs. This structure also decides between index and string based language model.
 * @return return order of NGram.
 */

quint8 clsLanguageModel::init(const QString &_filePath, const stuLMConfigs &_configs)
{
    if (this->pPrivate->Initialized)
        return this->pPrivate->Order;

    if (this->pPrivate->isBinary(_filePath)){
        throw exTargomanNotImplemented("Binary is Not implemented yet");
    }else{
        if (_configs.UseIndexBasedModel)
            this->pPrivate->Model = new clsIndexBasedProbingModel();
        else
            this->pPrivate->Model = new clsStringBasedProbingModel();

        this->pPrivate->Order = ARPAManager::instance().load(_filePath, this->pPrivate->Model);
        this->pPrivate->Model->setUnknownWordDefaults(_configs.UnknownWordDefault.Prob,
                                                      _configs.UnknownWordDefault.Backoff);

        LM_BEGIN_SENTENCE_WINDEX = this->pPrivate->Model->getID(LM_BEGIN_SENTENCE);
        LM_END_SENTENCE_WINDEX   = this->pPrivate->Model->getID(LM_END_SENTENCE);
    }
    return this->pPrivate->Order;
}

void clsLanguageModel::convertBinary(enuMemoryModel::Type _model, const QString &_binFilePath)
{
    Q_UNUSED(_model);
    Q_UNUSED(_binFilePath);
}

quint8 clsLanguageModel::order() const
{
    return this->pPrivate->Order;
}

WordIndex_t clsLanguageModel::getID(const QString &_word) const
{
    return this->pPrivate->Model->getID(_word.toUtf8().constData());
}

/**
 * @brief returns probablity of input NGram and maximum order of founded NGram.
 * @param[in] _ngram                    input NGram in a list of QString.
 * @param[out] _foundedGram maximum     order of NGram that was existed in Hash Table.
 * @return probablity of input NGram.
 */

LogP_t clsLanguageModel::lookupNGram(const QStringList & _ngram, quint8& _foundedGram) const
{
    return this->pPrivate->Model->lookupNGram(_ngram, _foundedGram);
}

LogP_t clsLanguageModel::lookupNGram(const QList<WordIndex_t> & _ngram, quint8& _foundedGram) const
{
    return this->pPrivate->Model->lookupNGram(_ngram, _foundedGram);
}

//////////////////////////////////////////////////////////////////////////////////////////////
const char* LM_UNKNOWN_WORD = "<unk>";
const char* LM_BEGIN_SENTENCE = "<s>";
const char* LM_END_SENTENCE = "</s>";

//Defined here initialized in Private/ArpaManager.hpp
WordIndex_t LM_UNKNOWN_WINDEX;
WordIndex_t LM_BEGIN_SENTENCE_WINDEX;
WordIndex_t LM_END_SENTENCE_WINDEX;
//////////////////////////////////////////////////////////////////////////////////////////////

Private::clsLanguageModelPrivate::clsLanguageModelPrivate()
{
    this->Initialized = false;
}

bool clsLanguageModelPrivate::isBinary(const QString &_file)
{
    Q_UNUSED(_file)
    return false;
}

}
}

