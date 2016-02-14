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

TARGOMAN_REGISTER_SINGLETON_MODULE(ZhangMaxEntProxy);

tmplConfigurable<FilePath_t> ZhangMaxEntProxy::FilePath(
        MAKE_CONFIG_PATH("FilePath"),
        "File path of binary maxent model. Relative to config file path unless specified as absolute path.",
        ""
        );
tmplConfigurable<FilePath_t> ZhangMaxEntProxy::RareWordsPath(
        MAKE_CONFIG_PATH("RareWordsPath"),
        "Path to the file containing rare words. Relative to config file path unless specified as absolute path.",
        ""
        );
maxent::MaxentModel ZhangMaxEntProxy::Model;
bool ZhangMaxEntProxy::ModelLoaded = false;
QStringList ZhangMaxEntProxy::RareWords;

ZhangMaxEntProxy::ZhangMaxEntProxy()
{ }

QStringList readalllines(string _filename)
{
    QStringList AllLines;
    ifstream File(_filename);

    string Line;
    while(getline(File, Line))
        AllLines.append(QString::fromStdString(Line));
    return AllLines;
}

void ZhangMaxEntProxy::init(QSharedPointer<QSettings> _configSettings)
{
    Q_UNUSED(_configSettings);
    if(ZhangMaxEntProxy::ModelLoaded)
        throw exTargomanInitialization("Initialization of ZhangMaxEntProxy must be called just once.");

    ZhangMaxEntProxy::Model.load(ZhangMaxEntProxy::FilePath.value().toStdString());
    ZhangMaxEntProxy::RareWords = readalllines(ZhangMaxEntProxy::RareWordsPath.value().toStdString());
    ZhangMaxEntProxy::ModelLoaded = true;
}

vector<string> ZhangMaxEntProxy::getMaxEntContext(
        const QList<InputDecomposer::clsToken::stuInfo>& _sentence,
        const QStringList &_previousTags,
        int _index)
{
    // Converted directly from Zhang's maxent implementation example, postagger.py
    // so do not blame me!
    auto get_prefix_suffix_english =
            [] (QString w, int length, QStringList& p, QStringList& s) {
        p.clear();
        s.clear();
        int wl = w.size();
        int l = qMin(wl, length + 1);
        for(int i = 0; i < l; ++i) {
            p.append(w.mid(0, i + 1));
            s.append(w.mid(wl - i - 1, i + 1));
        }
    };
    static QRegExp re_number = QRegExp("[0-9]");
    static QRegExp re_hyphen = QRegExp("-");
    static QRegExp re_uppercase = QRegExp("[A-Z]");
    auto get_context_english = [&] (const QList<InputDecomposer::clsToken::stuInfo>& words, const QStringList& pos, int i, bool rare_word) {
        QList<QString> context;
        QString w = words[i].Str;
        int n = words.size();
        if(rare_word) {
            QStringList prefix, suffix;
            get_prefix_suffix_english(w, 4, prefix, suffix);
            foreach(const QString& p, prefix)
                context.append("prefix=" + p);
            foreach(const QString& s, suffix)
                context.append("suffix=" + s);
            if(w.contains(re_number))
                context.append("numeric");
            if(w.contains(re_uppercase))
                context.append("uppercase");
            if(w.contains(re_hyphen))
                context.append("hyphen");
        } else {
            context.append("curword=" + w);
        }

        if(i > 0) {
            context.append("word-1=" + words[i - 1].Str);
            context.append("tag-1=" + pos[i - 1]);
            if(i > 1) {
                context.append("word-2=" + words[i - 2].Str);
                context.append("tag-1,2=" + pos[i - 2] + "," + pos[i - 1]);
            } else {
                context.append("word-2=BOUNDARY");
                context.append("tag-1,2=BOUNDARY," + pos[0]);
            }
        } else {
            context.append("word-1=BOUNDARY");
            context.append("word-2=BOUNDARY");
            context.append("tag-1=BOUNDARY");
            context.append("tag-1,2=BOUNDARY,BOUNDARY");
        }

        if(i + 1 < n) {
            context.append("word+1=" + words[i + 1].Str);
            if(i + 2 < n)
                context.append("word+2=" + words[i + 2].Str);
            else
                context.append("word+2=BOUNDARY");
        } else {
            context.append("word+1=BOUNDARY");
            context.append("word+2=BOUNDARY");
        }

        vector<string> stdcontext;
        stdcontext.resize(context.size());
        for(int i = 0; i < context.size(); ++i)
            stdcontext[i] = context[i].toStdString();

        return stdcontext;
    };

    return get_context_english(_sentence, _previousTags,
                               _index,
                               ZhangMaxEntProxy::RareWords.contains(
                                   _sentence[_index].Str
                                   )
                               );
}

void ZhangMaxEntProxy::tagNamedEntities(QList<InputDecomposer::clsToken::stuInfo>& _sentence)
{
    QStringList PreviousTags;
    for(int i = 0; i < _sentence.size(); ++i) {
        vector<string> Context = this->getMaxEntContext(
                    _sentence,
                    PreviousTags,
                    i
                    );
        QString CurrentTag = QString::fromStdString(
                    ZhangMaxEntProxy::Model.predict(Context)
                    );
        PreviousTags.append(CurrentTag);
        _sentence[i].Attrs.insert(NER_TAG_ATTR_KEY, CurrentTag);
    }
}

}
}
}
}
}

#endif
