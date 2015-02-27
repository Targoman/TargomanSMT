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

#ifndef TARGOMAN_NLPLIBS_LANGUAGEMODEL_CLSLANGUAGEMODEL_H
#define TARGOMAN_NLPLIBS_LANGUAGEMODEL_CLSLANGUAGEMODEL_H

#include "libTargomanCommon/exTargomanBase.h"
#include "libTargomanCommon/Macros.h"
#include "libTargomanLM/Definitions.h"
#include "libTargomanCommon/Configuration/tmplConfigurable.h"

namespace Targoman {
namespace NLPLibs {
namespace TargomanLM {

namespace Private {
    class clsLanguageModelPrivate;
}
/**
 * @brief This class manages loaded language model.
 */
class clsLanguageModel
{
public:
    clsLanguageModel();
    ~clsLanguageModel();

    quint8 init(bool _justVocab = false);

    quint8 init(const QString& _filePath, const stuLMConfigs& _configs = stuLMConfigs(), bool _justVocab = false);

    void convertBinary(const QString& _binFilePath);

    quint8  order() const;
    Common::WordIndex_t getID(const QString& _word) const;
    QString getWordByID(Common::WordIndex_t _wordIndex) const;

    Common::LogP_t lookupNGram(const QStringList & _ngram, quint8& _foundedGram) const;
    Common::LogP_t lookupNGram(const QList<Common::WordIndex_t> &_ngram, quint8& _foundedGram) const ;


private:
    QScopedPointer<Private::clsLanguageModelPrivate> pPrivate;

    static Targoman::Common::Configuration::tmplConfigurable<QString> FilePath;
    static Targoman::Common::Configuration::tmplConfigurable<double>  DeafultUnknownProb;
    static Targoman::Common::Configuration::tmplConfigurable<double>  DeafultUnknownBackoff;
    static Targoman::Common::Configuration::tmplConfigurable<bool>    UseIndexBasedModel;
    static Targoman::Common::Configuration::tmplConfigurable<bool>    VerifyBinaryChecksum;
};


}
}
}

#endif // TARGOMAN_NLPLIBS_LANGUAGEMODEL_CLSLANGUAGEMODEL_H
