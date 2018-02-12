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
 * @author Saeed Torabzadeh <saeed.torabzadeh@targoman.com>
 */

#include "clsTargetRule.h"
#include<iostream>
#include "Private/GlobalConfigs.h"
#include "Private/Proxies/LanguageModel/intfLMSentenceScorer.hpp"

namespace Targoman {
namespace SMT {
namespace Private {
namespace RuleTable{

using namespace Common;

QStringList  clsTargetRule::ColumnNames;
size_t clsTargetRule::PrecomputedValuesSize = 0;

bool clsTargetRule::AlignmentDataAvailable;
bool clsTargetRule::LexicalReorderingAvailable;

clsTargetRule* pInvalidTargetRule = NULL;
clsTargetRuleData* InvalidTargetRuleData = NULL;

/**
 * @brief constructor of this class initializes #Data with InvalidRargetRuleData which is instantiated before.
 */
clsTargetRule::clsTargetRule():
    Data(InvalidTargetRuleData)
{
    Q_ASSERT_X(RuleTable::InvalidTargetRuleData != NULL, "clsTargetRule",
        "Invalid target rule instantiated before InvalidTargetRuleData initialization.");
}

/**
 * @brief This Constructor of class initializes #Data with valid data.
 * @param _targetPhrase target language translation phrase.
 * @param _fields   fields of different feature functions for this translation phrase.
 */
clsTargetRule::clsTargetRule(const QList<WordIndex_t> &_targetPhrase, const QList<Cost_t> &_fields, const QMap<int, int> &_alignments, bool _hasNoRuleTableRecord):
    Data(new clsTargetRuleData(_targetPhrase, _fields, _alignments, clsTargetRule::PrecomputedValuesSize, _hasNoRuleTableRecord))
{
    if(_targetPhrase.size() == 1 && _targetPhrase.at(0) == gConfigs.EmptyLMScorer->unknownWordIndex())
        this->Data->IsUnknownWord = true;
}

void clsTargetRule::readBinary(clsIFStreamExtended &_input)
{
    if(this->isInvalid())
        this->detachInvalidData();
    int Count = _input.read<int>();
    this->Data->TargetPhrase.reserve(Count);
    for(int i = 0; i < Count; ++i) {
        WordIndex_t WordIndex = _input.read<WordIndex_t>();
        this->Data->TargetPhrase.append(WordIndex);
    }
    if(this->Data->TargetPhrase.size() == 1 && this->Data->TargetPhrase.at(0) == gConfigs.EmptyLMScorer->unknownWordIndex())
        this->Data->IsUnknownWord = true;
    for(Cost_t& Cost : this->Data->Fields)
        Cost = _input.read<Cost_t>();
    this->Data->PrecomputedValues.fill(-INFINITY, clsTargetRule::PrecomputedValuesSize);

    int AlignmentSize = _input.read<int>();
    for(int i = 0; i < AlignmentSize; i++){
        int key = _input.read<int>();
        int value = _input.read<int>();
        this->Data->Alignment.insertMulti(key, value);
    }
}

void clsTargetRule::writeBinary(clsOFStreamExtended &_output) const
{
    _output.write(this->Data->TargetPhrase.size());
    foreach(WordIndex_t WordIndex, this->Data->TargetPhrase)
        _output.write(WordIndex);

    foreach(Cost_t Cost, this->Data->Fields)
        _output.write(Cost);

    _output.write(this->Data->Alignment.size());
    foreach (int key, this->Data->Alignment.keys()) {
        foreach (int value, this->Data->Alignment.values(key)){
//            QString align = QString(key) + QString("-") + QString(value);
            _output.write(key);
            _output.write(value);
//            _output.write(align.toStdString().c_str());
        }
    }
}

#ifdef TARGOMAN_SHOW_DEBUG

QString clsTargetRule::toStr() const
{
    if(this->isInvalid())
        return QString();
    QString result = gConfigs.EmptyLMScorer->getWordByIndex(this->Data->TargetPhrase.at(0));
    for(int i = 1; i < this->Data->TargetPhrase.size(); ++i)
        result += " " + gConfigs.EmptyLMScorer->getWordByIndex(this->Data->TargetPhrase.at(i));
    return result;
}

#endif

}
}
}
}

#ifdef TARGOMAN_SHOW_DEBUG
QTextStream& operator << (QTextStream& _outputStream, const Targoman::SMT::Private::RuleTable::clsTargetRule& _targetRule)
{
    if(_targetRule.fieldCount() == 0)
        return _outputStream;
    _outputStream << _targetRule.field(0);
    for(size_t i = 1; i < _targetRule.fieldCount(); ++i)
        _outputStream << " " << _targetRule.field(i);
    return _outputStream;
}

#endif
