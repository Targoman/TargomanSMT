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
namespace TargomanLM {

const QString BIN_FILE_HEADER = "TargomanLMBin";

using namespace Private;
using namespace Targoman::Common::Configuration;

tmplConfigurable<FilePath_t> clsLanguageModel::FilePath(
        "/TargomanLM/FilePath",
        "File path of ARPA or binary models",
        "",
        Validators::tmplPathAccessValidator<
        (enuPathAccess::Type)(enuPathAccess::File | enuPathAccess::Readable)>);

tmplConfigurable<double>  clsLanguageModel::DeafultUnknownProb(
        "/TargomanLM/DeafultUnknownProb",
        "Default value for unknown words probability when not defined in model file",
        -100);
tmplConfigurable<double>  clsLanguageModel::DeafultUnknownBackoff(
        "/TargomanLM/DeafultUnknownBackoff",
        "Default value for unknown words backoff when not defined in model file",
        0);
Targoman::Common::Configuration::tmplConfigurable<bool> clsLanguageModel::UseIndexBasedModel(
        "/TargomanLM/UseIndexBasedModel",
        "Whether to use Index-Based or String-Based model",
        true);
Targoman::Common::Configuration::tmplConfigurable<bool> clsLanguageModel::VerifyBinaryChecksum(
        "/TargomanLM/VerifyBinaryChecksum",
        "Whether to verify checksum on binary files or not",
        true);

clsLanguageModel::clsLanguageModel() :
    pPrivate(new clsLanguageModelPrivate)
{
}

clsLanguageModel::~clsLanguageModel()
{
    //Just to suppress Compiler error when using QScopped Poiter
}

quint8 clsLanguageModel::init(bool _justVocab)
{
    return this->init(clsLanguageModel::FilePath.value(),
                      stuLMConfigs(
                          clsLanguageModel::DeafultUnknownProb.value(),
                          clsLanguageModel::DeafultUnknownBackoff.value(),
                          clsLanguageModel::UseIndexBasedModel.value(),
                          clsLanguageModel::VerifyBinaryChecksum.value()),
                      _justVocab);
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

quint8 clsLanguageModel::init(const QString &_filePath, const stuLMConfigs &_configs, bool _justVocab)
{
    if (this->pPrivate->FullyInitialized)
        return this->pPrivate->Order;

    if (this->pPrivate->isBinary(_filePath)){
        this->pPrivate->WasBinary = true;
        if (this->pPrivate->Model == NULL){
            if (_configs.UseIndexBasedModel)
                this->pPrivate->Model.reset(new clsIndexBasedProbingModel());
            else
                this->pPrivate->Model.reset(new clsStringBasedProbingModel());
        }

        this->pPrivate->Model->setUnknownWordDefaults(_configs.UnknownWordDefault.Prob,
                                                      _configs.UnknownWordDefault.Backoff);

        this->pPrivate->Order = this->pPrivate->Model->loadBinFile(_filePath, _configs.VerifyBinaryCheckSum);

        LM_BEGIN_SENTENCE_WINDEX = this->pPrivate->Model->getID(LM_BEGIN_SENTENCE);
        LM_END_SENTENCE_WINDEX   = this->pPrivate->Model->getID(LM_END_SENTENCE);
        this->pPrivate->FullyInitialized = true;
    }else{
        this->pPrivate->WasBinary = false;
        if (this->pPrivate->Model == NULL){
            if (_configs.UseIndexBasedModel)
                this->pPrivate->Model.reset(new clsIndexBasedProbingModel());
            else
                this->pPrivate->Model.reset(new clsStringBasedProbingModel());

            this->pPrivate->Model->setUnknownWordDefaults(_configs.UnknownWordDefault.Prob,
                                                          _configs.UnknownWordDefault.Backoff);
            this->pPrivate->Order = ARPAManager::instance().load(_filePath, *this->pPrivate->Model, _justVocab);

            LM_BEGIN_SENTENCE_WINDEX = this->pPrivate->Model->getID(LM_BEGIN_SENTENCE);
            LM_END_SENTENCE_WINDEX   = this->pPrivate->Model->getID(LM_END_SENTENCE);
            if(_justVocab == false)
                this->pPrivate->FullyInitialized = true;
        }else{
            if (_justVocab)
                return this->pPrivate->Order;
            this->pPrivate->Order = ARPAManager::instance().load(_filePath, *this->pPrivate->Model, false, true);
            this->pPrivate->FullyInitialized = true;
        }
    }

    return this->pPrivate->Order;
}

void clsLanguageModel::convertBinary(const QString &_binFilePath)
{
    if (!this->pPrivate->FullyInitialized)
        throw exLanguageModel("Seems that LM has not been fully initialized");
    if (this->pPrivate->WasBinary)
        throw exLanguageModel("LM has been loaded from a bin file so can not be written again");

    this->pPrivate->Model->saveBinFile(_binFilePath, this->pPrivate->Order);
}

quint8 clsLanguageModel::order() const
{
    return this->pPrivate->Order;
}

WordIndex_t clsLanguageModel::getID(const QString &_word) const
{
    return this->pPrivate->Model->getID(_word.toUtf8().constData());
}

QString clsLanguageModel::getWordByID(WordIndex_t _wordIndex) const
{
    return this->pPrivate->Model->getWordByID(_wordIndex);
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
    this->FullyInitialized = false;
}

clsLanguageModelPrivate::~clsLanguageModelPrivate()
{
    //Just to Suppress Compiler error using QScopped Pointer
}

bool clsLanguageModelPrivate::isBinary(const QString &_filePath)
{
    QFile BinFile(_filePath);
    if (BinFile.open(QFile::ReadOnly) == false)
        throw exLanguageModel("Unable to open <" + _filePath + "> For reading");

    try{
        return (BinFile.read(BIN_FILE_HEADER.size()) == BIN_FILE_HEADER.toLatin1());
    }catch(...){
        throw exLanguageModel("Unable to read from: " + _filePath);
    }
}

}
}
}


