#include <iostream>
#include <vector>
#include <cstdint>
#include <iterator>

#include <boost/range/adaptors.hpp>
#include <boost/range/algorithm.hpp>
#include <boost/range/irange.hpp>

int main()
{
    const auto multiplier = boost::adaptors::transformed([](std::int32_t i){ return i * 3; });
    const auto odd_filter = boost::adaptors::filtered([](std::int32_t i){ return i % 2 != 0; });

    const auto multi_filter = multiplier | odd_filter;

    boost::copy(
            boost::irange(0, 20) | multi_filter,
            std::ostream_iterator<std::int32_t>(std::cout, ", "));
    std::cout << std::endl;

    return 0;
}
