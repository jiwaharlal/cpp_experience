#include <iostream>
#include <list>

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/linestring.hpp>
#include <boost/geometry/geometries/register/point.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/geometries/multi_point.hpp>
#include <boost/geometry/geometries/multi_polygon.hpp>
#include <boost/geometry/algorithms/expand.hpp>
#include <boost/geometry.hpp>

#include <boost/foreach.hpp>

namespace bg = boost::geometry;

struct Point
{
    double lat;
    double lon;
};

BOOST_GEOMETRY_REGISTER_POINT_2D(
    Point,
    double,
    boost::geometry::cs::spherical_equatorial<boost::geometry::degree>,
    lon,
    lat
)

using PointModel = Point;
using BoxModel = boost::geometry::model::box<PointModel>;

int main()
{
    typedef boost::geometry::model::d2::point_xy<double> point_type;
    typedef boost::geometry::model::polygon<point_type> polygon_type;
    typedef boost::geometry::model::linestring<point_type> linestring_type;
    typedef boost::geometry::model::multi_point<point_type> multi_point_type;
    typedef boost::geometry::model::box<point_type> box_type;
    //typedef boost::geometry::model::ring

    point_type p(1,2);
    box_type box;
    polygon_type poly;
    linestring_type line;
    multi_point_type mp;

    boost::geometry::read_wkt(
        "POLYGON((2 1.3,2.4 1.7,2.8 1.8,3.4 1.2,3.7 1.6,3.4 2,4.1 3,5.3 2.6,5.4 1.2,4.9 0.8,2.9 0.7,2 1.3)"
            "(4.0 2.0, 4.2 1.4, 4.8 1.9, 4.4 2.2, 4.0 2.0))", poly);
    line.push_back(point_type(0,0));
    line.push_back(point_type(0,3));
    mp.push_back(point_type(0,0));
    mp.push_back(point_type(3,3));

    std::cout
        << "Point-Poly: " << boost::geometry::distance(p, poly) << std::endl
        << "Point-Line: " << boost::geometry::distance(p, line) << std::endl
        << "Point-MultiPoint: " << boost::geometry::distance(p, mp) << std::endl;

    using boost::geometry::expand;
    expand(box, point_type(0, 0));
    expand(box, point_type(2, 2));

    std::cout << boost::geometry::covered_by(point_type(1, 1), box) << std::endl;
    std::cout << boost::geometry::covered_by(point_type(3, 1), box) << std::endl;
    std::cout << boost::geometry::distance(point_type(5, 4), box) << std::endl;

    //auto perimeter = boost::geometry::perimeter(box);
    box_type buffer;
    bg::buffer(box, buffer, 0.5);
    std::cout << bg::distance(point_type(3, 1), buffer) << std::endl;

    linestring_type border;
    bg::convex_hull(box, border);
    //bg::envelope(box, border);

    //std::vector<polygon_type> border;
    //bg::difference(box, buffer, border);

    std::cout << bg::distance(point_type(1.5, 1.8), border) << std::endl;
    std::cout << "Within box: " << bg::within(point_type(1, 1), box) << std::endl;
    std::cout << "Coverd by box: " << bg::covered_by(point_type(1, 1), box) << std::endl;

    BoxModel b;
    bg::expand(b, Point{46.453, 30.6766});
    bg::expand(b, Point{46.4587, 30.6848});

    std::cout << "Covered by : " << bg::covered_by(Point{46.4567, 30.6812}, b) << std::endl;

    return 0;
}
