//
// Created by pc4 on 2018/7/11.
//

#ifndef SOLO_CROSSAOISERVER_H
#define SOLO_CROSSAOISERVER_H

#include "AOIServer.h"

class CrossAOINode : public AOINode
{
public:
    CrossAOINode()
        : AOINode::AOINode(0.0, 0.0)
    { }

    CrossAOINode(float x, float y)
            : AOINode::AOINode(x, y)
    { }

public:
    CrossAOINode* next;
    CrossAOINode* pre;
};

class CrossAOIServer
{
public:
    CrossAOIServer()
            : x_head_(nullptr),
              x_tail_(nullptr)
    {
    }

    void insert(CrossAOINode* node)
    {
        node->next = nullptr;
        node->pre = nullptr;

        if (x_head_)
        {
            x_head_->pre = node;
            node->next = x_head_;
            x_head_ = node;
        }
        else
        {
            x_head_ = node;
            x_tail_ = node;
        }

        update(node);
    }

    void update(CrossAOINode* node)
    {
        CrossAOINode* nextNode = node->next;
        CrossAOINode* preNode = node->pre;

        while(nextNode && (node->x() > nextNode->x()))
        {
            CrossAOINode tmp = *nextNode;

            if (preNode == nullptr)
                nextNode->pre = nullptr;
            else
                nextNode->pre = preNode;

            nextNode->next = node;
            node->pre = nextNode;

            if (tmp.next == nullptr)
            {
                node->next = nullptr;
                x_tail_ = node;
            }
            else
            {
                node->next = tmp.next;
                node->next->pre = node;
            }
        }
    }

    void remove(CrossAOINode* node)
    {

    }
private:
    CrossAOINode* x_head_;
    CrossAOINode* x_tail_;
};


#endif //SOLO_CROSSAOISERVER_H
