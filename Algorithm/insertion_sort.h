//
// Created by 46127 on 2018/11/1.
//

#ifndef SOLO_INSERTION_SORT_H
#define SOLO_INSERTION_SORT_H

#include <algorithm>

template <typename ForwardIt>
void insertion_sort(ForwardIt begin, ForwardIt end)
{
    if (begin + 1 >= end)
        return;

    for(auto i = begin + 1; i != end; ++i)
    {
        auto fend = i - 1;
        auto pos = std::lower_bound(begin, fend, *i);
        if (pos != fend)
        {
            
        }
    }
}

#endif //SOLO_INSERTION_SORT_H
