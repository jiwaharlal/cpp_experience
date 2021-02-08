#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>

template <typename Iter>
Iter findPivotFromCenter(Iter begin, Iter end)
{
    auto mid = begin + (end - begin) / 2;

    auto right_max_it = std::max_element(mid, end);
    auto left_max_it = std::max_element(begin, mid, std::less_equal<int>());

    if (*left_max_it < *right_max_it)
    {
        return right_max_it;
    }
    else if (*right_max_it < *left_max_it)
    {
        return left_max_it;
    }

    if (std::distance(left_max_it, mid) < std::distance(mid, right_max_it))
    {
        return left_max_it;
    }

    return right_max_it;
}

int main()
{
    std::vector<int> v(20, 10);

    std::copy(v.begin(), v.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << std::endl;

    auto pivot_it = std::max_element(v.begin(), v.end());
    std::cout << "Distance to pivot from 0th element: " << std::distance(v.begin(), pivot_it) << std::endl;

    //pivot_it = std::max_element(v.begin(), v.end(), std::less_equal<int>());
    //std::cout << "Distance to pivot from 0th element: " << std::distance(v.begin(), pivot_it) << std::endl;

    pivot_it = findPivotFromCenter(v.begin(), v.end());
    std::cout << "Distance to pivot from 0th element: " << std::distance(v.begin(), pivot_it) << std::endl;

    return 0;
}
