#include <cstdint>
#include <iostream>

#include <boost/concept_check.hpp>
#include <boost/concept/requires.hpp>
#include <boost/range.hpp>
#include <boost/range/adaptors.hpp>
#include <boost/range/concepts.hpp>
#include <boost/range/iterator_range.hpp>

template <typename RangeType>
BOOST_CONCEPT_REQUIRES(
    (( boost::BidirectionalRangeConcept<RangeType> )),
    (std::size_t))
rangeSize(const RangeType& range) //-> decltype(boost::size(range))
{
    //BOOST_CONCEPT_ASSERT(( boost::BidirectionalRangeConcept<RangeType> ));
    return boost::size(range);
}

int main()
{
    std::vector<int> v = {1, 2, 3, 4, 5};

    auto r = boost::make_iterator_range(std::next(boost::begin(v)), std::prev(boost::end(v)));
    using boost::adaptors::transformed;
    auto r2 = r | transformed([](const int i){ return i * 2; });
    std::cout << "size: " << rangeSize(r2) << '\n';

    return 0;
}
