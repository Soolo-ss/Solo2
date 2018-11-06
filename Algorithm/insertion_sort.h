//
// Created by 46127 on 2018/11/1.
//

#ifndef SOLO_INSERTION_SORT_H
#define SOLO_INSERTION_SORT_H

#include <algorithm>

template <class ForwardIt>
void selection_sort(ForwardIt begin, ForwardIt end)
{
    for (ForwardIt i = begin; i != end; ++i)
    {
        std::iter_swap(i, std::min_element(i, end));
    }
}

template <typename ForwardIt>
void insertion_sort(ForwardIt begin, ForwardIt last)
{
    for(ForwardIt i = begin; i != last; ++i)
    {
        std::rotate(std::lower_bound(begin, *i), i, i);
    }
}

template <typename RandomIter>
void merge_sort(RandomIter begin, RandomIter last)
{
    if (begin == last)
        return;

    RandomIter middle = (begin + last) / 2;
    merge_sort(begin, middle);
    merge_sort(middle, last);

    std::inplace_merge(begin, middle, last);

    return;
}

template <typename ForwardIter>
void quick_sort(ForwardIter begin, ForwardIter last)
{
    if (begin == last)
        return;

    auto pivot = *begin;

    using elem_type = typename std::iterator_traits<ForwardIter>::value_type;

    //错误 因为partition不保证middle这个迭代器指向pivot这个元素 后面的元素是乱序的
    //ForwardIter middle = std::partition(begin, last, [pivot](const elem_type & elem) { return elem < pivot; });

    //正确 这样保证pivot这个元素在两个子数组的中间
    ForwardIter middle1 = std::partition(begin, last, [pivot](const elem_type& elem) { return elem < pivot; });
    ForwardIter middle2 = std::partition(middle1, last, [pivot](const elem_type& elem) { return elem <= pivot; });

    quick_sort(begin, middle1);
    quick_sort(middle2, last);
}

#endif //SOLO_INSERTION_SORT_H
