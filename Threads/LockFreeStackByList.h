//
// Created by 46127 on 2018/10/22.
//
#ifndef SOLO_LOCKFREESTACKBYLIST_H
#define SOLO_LOCKFREESTACKBYLIST_H

#include <atomic>
#include <memory>

template <typename T>
class LockFreeStackByList
{
public:
    struct StackNode
    {
        T data;
        StackNode* next;

        StackNode(T&& t)
            : data(std::forward<T>(t)), next(nullptr)
        { }
    };

    void Push(const T& newData)
    {
        const StackNode* newNode = new StackNode(newData);          //此处可能会抛出异常 但是不会破坏原链表的结构 所以是异常安全的

        newNode->next = head_.load();

        /*
         * 检查head是否被其他线程改变 如果改变 那么newNode->next赋值为新的head 否则的话head = newNode
         */
        while(!head_.compare_exchange_weak(newNode->next, newNode))
    }

    /*
     * 一个POP的雏形
     *  存在内存泄漏
     */
    void Pop(T& result)
    {
        StackNode* oldHead = head_.load();

        /*
         * 比较相等 那么oldHead没有被改变 当前线程的head是正确的 使用oldHead->next替换掉head
         * 如果不等 用head_的值更新掉oldHead
         */
        while(!head_.compare_exchange_weak(oldHead, oldHead->next))
        result = oldHead->data;
    }

private:
    std::atomic<StackNode*> head_;
};

/*
 * 一个POP返回智能指针的进阶版本 带内存泄漏
 */
template <typename T>
class LockFreeStackByListV2
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

    LockFreeStackByListV2()
        : head_(nullptr)
    {

    }

    void Push(const T& newData)
    {
        StackNode* newNode = new StackNode(newData);
        newNode->next = head_.load();

        while(!head_.compare_exchange_weak(newNode->next, newNode))
    }

    /*
     * 比较上个版本的POP 两个优点
     * 1 没有之前使用引用的必要 因为如果while 退出 那么head已经被更新 数据结构本身已经被改变了 如果直接返回数据 在拷贝时
     *   发生异常 数据也会丢失。 传入引用的必要就没有了。
     * 2 判断了空指针的情况
     */
    std::shared_ptr<T> Pop()
    {
        StackNode* oldHead = head_.load();

        while(oldHead && !head_.compare_exchange_weak(oldHead, oldHead->next))
        return oldHead != nullptr? oldHead->data : std::shared_ptr<T>();
    }

private:
    std::atomic<StackNode*> head_;
};

/*
 * 一个没有内存泄漏的LockFreeStack
 */
template <typename T>
class LockFreeStackByListV3
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

    std::shared_ptr<T> Pop()
    {
        ++popThreadCount_;

        StackNode* oldHead = head_.load();
        while(!head_.compare_exchange_weak(oldHead, oldHead->next)) { }
        std::shared_ptr<T> result;

        //交值交换给传回的指针
        if (oldHead)
        {
            result.swap(oldHead->next);
        }

        //回收老的Head节点
        TryReclaim(oldHead);

        return result;
    }

private:
    void DeleteNodes(StackNode* node)
    {
        while(node)
        {
            StackNode* next = oldHead->next;
            delete node;
            node = next;
        }
    }

    void TryReclamin(StackNode* oldHead)
    {
        if (popThreadCount_ == 1)
        {
            StackNode* needDeletedNodes = deleted_.exchange(nullptr);

            if (!--popThreadCount_)
            {
                DeleteNodes(needDeletedNodes);
            }
            else if (needDeletedNodes)
            {

            }

            delete oldHead;
        }
        else
        {
            --popThreadCount_;
        }
    }

private:
    std::atomic<unsigned> popThreadCount_;
    std::atomic<StackNode*> deleted_;
    std::atomic<StackNode*> head_;
};

#endif //SOLO_LOCKFREESTACKBYLIST_H
