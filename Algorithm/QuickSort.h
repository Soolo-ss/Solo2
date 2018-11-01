//
// Created by pc4 on 2018/10/24.
//

#ifndef SOLO_QUICKSORT_H
#define SOLO_QUICKSORT_H

#include <list>
#include <vector>
#include <future>
#include <chrono>
#include <algorithm>

#include "../Threads/ThreadSafeStack.h"

template <typename T>
class QuickSorter
{
public:
    struct ChunkToSort
    {
        std::list<T> data;
        std::promise<std::list<T>> promise;
    };

    QuickSorter()
        : maxThreadCount_(std::thread::hardware_concurrency() - 1),
          endOfData_(false)
    { }

    ~QuickSorter()
    {
        endOfData_ = true;
        for(auto thread : threads_)
        {
            thread.join();
        }
    }

    std::list<T> Sort(std::list<T>& chunkData)
    {
        if (chunkData.empty())
        {
            return chunkData;
        }

        std::list<T> result;

        result.splice(result.begin(), chunkData, chunkData.begin());

        const T& partitionVal = *result.begin();

        auto dividePoint = std::partition(chunkData.begin(), chunkData.end(), [&] (const T& val) { return val < partitionVal; });

        ChunkToSort lowerChunk;
        lowerChunk.data.splice(lowerChunk.data.end(), chunkData, chunkData.begin(), dividePoint);

        std::future<std::list<T>> lowerFuture = lowerChunk.promise.get_future();

        chunkStack_.push(std::move(lowerChunk));

        if (threads_.size() < maxThreadCount_)
        {
            threads_.emplace_back(&QuickSorter<T>::SortThread, this);
        }

        std::list<T> highChunk(Sort(chunkData));

        result.splice(result.end(), highChunk);

        while(lowerFuture.wait_for(std::chrono::seconds(0)) != std::future_status::ready)
        {
            TrySortChunk();
        }

        result.splice(result.begin(), lowerFuture.get());

        return result;
    }

    void TrySortChunk()
    {
        std::shared_ptr<ChunkToSort> chunk = chunkStack_.pop();
        if (chunk)
        {
            SortChunk(chunk);
        }
    }

    void SortChunk(std::shared_ptr<ChunkToSort> chunk)
    {
        chunk->promise.set_value(Sort(chunk->data));
    }

    void SortThread()
    {
        while(!endOfData_)
        {
            TrySortChunk();
            std::this_thread::yield();
        }
    }

private:
    Solo::ThreadSafeStack<ChunkToSort> chunkStack_;

    std::vector<std::thread> threads_;
    const unsigned maxThreadCount_;
    std::atomic<bool> endOfData_;
};

template <typename T>
std::list<T> QuickSortWithParallel(std::list<T> input)
{
    if (input.empty())
        return input;

    QuickSorter<T> sort;

    return sort.Sort(input);
}

#endif //SOLO_QUICKSORT_H
