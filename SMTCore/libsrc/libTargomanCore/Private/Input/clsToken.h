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
 */

#ifndef TARGOMAN_CORE_PRIVATE_INPUT_CLSTOKEN_H
#define TARGOMAN_CORE_PRIVATE_INPUT_CLSTOKEN_H


#include <QString>
#include <QHash>

namespace Targoman {
namespace Core {
namespace Private {
namespace Input {

class clsToken
{
public:
    clsToken(const QString& _value, const QString& _tagStr = "", const QHash<QString, QString>& _attrs = QHash<QString, QString>()){
        this->Value = _value;
        this->TagStr = _tagStr;
        this->Attrs = _attrs;
    }

private:
    QString Value;
    QString TagStr;
    QHash<QString, QString> Attrs;
};

}
}
}
}

#endif // TARGOMAN_CORE_PRIVATE_INPUT_CLSTOKEN_H
