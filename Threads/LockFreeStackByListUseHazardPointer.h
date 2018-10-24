//
// Created by pc4 on 2018/10/23.
//

#ifndef SOLO_LOCKFREESTACKBYLISTUSEHAZARDPOINTER_H
#define SOLO_LOCKFREESTACKBYLISTUSEHAZARDPOINTER_H

#include <memory>
#include <atomic>
#include <thread>
#include <functional>

unsigned const maxHazardPointers = 100;

struct HazardPointer
{
    std::atomic<std::thread::id> id;
    std::atomic<void*> pointer;
};

//一个全局的风险指针数组
HazardPointer hazardPointers[maxHazardPointers];

/*
 * 每个线程持有的风险指针
 */
class HazardPointerOwner
{
public:
    /*
     *  初始化风险指针
     *  查找某一个没有被设置过的hazrdPointer 设置之
     */
    HazardPointerOwner()
        : hp(nullptr)
    {
        for(unsigned int i = 0; i < maxHazardPointers; ++i)
        {
            std::thread::id oldThreadID;
            if (hazardPointers[i].id.compare_exchange_strong(oldThreadID, std::this_thread::get_id()))
            {
                hp = &hazardPointers[i];
                break;
            }
        }

        if (!hp)
        {
            throw std::runtime_error("No hazard pointers available");
        }
    }

    HazardPointerOwner(const HazardPointerOwner&) = delete;
    HazardPointerOwner& operator=(const HazardPointerOwner&) = delete;

    std::atomic<void*>& GetHazardPointer()
    {
        return hp->pointer;
    }

private:
    HazardPointer* hp;
};

/*
 * 获取当前线程的HazardPointer
 */
std::atomic<void*>& GetHazardPointerForCurrentThread()
{
    thread_local static HazardPointerOwner hazard;

    return hazard.GetHazardPointer();
}

/*
 * 查询是否此节点被其他线程持有
 */
bool CheckHazardPointer(void* p)
{
    for(unsigned int i = 0; i != maxHazardPointers; ++i)
    {
        if (hazardPointers[i].pointer.load() == p)
        {
            return true;
        }
    }

    return false;
}

template <typename T>
void DoDelete(void* p)
{
    delete static_cast<T*>(p);
}

struct ReclaimData
{
    void* data;
    std::function<void(void*)> deleter;
    ReclaimData* next;

    template <typename T>
    ReclaimData(T* p)
        : data(p), deleter(&DoDelete<T>), next(nullptr)
    { }

    ~ReclaimData()
    {
        deleter(data);
    }
};

/*
 * 全局的回收列表
 */
std::atomic<ReclaimData*> reclaimNodes;

/*
 * 添加节点至回收列表
 */
void AddToReclaimNodes(ReclaimData* node)
{
    node->next = reclaimNodes.load();
    while(!reclaimNodes.compare_exchange_weak(node->next, node)) {}
}

template <typename T>
void ReclaimLater(T* data)
{
    AddToReclaimNodes(new ReclaimData(data));
}

void DeleteReclaimDataWithNoHazard()
{
    ReclaimData* current = reclaimNodes.exchange(nullptr);

    while(current)
    {

    }
}

/*
 * 一个使用hazard pointer（风险指针）的无锁队列
 *
 * hazard pointer 其实就是一组原子的标记位 每个线程把当前正持有的标记位告诉其他线程 这个节点目前有一个线程正在持有他 不要去删除这个节点
 */
template <typename T>
class LockFreeStackByListUseHazardPointer
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
        std::atomic<void*>& hp = GetHazardPointerForCurrentThread();

        StackNode* oldHead = head_.load();
        do
        {
            StackNode* temp{nullptr};
            do
            {
                temp = oldHead;
                hp.store(oldHead);      //存储风险指针 告诉其他线程 本线程正持有这个节点
                oldHead = head_.load();
            } while (oldHead != temp);
        }
        while(oldHead && head_.compare_exchange_strong(oldHead, oldHead->next));             //确保当前这个oldHead是目前的head

        hp.store(nullptr);              //不需要这个风险指针了 持有这个节点完毕

        std::shared_ptr<T> res;

        if (oldHead)
        {
            res.swap(oldHead->data);

            if (CheckHazardPointer(oldHead))
            {
                //此时有其他线程持有这个节点 不能马上删除
            }
            else
            {
                delete oldHead;
            }
        }

        return res;
    }
private:
    std::atomic<void*> hazardPtr_;
    std::atomic<StackNode*> head_;
};
#endif //SOLO_LOCKFREESTACKBYLISTUSEHAZARDPOINTER_H
