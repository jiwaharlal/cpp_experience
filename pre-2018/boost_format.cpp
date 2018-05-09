#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/string_generator.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <iostream>
#include <sstream>

int main()
{
    boost::format base("bbox=[%1$.11d,%2$.11d,%3$.11d,%4$.11d]");
    std::cout << base % 10.23456789123456 % 3 % 4 % 5 << std::endl;

    boost::format request_base("http://overpass.osm.rambler.ru/cgi/xapi_meta?*[bbox=%1$.11d,%2$.11d,%3$.11d,%4$.11d]");

    std::string request = boost::lexical_cast<std::string>(base % 10.23456789123456 % 3 % 4 % 5);

    std::string uid = boost::lexical_cast<std::string>(boost::uuids::random_generator()());
    //boost::uuids::string_generator gen;
    //gen(uid);
    std::stringstream ss;
    ss << "UID: " << uid << std::endl;
    std::cout << ss.str();

    return 0;
}
