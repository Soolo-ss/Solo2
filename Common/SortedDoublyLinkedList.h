//
// Created by pc4 on 2018/7/10.
//

#ifndef SOLO_SORTEDDOUBLYLINKEDLIST_H
#define SOLO_SORTEDDOUBLYLINKEDLIST_H

#include <memory>
#include <list>
#include <functional>

#include "Singleton.h"

/*
 * 用于AOI坐标系
 */
template <typename T>
class SortedDoublyLinkedList
{
public:
    SortedDoublyLinkedList()
    { }

    SortedDoublyLinkedList(const SortedDoublyLinkedList&) = delete;
    void operator=(const SortedDoublyLinkedList&) = delete;

    size_t size() const
    {
        return nodes_.size();
    }

    bool empty() const
    {
        return nodes_.empty();
    }

    bool insert(const std::shared_ptr<T>& node, std::function<void()> onNodePassedCB)
    {
        nodes_.push_front(node);


    }
private:
    std::list<std::shared_ptr<T> > nodes_;
};


#endif //SOLO_SORTEDDOUBLYLINKEDLIST_H
