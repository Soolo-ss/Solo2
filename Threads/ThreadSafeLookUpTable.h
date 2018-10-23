//
// Created by pc4 on 2018/10/22.
//

#ifndef SOLO_THREADSAFELOOKUPTABLE_H
#define SOLO_THREADSAFELOOKUPTABLE_H

#include <boost/thread.hpp>

#include <list>
#include <vector>
#include <mutex>

/*
 * 线程安全的Hash表  -》 提供多个桶 每个桶持有一个读写锁
 * 此种设计 提供了细粒度的锁支持，提高了容器的性能
 */
template <typename Key, typename Value, typename Hash>
class ThreadSafeLooKUpTable
{
public:
    ThreadSafeLooKUpTable(size_t bucketSize = 20, const Hash& hash = Hash())
        : buckets_(bucketSize),
          hash_(hash)
    {

    }

    ThreadSafeLooKUpTable(const ThreadSafeLooKUpTable&) = delete;
    ThreadSafeLooKUpTable& operator= (const ThreadSafeLooKUpTable&) = delete;


    Value GetValueWithKey(const Key& key, const Value& defaultValue = Value())
    {
        return GetBucket(key).GetValueWithKey(key, defaultValue);
    }

    void AddEntry(const Key& key, const Value& value)
    {
        return GetBucket(key).AddEntry(key, value);
    }

    void RemoveEntry(const Key& key)
    {
        return GetBucket(key).RemoveEntry(key);
    }

private:
    /*
     *
     */
    class Bucket
    {
    public:
        using BucketValue = std::pair<Key, Value>;
        using BucketData = std::list<BucketValue>;
        using BucketIterator = BucketData::iterator;

        Value GetValueWithKey(const Key& key, const Value& default_value)
        {
            boost::shared_lock<boost::shared_mutex> lock(mutex_);

            BucketIterator find = FindEntryWithKey(key);

            return (find == data_.end()) ? default_value : find->second;
        }

        void AddEntry(const Key& key, const Value& value)
        {
            std::unique_lock<boost::shared_mutex> lock(mutex_);

            const BucketIterator findEntry = FindEntryWithKey(key);

            if (findEntry == data_.end())
            {
                data_.push_back(value);
            }
            else
            {
                findEntry->second = value;
            }
        }

        void RemoveEntry(const Key& key)
        {
            std::unique_lock<boost::shared_mutex> lock(mutex_);

            const BucketIterator findEntry = FindEntryWithKey(key);

            if (findEntry != data_.end())
            {
                data_.erase(findEntry);
            }
        }

    private:
        BucketIterator FindEntryWithKey(const Key& key) const
        {
            return std::find_if(data_.begin(), data_.end(), [&](const BucketValue& item) { return item.first == key; });
        }

    private:
        BucketData data_;
        mutable boost::shared_mutex mutex_;
    };

    std::vector<std::unique_ptr<Bucket>> buckets_;
    Hash hash_;

    Bucket& GetBucket(const Key& key)
    {
        const std::size_t bucketIndex = hash_(key) % buckets_.size();
        return *(buckets_[bucketIndex]);
    }
};

#endif //SOLO_THREADSAFELOOKUPTABLE_H
