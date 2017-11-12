#include <iostream>

template <typename T>
struct TLatitude
{
    explicit TLatitude(const T& v) : value{v} {}

    T value;
};

template <typename T>
struct TLongitude
{
    explicit TLongitude(const T& v) : value(v) {}

    T value;
};

template <typename T>
struct TWgs84d2
{
    explicit TWgs84d2(const TLatitude<T>& lat, const TLongitude<T>& lon)
        : lat{lat}, lon{lon}
    {}

    TLatitude<T> lat;
    TLongitude<T> lon;
};

using default_float = double;
using Latitude = TLatitude<default_float>;
using Longitude = TLongitude<default_float>;
using Wgs84d2 = TWgs84d2<default_float>;

int main()
{
    return 0;

    Wgs84d2 coord(Latitude(30), Longitude(50.));
}
