//
// Created by pc4 on 2018/10/25.
//

#ifndef SOLO_THREADPOOLV2_H_H
#define SOLO_THREADPOOLV2_H_H

#include "ThreadSafeQueue.h"

#include <memory>
#include <future>
#include <vector>
#include <thread>

/*
 * 擦除绑定函数的类型, 一个通用的函数类型
 */
class FunctionWrapper
{
public:
    template <typename F>
    FunctionWrapper(F&& f)
        : impl_(new ImplType<F>(std::move(f)))
    {

    }

    void operator()()
    {

    }

private:

    /*
     * 类型擦除技术
     * 1 需要一个ImpleBase基类 和一个继承类 ImplType 目的是如果不使用这两个继承类 那么FunctionWarpper的内部
     */
    struct ImplBase
    {
        virtual void call() = 0;
        virtual ~ImplBase() {}
    };

    template <typename F>
    struct ImplType : ImplBase
    {
        F f;
        ImplType(F&& f_) : f(std::move(f_)) {}
        void call() { f(); }
    };

    std::unique_ptr<ImplBase> impl_;
};

class ThreadPool
{
    ThreadPool(size_t threadSize = std::thread::hardware_concurrency())
        : threadSize_(threadSize)
    { }

    template <typename FuncType>
    std::future<decltype(FuncType())>
    void SubmitTask(FuncType task)
    {
        std::packaged_task<decltype(FuncType())> packagedTask(std::move(task));

        taskQueue_.push(packagedTask);

        return packagedTask.get_future();
    }

private:
    ThreadSafeQueue<FunctionWrapper> taskQueue_;

    size_t threadSize_;
    std::vector<std::thread> threads_;
};

#endif //SOLO_THREADPOOLV2_H_H
