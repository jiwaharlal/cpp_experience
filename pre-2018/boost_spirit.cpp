#include <boost/range/istream_range.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/support_istream_iterator.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <boost/phoenix.hpp>

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

    auto date_parser = int_ >> '-' >> int_ >> '-' >> int_;
    auto time_parser = int_ >> char_(':') >> int_ >> char_(':') >> int_;

    bool r = phrase_parse(
        first,
        last,
        //date_parser >> qi::hold[ +(char_(' '))] >> time_parser,
        date_parser, // >> char_('Z') >> time_parser,
        space
    );

    std::cout << "r=" << std::boolalpha << r << std::endl;

    if (first != last) // fail if we did not get a full match
    {
        return false;
    }

    return r;
}

template <typename Iterator>
std::tm parseDate(Iterator first, Iterator last)
{
    using qi::phrase_parse;
    using qi::int_;
    using qi::char_;
    using boost::phoenix::ref;
    using ascii::space;

    std::tm result;

    bool r = phrase_parse(first, last,
            (int_[ref(result.tm_year) = qi::_1] >> '-'
            >> int_[ref(result.tm_mon) = qi::_1] >> '-'
            >> int_[ref(result.tm_mday) = qi::_1]), space);
    result.tm_year -= 1900;
    result.tm_mon--;

    std::cout << "Date parsed: " << std::boolalpha << r << std::endl;

    return result;
}

template <typename Iterator>
std::tm parseDateTime(Iterator first, Iterator last)
{
    using qi::phrase_parse;
    using qi::int_;
    using qi::char_;
    using boost::phoenix::ref;
    //using ascii::space;

    std::tm result;

    bool r = phrase_parse(first, last,
            (int_[ref(result.tm_year) = qi::_1] >> '-'
            >> int_[ref(result.tm_mon) = qi::_1] >> '-'
            >> int_[ref(result.tm_mday) = qi::_1] >> 't'
            >> int_[ref(result.tm_hour) = qi::_1] >> ':'
            >> int_[ref(result.tm_min) = qi::_1] >> ':'
            >> int_[ref(result.tm_sec) = qi::_1]), char_('"'));

    result.tm_year -= 1900;
    result.tm_mon--;

    std::cout << "Date parsed: " << std::boolalpha << r << std::endl;

    return result;
}

std::ostream& operator <<(std::ostream& os, const std::tm& time)
{
    return os << std::put_time(&time, "%Y-%m-%d %H:%M:%S");
}

int main()
{
    //std::stringstream ss;
    std::string str = "\"2017-10-11t12:23:47z\"";
    //boost::trim
    //ss << "2017-10-11";
    //auto stream_range = boost::istream_range<std::stringstream::char_type>(ss);
    //boost::spirit::istream_iterator f(ss), l;

    //const std::string& str = ss.str();
    std::cout << "String: " << str << std::endl;
    //std::cout << "Parse result: " << std::boolalpha << parse_numbers(f, l) << std::endl;
    auto date_time = parseDateTime(str.begin(), str.end());


    std::cout << "Date: " << date_time << std::endl;

    std::cout << "Now trying to parse with std::get_time" << std::endl;
    std::stringstream ss;
    ss << str;
    std::tm dt;
    ss >> std::get_time(&dt, "\"%Y-%m-%dt%H:%M:%Sz\"");
    std::cout << "date: " << dt << std::endl;

    return 0;
}
