//
// Created by pc4 on 2018/10/23.
//

#ifndef SOLO_LOCKFREESTACKBYLISTUSEHAZARDPOINTER_H
#define SOLO_LOCKFREESTACKBYLISTUSEHAZARDPOINTER_H

#include <memory>
#include <atomic>

/*
 * 一个使用hazard pointer（风险指针）的无锁队列
 *
 * hazard pointer 其实就是一组原子的标记位 每个线程把当前正持有的标记位告诉其他线程 这个节点目前有一个线程正在持有他 不要去删除这个节点
 */
template <typename T>
class LockFreeStackByListV4
{
public:
    struct StackNode
    {
        std::shared_ptr<T> data;
        StackNode* next;

        StackNode(T&& t)
            : data(std::make_shared<T>(t)), next(nullptr)
        { }
    };
private:
    std::atomic<void*> hazardPtr_;
    std::atomic<StackNode*> head_;
};
#endif //SOLO_LOCKFREESTACKBYLISTUSEHAZARDPOINTER_H
