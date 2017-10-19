#include <boost/range/algorithm.hpp>
#include <boost/range/istream_range.hpp>
#include <iostream>
#include <iterator>
#include <vector>
#include <sstream>

struct ItemId
{
    int64_t major_part;
    int32_t minor_part;
};

std::istream& operator >>(std::istream& in, ItemId& id)
{
    char c;
    int64_t major;
    int32_t minor;

    in >> major >> c >> minor;
    if (in.eof())
    {
        return in;
    }

    if (c == '.')
    {
        id.major_part = major;
        id.minor_part = minor;
    }
    else
    {
        in.setstate(std::ios_base::failbit);
    }

    return in;
}

std::ostream& operator <<(std::ostream& out, const ItemId& id)
{
    return out << id.major_part << '.' << id.minor_part;
}

int main(int argc, char** argv)
{
    std::stringstream ss;

    for (int32_t i = 1; i < argc; i++)
    {
        ss << argv[i] << ' ';
    }

    std::vector<ItemId> ids;

    boost::copy(boost::istream_range<ItemId>(ss), std::back_inserter(ids));
    boost::copy(boost::istream_range<ItemId>(std::cin), std::back_inserter(ids));

    std::cout << "Ids:\n";
    boost::copy(ids, std::ostream_iterator<ItemId>(std::cout, ", "));
    std::cout << std::endl;

    return 0;
}
