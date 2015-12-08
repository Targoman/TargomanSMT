/******************************************************************************
 * Targoman: A robust Statistical Machine Translation framework               *
 *                                                                            *
 * Copyright 2014-2015 by ITRC <http://itrc.ac.ir>                            *
 *                                                                            *
 * This file is part of Targoman.                                             *
 *                                                                            *
 * Targoman is free software: you can redistribute it and/or modify           *
 * it under the terms of the GNU Lesser General Public License as published   *
 * by the Free Software Foundation, either version 3 of the License, or       *
 * (at your option) any later version.                                        *
 *                                                                            *
 * Targoman is distributed in the hope that it will be useful,                *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of             *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              *
 * GNU Lesser General Public License for more details.                        *
 * You should have received a copy of the GNU Lesser General Public License   *
 * along with Targoman. If not, see <http://www.gnu.org/licenses/>.           *
 *                                                                            *
 ******************************************************************************/
/**
 * @author S. Mohammad M. Ziabary <ziabary@targoman.com>
 * @author Behrooz Vedadian <vedadian@targoman.com>
 */

// There is no NER for anything but Statistical Machine Translation!
#ifndef SMT

#include "ZhangMaxEntProxy.h"

namespace Targoman {
namespace SMT {
namespace Private {
namespace Proxies {
namespace NamedEntityRecognition {

using namespace Common::Configuration;
using namespace InputDecomposer;
using namespace std;

tmplConfigurable<FilePath_t> ZhangMaxEntProxy::FilePath(
        MAKE_CONFIG_PATH("FilePath"),
        "File path of binary maxent model",
        ""
        );
maxent::MaxentModel ZhangMaxEntProxy::Model;
bool ZhangMaxEntProxy::ModelLoaded = false;

ZhangMaxEntProxy::ZhangMaxEntProxy()
{ }

void ZhangMaxEntProxy::init()
{
    if(ZhangMaxEntProxy::ModelLoaded)
        throw exTargomanInitialization("Initialization of ZhangMaxEntProxy must be called just once.");

    ZhangMaxEntProxy::Model.load(ZhangMaxEntProxy::FilePath.value().toStdString());
    ZhangMaxEntProxy::ModelLoaded = true;
}

QVector<vector<string>> ZhangMaxEntProxy::getMaxEntContexts(Sentence_t _sentence)
{
    Q_UNUSED(_sentence);
    return  QVector<vector<string>>();
}

void ZhangMaxEntProxy::tagNamedEntities(Sentence_t _sentence)
{
    QVector<vector<string>> Contexts =
            getMaxEntContexts(_sentence);
    for(int i = 0; i < _sentence.size(); ++i)
        intfNamedEntityRecognizer::addTokenAttr(
                        _sentence[i],
                        NER_TAG_ATTR_KEY,
                        QString::fromStdString(ZhangMaxEntProxy::Model.predict(Contexts.at(i)))
                        );
}

}
}
}
}
}

#endif
