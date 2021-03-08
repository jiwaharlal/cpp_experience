#include <iostream>

template <typename Iter>
inplaceMerge(Iter begin, Iter mid, Iter end)
{
    auto it1 = begin;
    auto it2 = mid;
    for ( ; it1 != it2; )
    {
        if (*it2 < *it1)
        {
            std::swap(*it1, *it2);
            ++it2;
        }

        ++it1;
    }
}

template <typename Iter>
void inplaceMergeSort(Iter begin, Iter end)
{
    if (std::next(begin) == end)
    {
        return;
    }

    auto mid = begin + (end - begin) / 2;

    inplaceMergeSort(begin, mid);
    inplaceMergeSort(mid, end);

    inplaceMerge(begin, mid, end);
}
