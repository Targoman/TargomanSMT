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

#ifndef CLSTRANSLATOR_H
#define CLSTRANSLATOR_H

#include <QMap>
#include <QStringList>


struct stuTranslationOutput{
    struct stuCharacterRange{
        quint16 Start;
        quint16 End;
        //Todo Assert
    };

    struct stuTargetPhrase{
        QList<stuCharacterRange> Range;
        QStringList              TranslationOptions;
    };

    QString Translation;
    QMap<QList<stuCharacterRange>,  stuTargetPhrase> AlignInfo;

};


class clsTranslator
{
public:
    clsTranslator();


};

#endif // CLSTRANSLATOR_H
