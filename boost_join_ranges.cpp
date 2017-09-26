#include <vector>
#include <iostream>
#include <iterator>
#include <boost/range/join.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/range/algorithm.hpp>
#include <boost/range/combine.hpp>
#include <boost/range/adaptors.hpp>

template <typename IteratorT>
boost::iterator_range<IteratorT> make_single_element_range(IteratorT it)
{
    return boost::make_iterator_range(it, std::next(it));
}

struct Segment
{
    int begin;
    int end;
};

std::ostream& operator <<(std::ostream& out, const Segment& s)
{
    return out << "s(" << s.begin << ", " << s.end << ")";
}

Segment makeSegment(const boost::tuple<int, int>& t)
{
    return Segment{t.get<0>(), t.get<1>()};
}

int main()
{
    Segment s0{0};
    std::cout <<

    std::vector<int> v = {1, 2, 3, 4, 5};
    //int u = 42;
    //auto joined = boost::join(v, make_single_element_range(v.begin()));
    auto joined = boost::join(v, boost::make_iterator_range_n(v.begin(), 1));
    //auto long_joined = boost::join(joined, boost::make_iterator_range_n(&u, 1));

    //boost::copy(long_joined, std::ostream_iterator<int>(std::cout, ", "));
    std::cout << std::endl;

    auto combined = boost::combine(
            boost::make_iterator_range(joined.begin(), std::prev(joined.end())),
            boost::make_iterator_range(std::next(joined.begin()), joined.end()));

    //auto combined = boost::make_iterator_range(
            //boost::make_zip_iterator(boost::make_tuple(joined.begin(), std::next(joined.begin()))),
            //boost::make_zip_iterator(boost::make_tuple(std::prev(joined.end()), joined.end())));

    for (const auto& p : combined)
    {
        std::cout << p.get<0>() << ", " << p.get<1>() << std::endl;
    }

    using boost::adaptors::transformed;
    boost::copy(combined | transformed(&makeSegment), std::ostream_iterator<Segment>(std::cout, ", "));

    return 0;
}
