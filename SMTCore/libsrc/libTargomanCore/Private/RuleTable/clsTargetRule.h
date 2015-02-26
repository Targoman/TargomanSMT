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

class clsTargetRuleData;
extern clsTargetRuleData* InvalidTargetRuleData;

class clsTargetRule
{
public:
    clsTargetRule();

    clsTargetRule(const QList<Common::WordIndex_t>& _targetPhrase,
                  const QList<Common::Cost_t>& _fields);

    clsTargetRule(const clsTargetRule& _other):
        Data(_other.Data)
    {}
    ~clsTargetRule(){}

    inline bool isInvalid() const;

    inline Common::WordIndex_t at(int _index) const;
    inline size_t size() const;
#ifdef TARGOMAN_SHOW_DEBUG
    inline size_t fieldCount() const;
#endif
    inline Common::Cost_t  field(size_t _index) const;
    inline Common::Cost_t precomputedValue(size_t _index) const;
    inline void setCosts(const QList<Common::Cost_t>& _costs);
    inline void setPrecomputedValue(size_t _index, Common::Cost_t _value);
    static size_t getColumnIndex(const QString& _columnName, const QString& _moduleName){
        if (clsTargetRule::ColumnNames.contains(_columnName) == false)
            throw exRuleTable("Invalid filed name <"+_columnName+
                              "> not declared in Rule Table. Needed by:" + _moduleName);
        return clsTargetRule::ColumnNames.indexOf(_columnName);
    }

    static void setColumnNames(const QStringList& _names){
        if (clsTargetRule::ColumnNames.size())
            throw exRuleTable("Invalid reinitialization of rule table column names");
        clsTargetRule::ColumnNames = _names;
    }

    static size_t allocatePrecomputedValue(){
        size_t AllocatedIndex = clsTargetRule::PrecomputedValuesSize;
        ++clsTargetRule::PrecomputedValuesSize;
        return AllocatedIndex;

    }

    void detachInvalidData(){
        Q_ASSERT(this->Data == InvalidTargetRuleData);
        this->Data.detach();
    }

#ifdef TARGOMAN_SHOW_DEBUG
    QString toStr() const;
#endif

//private:
public:
    QExplicitlySharedDataPointer<clsTargetRuleData>   Data;
    static  QStringList                               ColumnNames;
    static  size_t                                    PrecomputedValuesSize;

    friend class clsTargetRuleData;
};

extern clsTargetRule* InvalidTargetRule;
/********************************************************************************/
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

    clsTargetRuleData(){
        if (clsTargetRule::ColumnNames.size() == 0)
            throw exRuleTable("Invalid TargetRule must be called after initialization");
        for(int i=0; i< clsTargetRule::ColumnNames.size(); ++i)
            this->Fields.append(INFINITY);
    }

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

/***********************************************************/
inline Common::WordIndex_t clsTargetRule::at(int _index) const{
    return this->Data->TargetPhrase.at(_index);
}

inline size_t clsTargetRule::size() const {
    return this->Data->TargetPhrase.size();
}

#ifdef TARGOMAN_SHOW_DEBUG
inline size_t clsTargetRule::fieldCount() const {
    return (size_t)this->Data->Fields.size();
}
#endif

inline Common::Cost_t  clsTargetRule::field(size_t _index) const{
    Q_ASSERT(_index < (size_t)this->Data->Fields.size());
    return this->Data->Fields.at(_index);
}

inline Common::Cost_t clsTargetRule::precomputedValue(size_t _index) const{
    Q_ASSERT(_index < (size_t)this->Data->PrecomputedValues.size());
    return this->Data->PrecomputedValues.at(_index);
}

inline void clsTargetRule::setCosts(const QList<Common::Cost_t>& _costs){
    Q_ASSERT(this->Data->Fields.isEmpty());
    Q_ASSERT(clsTargetRule::ColumnNames.size() == _costs.size());

    this->Data->Fields = _costs;
}

inline void clsTargetRule::setPrecomputedValue(size_t _index, Common::Cost_t _value){
    Q_ASSERT(_index < (size_t)this->Data->PrecomputedValues.size());
    this->Data->PrecomputedValues[_index] = _value;
}

inline bool clsTargetRule::isInvalid() const{
    return this->Data == InvalidTargetRuleData;
}

#ifdef TARGOMAN_SHOW_DEBUG

std::ostream& operator << (std::ostream& _outputStream, const clsTargetRule& _targetRule);

#endif

}
}
}
}

#endif // TARGOMAN_CORE_PRIVATE_RULETABLE_CLSTARGETRULE_H
