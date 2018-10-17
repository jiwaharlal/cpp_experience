#include <cstdint>
#include <iostream>
#include <string>

#include <boost/variant.hpp>

using IntStrVariant = boost::variant<std::string, std::int32_t>;

int main()
{
    IntStrVariant v1 = 230;
    IntStrVariant v2 = std::string("hello");
    IntStrVariant v3 = std::string("w");
    IntStrVariant v4 = 11111111;

    std::cout << (v1 < v2) << std::endl;
    std::cout << (v2 < v3) << std::endl;
    std::cout << (v3 < v2) << std::endl;
    std::cout << (v3 < v4) << std::endl;
    std::cout << (v1 < v4) << std::endl;

    return 0;
}
