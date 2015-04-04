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

#ifndef TARGOMAN_COMMON_TMPLEXPIRABLECACHE_H
#define TARGOMAN_COMMON_TMPLEXPIRABLECACHE_H

#include <QHash>
#include <QTime>
#include "libTargomanCommon/exTargomanBase.h"

namespace Targoman {
namespace Common {

template <template <class itmplKey, class itmplVal> class BaseContainer_t, class itmplKey, class itmplVal, quint32 itmplMaxItems = 10000, qint32 itmplTTL = 10000>
    /**
     * @brief The tmplExpirableCache class has two Maps, one for Cache and one for AccessDate.
     *
     * This class removes old items of Cache and updates access date of recently used items .
     */
    class tmplExpirableCache : public BaseContainer_t <itmplKey, itmplVal>
    {
    public:
      /*  class const_iterator:public QHash<itmplKey,itmplVal>::const_iterator
        {};*/

        tmplExpirableCache(){
            if (itmplMaxItems == 0)
                throw exTargomanInvalidParameter("Max cache Items must be greater than zero");
            this->MaxItems = itmplMaxItems;
            this->TTL = itmplTTL;
        }

        inline void insert(itmplKey _key, itmplVal _val){
            while(BaseContainer_t<itmplKey, itmplVal>::size() >= this->MaxItems){
                QList<QTime> Values = this->KeyAccessDateTime.values();
                qStableSort(Values);
                QList<itmplKey> ExpiredKeys = this->KeyAccessDateTime.keys(Values.first());

                foreach(itmplKey Key, ExpiredKeys){
                    this->remove(Key);
                }
            }

            this->KeyAccessDateTime.insert(_key, QTime::currentTime());
            BaseContainer_t<itmplKey, itmplVal>::insert(_key, _val);
        }

        inline void clear(){
            BaseContainer_t<itmplKey, itmplVal>::clear();
            this->KeyAccessDateTime.clear();
        }

        inline itmplVal value(const itmplKey& _key,
                              bool _updateAccessTime = true,
                              const itmplVal& _defaultValue = itmplVal()){
            if (BaseContainer_t<itmplKey, itmplVal>::contains(_key) == false)
                return _defaultValue;

            if (this->TTL > 0 && QTime::currentTime().msecsTo(this->KeyAccessDateTime.value(_key)) > this->TTL){
                this->remove(_key);
                return _defaultValue;
            }
            if (_updateAccessTime)
                this->KeyAccessDateTime.insert(_key, QTime::currentTime());
            return BaseContainer_t<itmplKey, itmplVal>::value(_key);
        }

        inline itmplVal& operator[] ( const itmplKey & _key){
            if (BaseContainer_t<itmplKey, itmplVal>::contains(_key) == false)
                return  BaseContainer_t<itmplKey, itmplVal>::operator [] (_key);

            if (this->TTL > 0 &&
                this->KeyAccessDateTime.value(_key).elapsed() > this->TTL){
                this->KeyAccessDateTime.remove(_key);
                return BaseContainer_t<itmplKey, itmplVal>::take(_key);
            }

            QTime AccessTime = QTime::currentTime();
            this->KeyAccessDateTime.insert(_key, AccessTime);

            return BaseContainer_t<itmplKey, itmplVal>::operator [] (_key);
        }

        inline int remove(const itmplKey& _key){
            this->KeyAccessDateTime.remove(_key);
            return BaseContainer_t<itmplKey, itmplVal>::remove(_key);
        }

        void setMaxItems(quint32 _maxItems){ this->MaxItems = _maxItems; }
        void setTTL(quint32 _ttl){ this->TTL = _ttl; }

        quint32 maxItems(){ return this->MaxItems; }
        quint32 ttl(){ return this->TTL; }

    private:
        QHash<itmplKey, QTime >      KeyAccessDateTime;

        quint32 MaxItems;
        quint32 TTL;
    };
}
}
#endif // TARGOMAN_COMMON_TMPLEXPIRABLECACHE_H
