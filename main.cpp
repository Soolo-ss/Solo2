#include <iostream>
#include <string>

#include <thread>
#include <vector>

#include "Threads/ThreadSafeStack.h"
#include "Threads/ThreadSafeQueue.h"
#include "Threads/ThreadSafeQueueByList.h"

int main()
{
	using namespace Solo;

	ThreadSafeQueueByListV1<int> stack1;

	std::vector<std::thread> pushthreads;
	std::vector<std::thread> popthreads;


	int hardware = std::thread::hardware_concurrency();

	std::cout << "hardware is " << hardware << std::endl;

	for (int i = 0; i != hardware / 2; ++i)
	{
		pushthreads.emplace_back([&stack1, i]() {
			for (int j = 0; j != 1000; ++j)
			{
				stack1.push(j);
			}
		});
	}

	/*
	for (int i = 0; i != hardware / 2; ++i)
	{
		popthreads.emplace_back([&stack1]() {
			while (true)
			{
				if (stack1.empty())
					std::shared_ptr<int> k = stack1.pop();
			}
		});
	}
	*/

	for (int i = 0; i != hardware / 2; ++i)
	{
		pushthreads[i].join();
	}

	/*
	for (int i = 0; i != hardware / 2; ++i)
	{
		popthreads[i].join();
	}
	*/

    return 0;
}