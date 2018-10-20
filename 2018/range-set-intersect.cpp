#include <iostream>
#include <iterator>
#include <vector>

#include <boost/range/adaptor/formatted.hpp>
#include <boost/range/algorithm.hpp>
#include <boost/range/irange.hpp>
#include <boost/range/iterator_range.hpp>
#include <range/v3/all.hpp>
#include <range/v3/detail/satisfy_boost_range.hpp>
#include <range/v3/view/set_algorithm.hpp>

namespace boost
{

template <typename ... T>
struct range_iterator<const ranges::v3::detail::set_algorithm_view<T...>>
{
    using type = ranges::iterator_t<const ranges::v3::detail::set_algorithm_view<T...>>;
};

//template <typename Range>
//struct range_iterator
//{
//};

}

int main()
{
    //const auto r1 = boost::irange(1, 8);
    //const auto r2 = boost::irange(5, 22);

    const auto r1 = ranges::view::closed_indices(1, 8);
    const auto r2 = ranges::view::closed_indices(5, 22);

    const auto r3 = ranges::view::set_intersection(r1, r2);
    //const auto r4 = boost::make_iterator_range(r3.begin(), r3.end());
    //std::vector<std::int32_t> v{r4.begin(), r4.end()};

    RANGES_FOR(auto i, r3)
    {
        std::cout << i << ", ";
    }
    std::cout << std::endl;

    //ranges::copy(r3, std::ostream_iterator<std::int32_t>(std::cout, ", "));
    //std::cout << std::endl;

    return 0;
}
