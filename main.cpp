#include "Algorithm/insertion_sort.h"

#include <vector>
#include <iostream>

int main()
{
    std::vector<int> a { 523, 222, 7, 7432, 32};

    merge_sort(a.begin(), a.end());

    for(auto x : a) std::cout << x << " ";

    return 0;
}