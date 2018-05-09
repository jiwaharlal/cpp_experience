#include <array>
#include <chrono>
#include <iostream>
#include <map>

#include <boost/foreach.hpp>
#include <boost/geometry.hpp>
//#include <boost/geometry.hpp>
#include <boost/geometry/algorithms/expand.hpp>
#include <boost/geometry/geometries/linestring.hpp>
#include <boost/geometry/geometries/multi_point.hpp>
#include <boost/geometry/geometries/multi_polygon.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/geometries/register/point.hpp>
#include <boost/geometry/index/rtree.hpp>
#include <boost/range/adaptors.hpp>
#include <boost/range/algorithm.hpp>
#include <osmium/io/any_input.hpp>
#include <osmium/io/file.hpp>
#include <osmium/io/reader.hpp>
#include <osmium/handler.hpp>
#include <osmium/visitor.hpp>

namespace bg = boost::geometry;
namespace bgi = bg::index;
using ObjectId = osmium::object_id_type;

struct Point
{
    double lat;
    double lon;
};

BOOST_GEOMETRY_REGISTER_POINT_2D(
    Point,
    double,
    bg::cs::spherical_equatorial<bg::degree>,
    lon,
    lat
)

typedef bg::model::box<Point> Box;

using NodeMap = std::map<ObjectId, Point>;

Point convertCoord(const osmium::Location& location)
{
    return {location.lat(), location.lon()};
}

struct NodeCollector : public osmium::handler::Handler
{
    NodeCollector(NodeMap& node_map)
        : m_nodes(node_map)
    {}

    void node(const osmium::Node& node)
    {
        m_nodes.emplace(node.id(), convertCoord(node.location()));
    }

    NodeMap& m_nodes;
};

std::map<ObjectId, Point> readNodes(const char* osm_path)
{
    osmium::io::Reader reader(osm_path);

    NodeMap nodes;
    NodeCollector collector(nodes);
    osmium::apply(reader, collector);

    return nodes;
}

struct RandomPointGenerator
{
    explicit RandomPointGenerator(const Box& box)
        : m_lat(box.min_corner().lat, box.max_corner().lat)
        , m_lon(box.min_corner().lon, box.max_corner().lon)
        , m_re(std::chrono::system_clock::now().time_since_epoch().count())
    {
    }

    Point get()
    {
        return Point{m_lat(m_re), m_lon(m_re)};
    }

    std::uniform_real_distribution<double> m_lat;
    std::uniform_real_distribution<double> m_lon;
    std::default_random_engine m_re;
};

Point findClosest(const std::vector<Point>& points, const Point& target)
{
    return *boost::min_element(points, [target](const Point& lhs, const Point& rhs)
    {
        return bg::comparable_distance(lhs, target) < bg::comparable_distance(rhs, target);
    });
}

//using PTree = bg::index::rtree<Point, bgi::quadratic<16>>;
using PTree = bg::index::rtree<Point, bgi::linear<10>>;

Point findClosest(const PTree& ptree, const Point& target)
{
    std::array<Point, 1> result;
    ptree.query(bgi::nearest(target, 1), result.begin());

    return result[0];
}

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        std::cerr << "Usage <executable> <osm map>" << std::endl;

        return 1;
    }

    auto nodes = readNodes(argv[1]);
    std::cout << "Map nodes size: " << nodes.size() << std::endl;
    std::cout << "Building r-tree..." << std::endl;

    auto start = std::chrono::high_resolution_clock::now();
    auto point_range = nodes | boost::adaptors::map_values;
    PTree node_tree(point_range.begin(), point_range.end());

    //for (const auto& node_pair : nodes)
    //{
        //node_tree.insert(node_pair.second);
    //}

    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Time spent: " << (end - start).count() << std::endl;

    std::cout << "Calculating bounding box..." << std::endl;

    std::vector<Point> points;
    points.reserve(nodes.size());
    boost::copy(boost::adaptors::values(nodes), std::back_inserter(points));

    Box bounding_box(points.front(), points.front());
    for (const auto& p : points)
    {
        bg::expand(bounding_box, p);
    }

    std::cout << std::setprecision(11);
    std::cout << "Bounding box: " << boost::geometry::wkt(bounding_box) << std::endl;

    RandomPointGenerator point_gen(bounding_box);

    for (int i = 0; i < 5; i++)
    {
        auto p = point_gen.get();
        std::cout << "Looking for point " << bg::wkt(p) << " in vector" << std::endl;

        auto start = std::chrono::high_resolution_clock::now();
        auto closest = findClosest(points, p);
        auto end = std::chrono::high_resolution_clock::now();

        std::cout << "Closest point by min_element: " << bg::wkt(closest) << std::endl;
        std::cout << "Time spent: " << (end - start).count() << std::endl;

        start = std::chrono::high_resolution_clock::now();
        closest = findClosest(node_tree, p);
        end = std::chrono::high_resolution_clock::now();

        std::cout << "Closest point by r-tree: " << bg::wkt(closest) << std::endl;
        std::cout << "Time spent: " << (end - start).count() << "\n----------------" << std::endl;
    }

    return 0;
}
