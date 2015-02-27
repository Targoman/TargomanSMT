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

#ifndef TARGOMAN_CORE_PRIVATE_CLSTRANSLATOR_P_H
#define TARGOMAN_CORE_PRIVATE_CLSTRANSLATOR_P_H

#include <QScopedPointer>
#include "InputDecomposer/clsInput.h"
#include "SearchGraph/clsSearchGraphBuilder.h"
#include "N-BestFinder/clsNBestFinder.h"
#include "OutputComposer/clsOutputComposer.h"

namespace Targoman{
namespace Core {
namespace Private{

class clsTranslatorPrivate{
public:
    clsTranslatorPrivate(const QString & _inputStr) :
        Input(new InputDecomposer::clsInputDecomposer (_inputStr)),
        SearchGraphBuilder(new SearchGraph::clsSearchGraphBuilder(Input->tokens())),
        NBest(new NBestFinder::clsNBestFinder(*this->SearchGraphBuilder)),
        Output(new OutputComposer::clsOutputComposer(*this->Input, *this->NBest))
    {}

    ~clsTranslatorPrivate(){}

public:
    QScopedPointer<InputDecomposer::clsInputDecomposer> Input;
    QScopedPointer<SearchGraph::clsSearchGraphBuilder> SearchGraphBuilder;
    QScopedPointer<NBestFinder::clsNBestFinder> NBest;
    QScopedPointer<OutputComposer::clsOutputComposer> Output;

    static bool Initialized;
};

}
}
}
#endif // TARGOMAN_CORE_PRIVATE_CLSTRANSLATOR_P_H
