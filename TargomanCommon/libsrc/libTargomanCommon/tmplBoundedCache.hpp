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
#include <QMutex>
#include "libTargomanCommon/exTargomanBase.h"

namespace Targoman {
namespace Common {

template <template <class itmplKey, class itmplVal> class BaseContainer_t, class itmplKey, class itmplVal, quint32 itmplMaxItems = 10000>
    /**
     * @brief The tmplBoundedCache template is a derivation of Map or Hash class (depending on BaseContainer_t) and
     *        it is augmented with QHash for AccessDate.
     *
     * This class removes old items of Cache when itmlMaxItems is reached except when itmplMaxItems is set to zero.
     */
    class tmplBoundedCache : public BaseContainer_t <itmplKey, itmplVal>
    {
    public:

        tmplBoundedCache(){
            if (itmplMaxItems == 0)
                throw exTargomanInvalidParameter("Max cache Items must be greater than zero");
            this->MaxItems = itmplMaxItems;
        }

        tmplBoundedCache(const tmplBoundedCache& _other):
            BaseContainer_t<itmplKey, itmplVal>(_other),
            Lock(_other.Lock),
            KeyAccessDateTime(_other.KeyAccessDateTime),
            MaxItems(_other.MaxItems)
        {}

        typedef typename BaseContainer_t<itmplKey, itmplVal>::iterator   Iterator_t;

        inline Iterator_t insert(itmplKey _key, itmplVal _val){
            QMutexLocker Locker(&this->Lock);
            if(this->MaxItems && BaseContainer_t<itmplKey, itmplVal>::size() >= this->MaxItems){
                QList<QTime> Values = this->KeyAccessDateTime.values();
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
            if(this->MaxItems && BaseContainer_t<itmplKey, itmplVal>::size() >= this->MaxItems){
                QList<QTime> Values = this->KeyAccessDateTime.values();
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
            QMutexLocker Locker(&this->Lock);
            BaseContainer_t<itmplKey, itmplVal>::clear();
            this->KeyAccessDateTime.clear();
        }

        inline itmplVal value(const itmplKey& _key,
                              bool _updateAccessTime = true,
                              const itmplVal& _defaultValue = itmplVal()){
            QMutexLocker Locker(&this->Lock);
            if (BaseContainer_t<itmplKey, itmplVal>::contains(_key) == false)
                return _defaultValue;

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
            QMutexLocker Locker(&this->Lock);
            if (BaseContainer_t<itmplKey, itmplVal>::contains(_key) == false)
                return  BaseContainer_t<itmplKey, itmplVal>::operator [] (_key);

            QTime AccessTime = QTime::currentTime();
            this->KeyAccessDateTime.insert(_key, AccessTime);

            return BaseContainer_t<itmplKey, itmplVal>::operator [] (_key);
        }

        inline int remove(const itmplKey& _key){
            QMutexLocker Locker(&this->Lock);
            this->KeyAccessDateTime.remove(_key);
            return BaseContainer_t<itmplKey, itmplVal>::remove(_key);
        }

        void setMaxItems(quint32 _maxItems){ this->MaxItems = _maxItems; }

        quint32 maxItems(){ return this->MaxItems; }

    private:
        QMutex                       Lock;
        QHash<itmplKey, QTime >      KeyAccessDateTime;

        quint32 MaxItems;
    };
}
}
#endif // TARGOMAN_COMMON_TMPLEXPIRABLECACHE_H
