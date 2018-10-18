#include <iostream>
#include <string>

#include <thread>
#include <vector>

#include "Threads/ThreadSafeStack.h"

int main()
{
	using namespace Solo;

	ThreadSafeStackV2<int> stack1;

	std::vector<std::thread> pushthreads;
	std::vector<std::thread> popthreads;
	
	int hardware = std::thread::hardware_concurrency();

	std::cout << "hardware is " << hardware << std::endl;

	for (int i = 0; i != hardware / 2; ++i)
	{
		pushthreads.emplace_back([&stack1, i]() {
			for (int j = 0; j != 10000; ++j)
			{
				stack1.push(j);
			}
		});
	}

	for (int i = 0; i != hardware / 2; ++i)
	{
		popthreads.emplace_back([&stack1]() {
			while (true)
			{
				if (!stack1.empty())
				{
					try
					{
						std::shared_ptr<int> k = stack1.pop();
					}
					catch (ThreadSafeStackV2<int>::EmptyStack e)
					{
						continue;
					}
				}
			}
		});
	}

	for (int i = 0; i != hardware / 2; ++i)
	{
		pushthreads[i].join();
	}

	for (int i = 0; i != hardware / 2; ++i)
	{
		popthreads[i].join();
	}

    return 0;
}