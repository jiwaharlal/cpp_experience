#include <cstdint>
#include <iostream>

#include <boost/fusion/include/set.hpp>
#include <boost/fusion/container/map.hpp>
#include <boost/fusion/include/map.hpp>
#include <boost/fusion/include/make_map.hpp>
#include <boost/fusion/container/map/map_fwd.hpp>
#include <boost/fusion/include/map_fwd.hpp>
//#include <boost/hana/map.hpp>

//namespace hana = boost::hana;
namespace fusion = boost::fusion;

//auto type_names = hana::make_map(
        //hana::make_pair(hana::type_c<std::int32_t>, "int"),
        //hana::make_pair(hana::type_c<std::uint32_t>, "uint")
        //);

#define DECLARE_MAP(map_name) \
    template <typename T> struct map_name { static const std::string value; };
#define ADD_MAP_VALUE(map_name, key_type, map_value) \
    template <> const std::string map_name<key_type>::value = map_value;

//template <typename T> struct type_names { static const std::string value; };
//template <> const std::string type_names<int>::value = "int";
//template <> const std::string type_names<double>::value = "double";

DECLARE_MAP(type_names);
ADD_MAP_VALUE(type_names, int, "int");
ADD_MAP_VALUE(type_names, double, "double");


//constexpr auto fusion_names = fusion::make_map<int, double>("int", "double");
//typedef fusion::map<
    //fusion::pair<std::int32_t>("int"),
    //fusion::pair<std::uint32_t>("uint")

int main()
{
    //std::cout << hana::at_key(type_names, hana::type_c<std::int32_t>) << std::endl;
    //std::cout << fusion::at_key<int>(fusion_names) << std::endl;

    std::cout << type_names<int>::value << std::endl;

    //fusion::at_key<int>(fusion_names);

    return 0;
}
