//
// Created by pc4 on 2018/10/22.
//

#ifndef SOLO_SPINLOCKMUTEX_H
#define SOLO_SPINLOCKMUTEX_H

#include <atomic>

/*
 * 一个简单的自旋锁
 */
class SimpleSpinLockMutex
{
public:
    SimpleSpinLockMutex()
        : flag_(ATOMIC_FLAG_INIT)
    { }

    void Lock()
    {
        while(flag_.test_and_set(std::memory_order_acquire));
    }

    void UnLock()
    {
        flag_.clear(std::memory_order_release);
    }

private:
    std::atomic_flag flag_;
};

#endif //SOLO_SPINLOCKMUTEX_H
