#ifndef SOLO_THREAD_SAFE_QUEUE_H
#define SOLO_THREAD_SAFE_QUEUE_H

#include <mutex>
#include <queue>
#include <condition_variable>

struct EmptyQueue : std::exception
{
	const char* what() const noexcept {
		return "empty queue";
	}
};

/*
和ThreadSafeStack一样 取消front接口 直接poo返回 避免data race
提供pop_and_wait 使用condition_variable来避免取数据时轮询是否为空
*/
template<typename T>
class ThreadSafeQueue
{
public:
	ThreadSafeQueue() { }

	ThreadSafeQueue(const ThreadSafeQueue&) = delete;
	ThreadSafeQueue& operator=(const ThreadSafeQueue&) = delete;

	template<typename U>
	void push(U&& newV)
	{
		std::lock_guard<std::mutex> lg(mutex_);
		queue_.push(std::forward<U>(newV));
		cond_.notify_one();
	}

	std::shared_ptr<T> pop()
	{
		std::lock_guard<std::mutex> lg(mutex_);

		if (queue_.empty())
		{
			throw EmptyQueue();
		}

		std::shared_ptr<T> popone = std::make_shared<T>(queue_.pop());

		return popone;
	}

	std::shared_ptr<T> wait_and_pop()
	{
		std::unique_lock<std::mutex> ul(mutex_);
		cond_.wait(ul, [this] { return !queue_.empty(); });

		std::shared_ptr<T> popone = std::make_shared<T>(queue_.front());

		queue_.pop();

		return popone;
	}

	bool empty()
	{
		std::lock_guard<std::mutex> lg(mutex_);
		return queue_.empty();
	}

private:
	std::mutex mutex_;
	std::condition_variable cond_;
	std::queue<T> queue_;
};

#endif