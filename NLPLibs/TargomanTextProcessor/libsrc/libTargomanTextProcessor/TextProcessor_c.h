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
 
bool c_init(
    const char* _normalizationFilePath,
    const char* _abbreviationFilePath,
    const char* _spellCorrectorBaseConfigPath,
    const char* _languageCode
    );

bool c_ixml2Text(const char* _source, char* _target, int _targetMaxLength);
bool c_text2IXML(const char* _source, const char* _language, bool _noSpellCorrector, char* _target, int _targetMaxLength);
bool c_tokenize(const char* _source, const char* _language, bool _noSpellCorrector, char* _target, int _targetMaxLength);
bool c_normalize(const char* _source, const char* _language, char* _target, int _targetMaxLength);
