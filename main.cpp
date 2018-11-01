#include "Algorithm/SelectionSort.h"

#include <vector>

int main()
{
    std::vector<int> a { 3, 5, 3,123, 432, 32};

    selection_sort(a.begin(), a.end());

    return 0;
}