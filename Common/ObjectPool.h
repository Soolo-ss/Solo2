//
// Created by pc4 on 2018/7/10.
//

#ifndef SOLO_OBJECTPOOL_H
#define SOLO_OBJECTPOOL_H

#include <memory>
#include <stack>

const int OBJECT_POOL_INIT_SIZE = 64;

template <typename T>
class ObjectPool : public std::enable_shared_from_this<ObjectPool<T> >
{
public:
    class PoolObjectDeleter
    {
    public:
        PoolObjectDeleter(const std::shared_ptr<ObjectPool<T> >& pool)
            : pool_(pool)
        {

        }

        void operator()(T* ptr)
        {
            if (auto pn = pool_.lock())
            {
                pn->reclaim(std::unique_ptr<T>(ptr));
            }
            else
            {
                std::default_delete<T>{}(ptr);
            }
        }
    private:
        std::weak_ptr<ObjectPool<T> > pool_;
    };
public:
    using ptr_type = std::unique_ptr<T, PoolObjectDeleter>;

    ObjectPool()
    {
        assign();
    }

    ObjectPool(const ObjectPool&) = delete;
    void operator=(const ObjectPool&) = delete;

    size_t size() const
    {
        return objs_.size();
    }

    size_t all() const
    {
        return all_;
    }

    bool empty() const
    {
        return objs_.empty();
    }

    ptr_type acquire()
    {
        if (empty())
        {
            assign();
        }

        ptr_type  tmp { objs_.top().release(), PoolObjectDeleter(this->shared_from_this()) };

        objs_.pop();

        return std::move(tmp);
    }

    void reclaim(std::unique_ptr<T> t)
    {
        objs_.push(std::move(t));
    }

private:
    void assign(size_t size = OBJECT_POOL_INIT_SIZE)
    {
        for (int i = 0; i != size; ++i)
        {
            objs_.push(std::unique_ptr<T>(new T()));
            all_++;
        }
    }

private:
    std::stack<std::unique_ptr<T> > objs_;
    size_t all_ = 0;
};


#endif //SOLO_OBJECTPOOL_H
