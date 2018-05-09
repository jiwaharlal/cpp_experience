#include <boost/range/algorithm.hpp>
#include <boost/range/istream_range.hpp>
#include <iostream>
#include <iterator>
#include <sstream>
#include <stdio.h>
#include <unistd.h>
#include <vector>

struct ItemId
{
    int64_t major_part;
    int32_t minor_part;
};

std::ostream& operator <<(std::ostream& out, const ItemId& id)
{
    return out << id.major_part << '.' << id.minor_part;
}

std::istream& operator >>(std::istream& in, ItemId& id)
{
    int64_t major;
    int32_t minor;
    char c;

    if ((in >> major) && (in >> c) && (c == '.') && (in >> minor))
    {
        id.major_part = major;
        id.minor_part = minor;
    }

    return in;
}

int main(int argc, char** argv)
{
    std::stringstream ss;
    for (int32_t i = 1; i < argc; i++)
    {
        ss << argv[i] << ' ';
    }

    std::vector<ItemId> ids{std::istream_iterator<ItemId>(ss),
                            std::istream_iterator<ItemId>()};

    // if stdin connected to pipe
    if (!isatty(fileno(stdin)))
    {
        boost::copy(boost::istream_range<ItemId>(std::cin), std::back_inserter(ids));
    }

    std::cout << "Ids parsed: " << ids.size() << std::endl;

    boost::copy(ids, std::ostream_iterator<ItemId>(std::cout, ", "));
    std::cout << std::endl;

    return 0;
}
