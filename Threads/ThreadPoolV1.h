//
// Created by pc4 on 2018/10/25.
//

#ifndef SOLO_THREADPOOLV1_H
#define SOLO_THREADPOOLV1_H

#include "ThreadSafeQueue.h"

#include <thread>
#include <atomic>
#include <vector>
#include <functional>

/*
 * 初级线程池实现
 */
class ThreadPool
{
public:
    ThreadPool(size_t threadSize = std::thread::hardware_concurrency())
        : threadSize_(threadSize),
          done_(true)
    {

    }

    ~ThreadPool()
    {
        if (done_ == false)
        {
            done_ = true;

            for(int i = 0; i != threadSize_; ++i)
            {
                threads_[i].join();
            }
        }
    }

    void Run()
    {
        done_ = false;
        for(int i = 0; i != threadSize_; ++i)
        {
            threads_.emplace_back(&ThreadPool::DoThreadWork, this);
        }
    }

    void Stop()
    {
        done_ = true;

        for(int i = 0; i != threadSize_; ++i)
        {
            threads_[i].join();
        }
    }

    void SubmitTask(std::function<void()> task)
    {
        if (!done_)
        {
            taskQueue_.push(task);
        }
        else
        {
            throw std::runtime_error("submit task to stopped threadpool");
        }
    }
private:
    void DoThreadWork()
    {
        std::shared_ptr<std::function<void()>> task = taskQueue_.wait_and_pop();

        while(!done_)
        {
            if (task)
            {
                task->operator()();
            }
            else
            {
                std::this_thread::yield();
            }
        }
    }

private:
    std::atomic<bool> done_;
    std::vector<std::thread> threads_;

    ThreadSafeQueue<std::function<void()>> taskQueue_;
    size_t threadSize_;
};

#endif //SOLO_THREADPOOLV1_H
