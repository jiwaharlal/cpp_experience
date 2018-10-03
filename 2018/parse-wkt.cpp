#include <iostream>
#include <regex>

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/register/point.hpp>
#include <boost/geometry/io/wkt/stream.hpp>
#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/geometries/linestring.hpp>
#include <boost/tuple/tuple_io.hpp>

namespace bg = boost::geometry;

struct Point
{
    double x, y;
};

BOOST_GEOMETRY_REGISTER_POINT_2D(Point, double, cs::cartesian, x, y);

using Linestring = bg::model::linestring<Point>;

Point parseWktPoint(const std::string& wkt_string)
{
    Point p;
    bg::read_wkt(wkt_string, p);

    return p;
}

Point parseFullString(const std::string& geom_msg)
{
    std::regex r("wkt_geometry\\{(.*)\\}");
    std::cmatch m;
    if (!std::regex_search(geom_msg.c_str(), m, r))
    {
        throw std::runtime_error("Bad geometry string");
    };

    return parseWktPoint(*std::next(m.begin()));
}

std::ostream& operator <<(std::ostream& out, const Point& p)
{
    return out << '(' << p.x << ", " << p.y << ')';
    //return out << boost::tie(p.x, p.y);
}

int main()
{
    const std::string point_str = "POINT(1 2)";
    //const std::string linestring_str = "LINESTRING(0 0,2 2,3 1)";

    std::string full_str = "Some [Context] message wkt_geometry{" + point_str + "}";

    //auto geom = parseWktPoint(full_str);
    auto geom = parseFullString(full_str);

    std::cout << geom << std::endl;

    return 0;
}
