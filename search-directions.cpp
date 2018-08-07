#include <iostream>
#include <vector>
#include <tuple>

#include <boost/range/irange.hpp>
#include <boost/range/algorithm_ext/push_back.hpp>
#include <boost/tuple/tuple.hpp>

template <typename MajorType, typename MinorType>
struct CombinedId : public std::pair<MajorType, MinorType>
{
    using ParentPair = std::pair<MajorType, MinorType>;
    MajorType& majorPart() { return ParentPair::first; }
    const MajorType& majorPart() const { return ParentPair::first; }
    MinorType& minorPart() { return ParentPair::second; }
    const MinorType& minorPart() const { return ParentPair::second; }
};

using ItemId = CombinedId<std::int64_t, std::int32_t>;

int main()
{
    std::cout << sizeof(std::pair<std::int64_t, std::int32_t>) << '\n';
    std::cout << sizeof(ItemId) << '\n';

    return 0;
}
