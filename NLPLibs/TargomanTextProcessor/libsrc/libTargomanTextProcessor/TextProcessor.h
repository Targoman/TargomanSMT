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

#ifndef TARGOMAN_NLPLIBS_TARGOMANTP_TEXTPROCESSOR_H
#define TARGOMAN_NLPLIBS_TARGOMANTP_TEXTPROCESSOR_H

#include <QString>
#include "libTargomanCommon/Macros.h"
#include "libTargomanCommon/exTargomanBase.h"
#include "libTargomanCommon/Logger.h"
#include "ISO639.h" //From https://github.com/softnhard/ISO639

namespace Targoman {
namespace NLPLibs {

TARGOMAN_DEFINE_ENHANCED_ENUM(enuTextTags,
                              Number,
                              SpecialNumber,
                              Email,
                              URL,
                              Abbreviation,
                              OrderedListItem,
                              Time,
                              Date,
                              Ordinals,
                              Symbol
                              )
TARGOMAN_ADD_EXCEPTION_HANDLER(exTextProcessor, Targoman::Common::exTargomanBase);

//Used by Logger Methods
extern QString ActorUUID;

class TargomanTextProcessor
{
public:
    struct stuConfigs{
        QString NormalizationFile;
        QString AbbreviationsFile;
        QString SpellCorrectorBaseConfigPath;
        QHash<QString, QVariantHash> SpellCorrectorLanguageBasedConfigs;
    };

public:
    static inline TargomanTextProcessor& instance(){
        return *(Q_LIKELY(Instance) ? Instance : (Instance = new TargomanTextProcessor));}

    bool init(const stuConfigs &_configs);
    bool init(const QString _configFile = "");

    QString text2IXML(const QString& _inStr,
                      const QString& _lang = "",
                      quint32 _lineNo = 0,
                      bool _interactive = true,
                      bool _useSpellCorrector = true,
                      QList<enuTextTags::Type> _removingTags = QList<enuTextTags::Type>()) const;
    QString text2RichIXML(const QString& _inStr, const QString& _lang = "") const;

    QString ixml2Text(const QString& _ixml) const;
    QString richIXML2Text(const QString& _ixml) const;

    QString normalizeText(const QString _input,  bool _interactive = false, const QString& _lang = "") const;

private:
    TargomanTextProcessor();
    Q_DISABLE_COPY(TargomanTextProcessor)
private:
    static TargomanTextProcessor* Instance;
};


}
}

#endif // TARGOMAN_NLPLIBS_TARGOMANTP_TEXTPROCESSOR_H
