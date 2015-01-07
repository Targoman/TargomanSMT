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
 */

#ifndef TARGOMAN_CORE_PRIVATE_INPUT_CLSINPUT_H
#define TARGOMAN_CORE_PRIVATE_INPUT_CLSINPUT_H

#include <QList>
#include <QSet>
#include "libTargomanCore/Configurations.h"

namespace Targoman {
namespace Core {
namespace Private {
namespace Input {

TARGOMAN_ADD_EXCEPTION_HANDLER(exInput, exTargomanCore);

class clsToken;

class clsInput
{
public:
    clsInput();

    static void init(const QString& _tags = "", const QString& _separator = "");

    void parsePlain(const QString& _inputStr, const QString& _lang = "");
    void parseRichIXML(const QString& _inputIXML);
    void parseRichIXML(const QString& _inputIXML, bool _normalize, const QString &_lang = "");

    void clear();
private:
    inline bool isSpace(const QChar& _ch){
        return _ch == ' ';// || _ch == '\n' || _ch == '\t';
    }

private:
    QList<clsToken*> Tokens;
    static QSet<QString>    SpecialTags;
};

}
}
}
}
#endif // TARGOMAN_CORE_PRIVATE_INPUT_CLSINPUT_H
