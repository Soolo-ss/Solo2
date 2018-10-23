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
        while(!head_.compare_exchange_weak(newNode->next, newNode)) { }
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

        while(!head_.compare_exchange_weak(newNode->next, newNode)) { }
    }

    /*
     * 比较上个版本的POP 两个优点
     * 1 没有之前使用引用的必要 因为如果while 退出 那么head已经被更新 数据结构本身已经被改变了 如果直接返回数据 在拷贝时
     *   发生异常 数据也会丢失。 传入引用的必要就没有了。使用智能指针，避免拷贝， 也就避免了可能的异常。
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
 * 核心问题  需要delete掉Pop掉的这个节点本身 数据已经通过shared_ptr<T>转移出去了
 * ABA问题   上个版本的POP
 * 假设两个线程Thread1 Thread2读取相同的oldHead值。线程1被切换至线程2，其中Thread2释放了oldHead, 并且POP掉了所有的Node，随后他又PUSH了一个Node， 恰好
 * PUSH的这个Node的内存地址与Thread1中读取的oldHead值相等。
 * 那么对于线程1的CAS来说， oldHead值与当前的head_相等 CAS成功。但此时oldHead->next是一个已经被释放了的值 -> 未定义行为
 *
 * 更常见的情况是Thread1持有一个oldHead 如果此时别的线程delete了这个oldHead 那么也会发生未定义行为
 *
 * 解决方案： 确保释放节点时 没有同时有多个线程持有这个节点
 *           也就是说 保证在删除节点的时候没有多个线程同时在POP 确保只有一个线程在调用POP
 *           那么再引入一个计数器 即同时在POP的线程计数器就可以解决这个问题
 *
 * 还存在的问题： 在低负载情况下  工作得很好
 *              如果在高负载的情况下 POP频繁得被调用， 没有任何机会来通过deleteNodes 来删除节点  那么这种情况下等同于内存泄露
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
        ++popThreadCount_;          //代表有线程正在调用pop

        StackNode* oldHead = head_.load();

         //如果此时线程在这里被抢占  那么oldHead这个节点 被其他的线程删除了 会发生未定义行为 所以需要保护这个节点
         //如果保护了这个节点 当前被抢占的线程还能通过CAS获得正确的oldHead
        while(!head_.compare_exchange_weak(oldHead, oldHead->next)) { }
        std::shared_ptr<T> result;

        //将数据交换给传回的指针
        if (oldHead)
        {
            result.swap(oldHead->data);
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
            StackNode* next = node->next;
            delete node;
            node = next;
        }
    }

    void AppendDeleteNodes(StackNode* nodes)
    {
        StackNode* last = nodes;

        while(StackNode* const next = last->next)
        {
            last = next;
        }

        AppendDeleteNodes(nodes, last);
    }

    void AppendDeleteNodes(StackNode* first, StackNode* last)
    {
        last->next = deleted_;

        while(!deleted_.compare_exchange_weak(last->next, first)) { }
    }

    void AppendDeleteNode(StackNode* node)
    {
        AppendDeleteNodes(node, node);
    }

    void TryReclaim(StackNode* oldHead)
    {
        if (popThreadCount_ == 1)
        {
            StackNode* needDeletedNodes = deleted_.exchange(nullptr);

            /*
             * 必须保证在DeleteNodes之前只有一个线程在POP 因为可能会有其他线程正持有一个被delete掉的节点
             */
            if (!--popThreadCount_)
            {
                DeleteNodes(needDeletedNodes);
            }
            else if (needDeletedNodes)
            {
                AppendDeleteNodes(needDeletedNodes);
            }

            delete oldHead;
        }
        else
        {
            AppendDeleteNode(oldHead);
            --popThreadCount_;
        }
    }

private:
    std::atomic<unsigned> popThreadCount_;
    std::atomic<StackNode*> deleted_;
    std::atomic<StackNode*> head_;
};




#endif //SOLO_LOCKFREESTACKBYLIST_H
