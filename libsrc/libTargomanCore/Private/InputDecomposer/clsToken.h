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

#ifndef TARGOMAN_CORE_PRIVATE_INPUTDECOMPOSER_CLSTOKEN_H
#define TARGOMAN_CORE_PRIVATE_INPUTDECOMPOSER_CLSTOKEN_H


#include <QString>
#include <QHash>
#include "libTargomanCommon/Types.h"


namespace Targoman {
namespace Core {
namespace Private {
namespace InputDecomposer {

class clsToken
{
public:
    clsToken(const QString& _value,
             const Common::WordIndex_t _wordIndex,
             const QString& _tagStr = "",
             const QHash<QString, QString>& _attrs = QHash<QString, QString>()){
        this->Value = _value;
        this->WordIndex = _wordIndex;
        this->TagStr = _tagStr;
        this->Attrs = _attrs;
    }

    inline const QString& value() const {return this->Value;}
    inline const Common::WordIndex_t& wordIndex() const {return this->WordIndex;}
    inline const QString& tagStr() const {return this->TagStr;}
    inline const QHash<QString, QString>& attrs() const {return this->Attrs;}

private:
    QString Value;
    Common::WordIndex_t WordIndex;
    QString TagStr;
    QHash<QString, QString> Attrs;
};


}
}
}
}

#endif // TARGOMAN_CORE_PRIVATE_INPUTDECOMPOSER_CLSTOKEN_H
