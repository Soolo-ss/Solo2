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
			{
				nextNode->pre = nullptr;
				x_head_ = nextNode;
			}
			else
			{
				nextNode->pre = preNode;
				preNode->next = nextNode;
			}

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

			nextNode = node->next;
			preNode = node->pre;
        }
    }

    void remove(CrossAOINode* node)
    {
		CrossAOINode* currentNode = x_head_;
		while (currentNode != nullptr)
		{
			if (node == currentNode)
			{
				if (currentNode->pre == nullptr)
				{
					if (currentNode->next == nullptr)
					{
						x_head_ = nullptr;
					}
					else
					{
						x_head_ = currentNode->next;
						x_head_->pre = nullptr;
					}
				}
				else
				{
					if (currentNode->next == nullptr)
					{
						currentNode->pre->next = nullptr;
						x_tail_ = currentNode->pre;
					}
					else
					{
						currentNode->pre->next = currentNode->next;
						currentNode->next->pre = currentNode->pre;
					}
				}

				break;
			}

			currentNode = currentNode->next;
		}
    }
private:
    CrossAOINode* x_head_;
    CrossAOINode* x_tail_;
};


#endif //SOLO_CROSSAOISERVER_H
