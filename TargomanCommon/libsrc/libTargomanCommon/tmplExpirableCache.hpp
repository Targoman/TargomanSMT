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
     * @brief The tmplExpirableCache class is a derivation of Map class and it is augmented with QHash for AccessDate.
     *
     * This class removes old items of Cache and updates access date of recently used items .
     */
    class tmplExpirableCache : public BaseContainer_t <itmplKey, itmplVal>
    {
    public:

        tmplExpirableCache(){
            if (itmplMaxItems == 0)
                throw exTargomanInvalidParameter("Max cache Items must be greater than zero");
            this->MaxItems = itmplMaxItems;
            this->TTL = itmplTTL;
        }

        typedef typename BaseContainer_t<itmplKey, itmplVal>::iterator   Iterator_t;

        inline Iterator_t insert(itmplKey _key, itmplVal _val){
            //while(BaseContainer_t<itmplKey, itmplVal>::size() >= this->MaxItems){
            if(BaseContainer_t<itmplKey, itmplVal>::size() >= this->MaxItems){
                QList<QTime> Values = this->KeyAccessDateTime.values();
//                qStableSort(Values);
//                QList<itmplKey> ExpiredKeys = this->KeyAccessDateTime.keys(Values.first());
                QTime Oldest = Values.first();
                for(int i = 0; i < Values.size(); ++i)
                    if(Values.at(i) < Oldest)
                        Oldest = Values.at(i);
                QList<itmplKey> ExpiredKeys = this->KeyAccessDateTime.keys(Oldest);

                foreach(itmplKey Key, ExpiredKeys){
                    this->remove(Key);
                }
            }

            this->KeyAccessDateTime.insert(_key, QTime::currentTime());
            return BaseContainer_t<itmplKey, itmplVal>::insert(_key, _val);
        }

        inline Iterator_t insertMulti(itmplKey _key, itmplVal _val){
            //while(BaseContainer_t<itmplKey, itmplVal>::size() >= this->MaxItems){
            if(BaseContainer_t<itmplKey, itmplVal>::size() >= this->MaxItems){
                QList<QTime> Values = this->KeyAccessDateTime.values();
//                qStableSort(Values);
//                QList<itmplKey> ExpiredKeys = this->KeyAccessDateTime.keys(Values.first());
                QTime Oldest = Values.first();
                for(int i = 0; i < Values.size(); ++i)
                    if(Values.at(i) < Oldest)
                        Oldest = Values.at(i);
                QList<itmplKey> ExpiredKeys = this->KeyAccessDateTime.keys(Oldest);

                foreach(itmplKey Key, ExpiredKeys){
                    this->remove(Key);
                }
            }

            this->KeyAccessDateTime.insertMulti(_key, QTime::currentTime());
            return BaseContainer_t<itmplKey, itmplVal>::insertMulti(_key, _val);
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
            /*
            if (this->TTL > 0 && QTime::currentTime().msecsTo(this->KeyAccessDateTime.value(_key)) > this->TTL){
                this->remove(_key);
                return _defaultValue;
            }
            */

            if (_updateAccessTime)
                this->KeyAccessDateTime.insert(_key, QTime::currentTime());
            return BaseContainer_t<itmplKey, itmplVal>::value(_key);
        }

        inline QList<itmplVal> values(const itmplKey& _key,
                              bool _updateAccessTime = true,
                              const itmplVal& _defaultValue = itmplVal()){
            if (BaseContainer_t<itmplKey, itmplVal>::contains(_key) == false)
                return _defaultValue;


            if (_updateAccessTime) {
                auto Iter = this->KeyAccessDateTime.find(_key);
                QTime Time = QTime::currentTime();
                while(Iter != this->KeyAccessDateTime.end() && Iter.key() == _key) {
                    Iter.value() = Time;
                    ++Iter;
                }
            }
            return BaseContainer_t<itmplKey, itmplVal>::values(_key);
        }

        inline itmplVal& operator[] ( const itmplKey & _key){
            if (BaseContainer_t<itmplKey, itmplVal>::contains(_key) == false)
                return  BaseContainer_t<itmplKey, itmplVal>::operator [] (_key);

            /*
            if (this->TTL > 0 &&
                this->KeyAccessDateTime.value(_key).elapsed() > this->TTL){
                this->KeyAccessDateTime.remove(_key);
                return BaseContainer_t<itmplKey, itmplVal>::take(_key);
            }
            */

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
