#include <iostream>
#include <boost/spirit/include/qi.hpp>
#include <boost/range/istream_range.hpp>
#include <boost/spirit/include/support_istream_iterator.hpp>
#include <sstream>

namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;

template <typename Iterator>
bool parse_numbers(Iterator first, Iterator last)
{
    using qi::double_;
    using qi::phrase_parse;
    using ascii::space;
    using qi::int_;
    using qi::char_;

    auto date_parser = int_ >> char_('-') >> int_ >> char_('-') >> int_;
    auto time_parser = int_ >> char_(':') >> int_ >> char_(':') >> int_;

    bool r = phrase_parse(
        first,
        last,
        date_parser >> qi::hold[ +(char_(' '))] >> time_parser,
        space
    );

    if (first != last) // fail if we did not get a full match
    {
        return false;
    }

    return r;
}

int main()
{
    std::stringstream ss;
    ss << "2017-10-11 12:23:47";
    //auto stream_range = boost::istream_range<std::stringstream::char_type>(ss);
    boost::spirit::istream_iterator f(ss), l;

    const std::string& str = ss.str();
    std::cout << "String: " << str << std::endl;
    std::cout << "Parse result: " << std::boolalpha << parse_numbers(f, l) << std::endl;

    return 0;
}
