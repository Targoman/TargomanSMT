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

#ifndef TARGOMAN_NLPLIBS_CLSLANGUAGEMODEL_H
#define TARGOMAN_NLPLIBS_CLSLANGUAGEMODEL_H

#include "libTargomanCommon/exTargomanBase.h"
#include "libTargomanCommon/Macros.h"
#include "libLanguageModel/Definitions.h"

namespace Targoman {
namespace NLPLibs {

namespace Private {
    class clsLanguageModelPrivate;
}

class clsLanguageModel
{
public:
    clsLanguageModel();
    ~clsLanguageModel();

    quint8 init(const QString& _filePath, const stuLMConfigs& _configs = stuLMConfigs());

    void convertBinary(enuMemoryModel::Type _model, const QString& _binFilePath);

    quint8  order() const;
    Targoman::Common::WordIndex_t getIndex(const char *_word) const;

    Targoman::Common::LogP_t lookupNGram(const QStringList &_ngram, quint8& _foundedGram) const ;


private:
    QScopedPointer<Private::clsLanguageModelPrivate> pPrivate;
};


}
}

#endif // TARGOMAN_NLPLIBS_CLSLANGUAGEMODEL_H
