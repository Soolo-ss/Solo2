#ifndef SOLO_THREAD_SAFE_QUEUE_BY_LIST_H
#define SOLO_THREAD_SAFE_QUEUE_BY_LIST_H

#include <memory>
#include <mutex>

struct EmptyQueueByList : std::exception
{
	const char* what() const noexcept
	{
		return "empty queue by list";
	}
};

/*
线程安全的队列 锁的粒度较大  接口有待完善
 每个节点持有一个std::unique_ptr<QueueNode> 的设计，在队列析构时如果队列中持有大量节点的情况下，
 会导致递归得调用unique_ptr的析构函数， 并且耗尽栈空间。 注意
*/
template <typename T>
class ThreadSafeQueueByListV1
{
public:
	struct QueueNode
	{
		T data;
		std::unique_ptr<QueueNode> next;
		
		QueueNode(T&& data)
			: data(std::forward<T>(data)), next(nullptr)
		{ }
	};

	ThreadSafeQueueByListV1()
		: head_(nullptr), tail_(nullptr)
	{ }

	ThreadSafeQueueByListV1(const ThreadSafeQueueByListV1&) = delete;
	ThreadSafeQueueByListV1& operator=(const ThreadSafeQueueByListV1&) = delete;

	void push(T newV)
	{
		//std::unique_ptr<QueueNode> newNode = std::make_unique<QueueNode>(new QueueNode(std::move(newV)));
		std::unique_ptr<QueueNode> newNode(new QueueNode(std::move(newV)));
		std::lock_guard<std::mutex> lg(mutex_);

		QueueNode* newTail = newNode.get();

		if (!head_)
		{
			head_ = std::move(newNode);
		}
		else
		{
			tail_->next = std::move(newNode);
		}

		tail_ = newTail;
	}

	std::shared_ptr<T> pop()
	{
		std::lock_guard<std::mutex> lg(mutex_);

		if (!head_)
		{
			//throw EmptyQueueByList();
			return std::shared_ptr<T>();
		}

		//std::shared_ptr<T> popone(head_);
		std::shared_ptr<T> popone = std::make_shared<T>(std::move(head_->data));
		
		std::unique_ptr<QueueNode> oldHead = std::move(head_);
		head_ = std::move(oldHead->next);

		if (!head_)
		{
			tail_ = nullptr;
		}

		return popone;
	}

	bool empty()
	{
		std::lock_guard<std::mutex> lg(mutex_);

		return head_ == nullptr;
	}

private:
	std::mutex mutex_;

	std::unique_ptr<QueueNode> head_;
	QueueNode* tail_;
};

/*
�̰߳�ȫ��ջ �������� ���ܽϸ�
*/
template <typename T>
class ThreadSafeQueueByListV2
{

};

#endif
