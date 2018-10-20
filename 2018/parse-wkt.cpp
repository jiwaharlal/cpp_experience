#include <iostream>
#include <regex>

#include <boost/geometry.hpp>
#include <boost/geometry/algorithms/within.hpp>
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
using Ring = bg::model::ring<Point>;
using Polygon = bg::model::polygon<Point>;

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
    const std::string ring_str = "POLYGON((1 2,1 3,2 3,2 2,1 2))";


    std::string full_str = "Some [Context] message wkt_geometry{" + point_str + "}";

    //auto geom = parseWktPoint(full_str);
    auto geom = parseFullString(full_str);

    std::cout << geom << std::endl;

    Ring ring;
    ring.push_back(Point{1., 2.});

    std::cout << bg::wkt(ring) << std::endl;

    Polygon poly;
    bg::read_wkt(ring_str, poly);
    std::cout << bg::wkt(poly) << std::endl;

    std::cout << bg::within(Point{1.5, 2.5}, poly) << std::endl;

    ring = poly.outer();

    std::cout << bg::wkt(ring) << std::endl;

    return 0;
}
