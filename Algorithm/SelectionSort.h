//
// Created by 46127 on 2018/11/1.
//

#ifndef SOLO_SELECTIONSORT_H
#define SOLO_SELECTIONSORT_H

#include <algorithm>

template <class ForwardIt>
void selection_sort(ForwardIt begin, ForwardIt end)
{
    for (ForwardIt i = begin; i != end; ++i)
    {
        std::iter_swap(i, std::min_element(i, end));
    }
}

#endif //SOLO_SELECTIONSORT_H
