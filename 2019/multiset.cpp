#include <set>
#include <iostream>
#include <boost/range/irange.hpp>
#include <boost/range/adaptors.hpp>

int main()
{
    std::multiset<int> s;

    for (auto i : boost::irange(0, 10))
    {
        for (auto j : boost::irange(0, 3))
        {
            s.insert(i);
        }
    }

    std::cout << boost::adaptors::format(s) << std::endl;

    s.erase(std::prev(s.end()));

    std::cout << boost::adaptors::format(s) << std::endl;

    s.erase(std::prev(s.end()));

    std::cout << boost::adaptors::format(s) << std::endl;

    return 0;
}
