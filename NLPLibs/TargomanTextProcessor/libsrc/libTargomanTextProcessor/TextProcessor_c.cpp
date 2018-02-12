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
 * @author Behrooz Vedadian <vedadian@targoman.com>
 */
 
#include <string.h>
#include <QtCore>
#include "TextProcessor.h"
#include "libTargomanCommon/Logger.h"

using namespace Targoman::Common;
using namespace Targoman::NLPLibs;

bool c_init(
    const char* _normalizationFilePath,
    const char* _abbreviationFilePath,
    const char* _spellCorrectorBaseConfigPath,
    const char* _language
    ) {

    Targoman::Common::TARGOMAN_IO_SETTINGS.setSilent();
    
    TargomanTextProcessor::stuConfigs Configs;

    Configs.NormalizationFile = QString::fromUtf8(_normalizationFilePath);
    Configs.AbbreviationsFile = QString::fromUtf8(_abbreviationFilePath);
    Configs.SpellCorrectorBaseConfigPath = QString::fromUtf8(_spellCorrectorBaseConfigPath);
    
    if(_language != NULL && strlen(_language) > 0) {
        QVariantHash PersianSpellCorrector;
        PersianSpellCorrector.insert("Active", true);

        Configs.SpellCorrectorLanguageBasedConfigs.insert(QString::fromUtf8(_language), PersianSpellCorrector);
    }

    TargomanTextProcessor::instance().init(Configs);

    return true;
}

bool secureCopyQStringToBuffer(QString _source, char* _target, int _targetMaxLength) {
     QByteArray Bytes = _source.toUtf8();
     memcpy(_target, Bytes.constData(), Bytes.size() < (_targetMaxLength - 1) ? Bytes.size() : (_targetMaxLength - 1));
     _target[_targetMaxLength - 1] = 0;
     return Bytes.size() < _targetMaxLength;
}

bool c_ixml2Text(const char* _source, char* _target, int _targetMaxLength) {
    QString Source = QString::fromUtf8(_source);
    return secureCopyQStringToBuffer(
            TargomanTextProcessor::instance().ixml2Text(Source),
            _target,
            _targetMaxLength);
}

bool c_text2IXML(const char* _source, const char* _language, bool _noSpellCorrector, char* _target, int _targetMaxLength) {
    QString Source = QString::fromUtf8(_source);
    QString Language = QString::fromUtf8(_language);
    bool SpellCorrected = false;
    return secureCopyQStringToBuffer(
            TargomanTextProcessor::instance().text2IXML(
                Source,
                SpellCorrected,
                Language,
                0,
                false,
                _noSpellCorrector,
                QList<enuTextTags::Type>(),
                QList<stuIXMLReplacement>()
            ),
            _target,
            _targetMaxLength);
}

bool c_tokenize(const char* _source, const char* _language, bool _noSpellCorrector, char* _target, int _targetMaxLength) {
    QString Source = QString::fromUtf8(_source);
    QString Language = QString::fromUtf8(_language);
    bool SpellCorrected = false;
    return secureCopyQStringToBuffer(
        TargomanTextProcessor::instance().ixml2Text(
            TargomanTextProcessor::instance().text2IXML(
                                       Source,
                                       SpellCorrected,
                                       Language,
                                       0,
                                       false,
                                       _noSpellCorrector,
                                       QList<enuTextTags::Type>(),
                                       QList<stuIXMLReplacement>()
                                       )),
        _target,
        _targetMaxLength);
}

bool c_normalize(const char* _source, const char* _language, char* _target, int _targetMaxLength) {
    QString Source = QString::fromUtf8(_source);
    QString Language = QString::fromUtf8(_language);
    bool SpellCorrected = false;
    return secureCopyQStringToBuffer(
                TargomanTextProcessor::instance().normalizeText(
                                   Source,
                                   SpellCorrected,
                                   Language),
                _target,
                _targetMaxLength);
}