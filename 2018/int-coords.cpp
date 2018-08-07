#include <iostream>
#include <cstdint>
#include <limits>

static const double c_max_uint32 = static_cast<double>(std::numeric_limits<std::uint32_t>::max());

std::uint32_t convertLatFromDouble(double lat)
{
    return (lat + 90.) / 180. * c_max_uint32;
}

std::uint32_t convertLonFromDouble(double lon)
{
    return (lon + 180.) / 360. * c_max_uint32;
}

struct Coord
{
    Coord(double lat, double lon)
        : m_lat(convertLatFromDouble(lat))
        , m_lon(convertLonFromDouble(lon))
    {
    }

    std::uint32_t m_lat;
    std::uint32_t m_lon;
};

int main()
{
    auto i = std::numeric_limits<std::uint32_t>::max();

    std::cout << i << std::endl;
    i++;
    std::cout << i << std::endl;
    i++;
    std::cout << i << std::endl;

    return 0;
}
