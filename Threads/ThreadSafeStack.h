#ifndef SOLO_THREAD_SAFE_STACK_H
#define SOLO_THREAD_SAFE_STACK_H

#include <mutex>
#include <stack>
#include <memory>

#include <iostream>

namespace Solo
{
	/*
	unsafe one
	becasue between empty and top,  between top and pop,  there exits a data race
	*/
	template<typename T>
	class ThreadSafeStackV1
	{
	public:
		ThreadSafeStackV1()
		{ }

		ThreadSafeStackV1(const ThreadSafeStackV1&) = delete;
		ThreadSafeStackV1& operator=(const ThreadSafeStackV1&) = delete;

		bool empty()
		{
			std::lock_guard<std::mutex> lg(mutex_);
			return stack_.empty();
		}

		void push(const T newValue)
		{
			std::lock_guard<std::mutex> lg(mutex_);
			stack_.push(std::move(newValue));
		}

		void pop()
		{
			std::lock_guard<std::mutex> lg(mutex_);
			stack_.pop();
		}

		const T& top()
		{
			std::lock_guard<std::mutex> lg(mutex_);
			return stack_.top();
		}

	private:
		std::mutex mutex_;
		std::stack<T> stack_;
	};

	/*
	* a simple one threadsafe stack
	*/
	template<typename T>
	class ThreadSafeStackV2
	{
	public:
		struct EmptyStack : std::exception
		{
			const char* what() const noexcept {
				return "empty stack";
			}
		};

		ThreadSafeStackV2() { }

		ThreadSafeStackV2(const ThreadSafeStackV2&) = delete;
		ThreadSafeStackV2& operator=(const ThreadSafeStackV2&) = delete;

		bool empty() 
		{
			std::lock_guard<std::mutex> lg(mutex_);
			return stack_.empty();
		}

		//��������ָ�� ���⿽�����캯�������׳����쳣 ���stack�����ݳ�Ա���ƻ�
		std::shared_ptr<T> pop()
		{
			std::lock_guard<std::mutex> lg(mutex_);

			//���Ϊ�մ�ʱֱ���׳��쳣 ����V1�汾����������
			if (stack_.empty())
			{
				throw EmptyStack();
			}

			std::shared_ptr<T> popone = std::make_shared<T>(stack_.top());

			stack_.pop();
			return popone;
		}

		template <typename U>
		void push(U&& newV)
		{
			std::lock_guard<std::mutex> lg(mutex_);

			stack_.push(std::forward<U>(newV));
		}
	private:
		std::mutex mutex_;
		std::stack<T> stack_;
	};
}

#endif