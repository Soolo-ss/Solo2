#ifndef SOLO_THREAD_SAFE_QUEUE_BY_LIST_H
#define SOLO_THREAD_SAFE_QUEUE_BY_LIST_H

#include <memory>
#include <mutex>

struct EmptyQueueByList : std::exception
{
	const char* what() const
	{
		return "empty queue by list";
	}
};

/*
线程安全的栈 锁的粒度较大 性能较低
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
线程安全的栈 低粒度锁 性能较高
*/
template <typename T>
class ThreadSafeQueueByListV2
{

};

#endif
