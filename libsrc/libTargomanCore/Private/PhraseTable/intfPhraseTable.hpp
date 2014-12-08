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

#ifndef TARGOMAN_CORE_PRIVATE_PHRASETABLE_INTFPHRASETABLE_H
#define TARGOMAN_CORE_PRIVATE_PHRASETABLE_INTFPHRASETABLE_H

#include "Configurations.h"
#include "libTargomanCommon/Macros.h"
#include "libTargomanCommon/tmplExpirableCache.h"
#include "Private/Input/clsInput.h"

namespace Targoman {
namespace Core {
namespace Private {
namespace PhraseTable{

TARGOMAN_ADD_EXCEPTION_HANDLER(exPhraseTable, exTargomanCore);

TARGOMAN_DEFINE_ENHANCED_ENUM_BEGIN(enuPTFileFormat)
    Moses,
    Hiero,
    Compact,
    Binary
TARGOMAN_DEFINE_ENHANCED_ENUM_STRINGS
    "Moses",
    "Hiero",
    "Compact",
    "Binary"
TARGOMAN_DEFINE_ENHANCED_ENUM_END

//Reimplementation of PhraseDictionary class in moses
class intfPhraseTable
{
public:
    intfPhraseTable(){}

    virtual void load() = 0;
    virtual void initializeForInput(Input::clsInput const& _source){
        Q_UNUSED(_source)
    }

    virtual void cleanUpAfterSentenceProcessing(Input::clsInput const& _source){
        Q_UNUSED(_source)
    }

    //it was named SetParameter in moses
    virtual void init(){
        this->CacheSize  = Targoman::Core::gConfigs.getConfig("CacheSize").toUInt();
        this->FilePath   = Targoman::Core::gConfigs.getConfig("FilePath").toString();
        this->TableLimit = Targoman::Core::gConfigs.getConfig("TableLimit").toUInt();
    }

    static void registerConfigs(const QString& _configPath){
        Targoman::Core::gConfigs.addConfig(_configPath + "/CacheSize" , QVariant::UInt  , (quint32)DEFAULT_MAX_TRANS_OPT_CACHE_SIZE);
        Targoman::Core::gConfigs.addConfig(_configPath + "/FilePath"  , QVariant::String, "");
        Targoman::Core::gConfigs.addConfig(_configPath + "/TableLimit", QVariant::UInt  , 20);
    }

protected:
    //bool satisfyBackoff(const InputPath &inputPath)

protected:
    size_t  CacheSize;
    QString FilePath;
    size_t  TableLimit;

    //Targoman::Common::tmplExpirableCache<QString, TargetPhraseCollection> Cache;

    //TODO m_id and s_staticColl must be implemented if multiple PT is needed
};

}
}
}
}

#endif // TARGOMAN_CORE_PRIVATE_PHRASETABLE_INTFPHRASETABLE_H
