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
 @author Behrooz Vedadian <vedadian@gmail.com>
 */

#ifndef TARGOMAN_CORE_PRIVATE_RULETABLE_CLSTARGETRULE_H
#define TARGOMAN_CORE_PRIVATE_RULETABLE_CLSTARGETRULE_H

#include <QList>
#include "libTargomanCommon/Types.h"
#include "libTargomanCore/clsTranslator.h"

namespace Targoman {
namespace Core {
namespace Private {
namespace RuleTable{

TARGOMAN_ADD_EXCEPTION_HANDLER(exRuleTable, exTargomanCore);

class clsTargetRuleData : public QSharedData
{
public:
    clsTargetRuleData(const QList<Common::WordIndex_t>& _targetPhrase,
                      const QList<Common::Cost_t>& _fields,
                      size_t _precomputedValueSize):
        TargetPhrase(_targetPhrase),
        Fields(_fields),
        PrecomputedValues(_precomputedValueSize,-INFINITY)
    {}

    clsTargetRuleData()
    {}

    clsTargetRuleData(const clsTargetRuleData& _other):
        QSharedData(_other),
        TargetPhrase(_other.TargetPhrase),
        Fields(_other.Fields),
        PrecomputedValues(_other.PrecomputedValues)
    {}
    ~clsTargetRuleData() {}

public:
    QList<Common::WordIndex_t> TargetPhrase;
    QList<Common::Cost_t>      Fields;
    QVector<Common::Cost_t>    PrecomputedValues;
};

class clsTargetRule
{
public:
    clsTargetRule();
    clsTargetRule(const QList<Common::WordIndex_t>& _targetPhrase,
                  const QList<Common::Cost_t>& _fields);
    clsTargetRule(const clsTargetRule& _other):
        Data(_other.Data)
    {}

    inline Common::WordIndex_t at(int _index) const{
        return this->Data->TargetPhrase.at(_index);
    }

    inline size_t size() const {
        return this->Data->TargetPhrase.size();
    }

    Common::Cost_t  field(size_t _index) const{
        Q_ASSERT(_index < (size_t)this->Data->Fields.size());
        return this->Data->Fields.at(_index);
    }

    Common::Cost_t precomputedValue(size_t _index) const{
        Q_ASSERT(_index < (size_t)this->Data->PrecomputedValues.size());
        return this->Data->PrecomputedValues.at(_index);
    }

    inline void setCosts(const QList<Common::Cost_t>& _costs){
        Q_ASSERT(this->Data->Fields.isEmpty());
        Q_ASSERT(clsTargetRule::ColumnNames.size() == _costs.size());

        this->Data->Fields = _costs;
    }

    void setPrecomputedValue(size_t _index, Common::Cost_t _value){
        Q_ASSERT(_index < (size_t)this->Data->PrecomputedValues.size());
        this->Data->PrecomputedValues[_index] = _value;
    }

    static size_t getColumnIndex(const QString& _name){
        if (clsTargetRule::ColumnNames.contains(_name) == false)
            throw exRuleTable("Invalid filed name. not declared in Rule Table: " + _name);
        return clsTargetRule::ColumnNames.indexOf(_name);
    }

    static void setColumnNames(const QStringList& _names){
        if (clsTargetRule::ColumnNames.size())
            throw exRuleTable("Invalid reinitialization of rule table column names");
        clsTargetRule::ColumnNames = _names;
    }

    static size_t allocatePrecomputedValue(){
        return ++clsTargetRule::PrecomputedValuesSize;
    }

private:
    QExplicitlySharedDataPointer<clsTargetRuleData>   Data;
    static  QStringList                     ColumnNames;
    static  size_t                          PrecomputedValuesSize;
};

extern clsTargetRule InvalidTargetRule;

}
}
}
}

#endif // TARGOMAN_CORE_PRIVATE_RULETABLE_CLSTARGETRULE_H
