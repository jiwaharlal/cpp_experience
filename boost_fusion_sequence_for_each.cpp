#include <boost/fusion/include/algorithm.hpp>
#include <boost/fusion/include/at_c.hpp>
#include <boost/fusion/include/define_struct.hpp>
#include <boost/fusion/include/at.hpp>
#include <boost/fusion/mpl/at.hpp>
#include <boost/fusion/sequence.hpp>
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/range_c.hpp>
#include <boost/range/algorithm/copy.hpp>
#include <functional>
#include <iostream>
#include <iterator>
#include <vector>

BOOST_FUSION_DEFINE_STRUCT(
    , MapData,
    (int, id)
    (std::string, name)
    )

struct Printer
{
    template <typename T> void operator ()(const T& value) const
    {
        std::cout << "print: " << value << std::endl;
    }
};

struct PrintIndex
{
    PrintIndex(MapData& md) : m_md(md) {}

    template <int i> void operator ()(const boost::mpl::integral_c<int, i>&)
    {
        std::cout << "index i = " << i << std::endl;
        std::cout << "value at i = " << boost::fusion::at_c<i>(m_md) << std::endl;
    }

    MapData& m_md;
};

int main()
{
    MapData md = {4, "hello"};

    boost::fusion::for_each(md, Printer());

    typedef boost::mpl::range_c<int, 0, boost::fusion::result_of::size<MapData>::value> md_range;
    //boost::mpl::for_each<md_range>();
    boost::mpl::for_each<md_range>(PrintIndex(md));

    std::vector<int> v(10, 10);
    //std::reference_wrapper<int> ri(v[5]);
    auto ri = std::ref(v[5]);

    ri.get() = 45;

    boost::copy(v, std::ostream_iterator<int>(std::cout, ", "));
    std::cout << std::endl;

    return 0;
}
