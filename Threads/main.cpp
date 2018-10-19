//
// Created by pc4 on 2018/10/18.
//

#include "ThreadSafeStack.h"

#include <thread>
#include <vector>
#include <iostream>

int main(int argc, char** argv)
{
    std::vector<std::thread> threads;

    ThreadSafeStack thread_safe_stack;

    int hardware = std::thread::hardware_concurrency();

    std::cout << hardware << std::endl;

    for(int i = 0; i != hardware / 2; ++i)
    {
        threads.emplace_back([]{
            for(int j = 0; j != 1000; ++j)
            {
                thread_safe_stack.push(j);
            }
        });
    }

    for(int i = 0; i != hardware / 2; ++i)
    {
        threads.emplace_back([]{
            for(int j = 0; j != 1000; ++j)
            {
                thread_safe_stack.pop();
            }
        });
    }

    std::cout << thread_safe_stack.size() << std::endl;
}