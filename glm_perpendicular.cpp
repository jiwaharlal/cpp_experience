#include <iostream>

#include <glm/vec3.hpp>
#include <glm/gtx/perpendicular.hpp>

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/geometries.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/register/linestring.hpp>
#include <boost/geometry/geometries/register/multi_polygon.hpp>
#include <boost/geometry/geometries/register/point.hpp>
#include <boost/geometry/strategies/cartesian/point_in_poly_franklin.hpp>
#include <boost/geometry/strategies/agnostic/point_in_poly_winding.hpp>
#include <boost/geometry/strategies/cartesian/point_in_poly_crossings_multiply.hpp>
#include <boost/geometry/strategies/within.hpp>

namespace bg = boost::geometry;

BOOST_GEOMETRY_REGISTER_POINT_2D(
    glm::dvec3,
    double,
    bg::cs::cartesian,
    x,
    z);

std::ostream& operator <<(std::ostream& out, const glm::dvec3& v)
{
    return out << "(" << v.x << ", " << v.y << ", " << v.z << ")";
}

void testWithin()
{
    glm::dvec3 p1{0., 0., 0.}, p2{0., 0., 1.}, p3{1., 0., 1.}, p4{1., 0., 0.};
    glm::dvec3 p5{0., 0., -1.}, p6{1., 0., 2.};
    bg::model::ring<glm::dvec3> poly{p1, p2, p3, p4, p1};
    bg::model::linestring<glm::dvec3> link{p1, p3};
    bg::model::linestring<glm::dvec3> lane1{p5, p1, p2};
    bg::model::linestring<glm::dvec3> lane2{p3, p4, p6};

    std::cout << "default: " << bg::within(link, poly) << std::endl;
    //std::cout << "winding: " << bg::within(link, poly, bg::strategy::within::winding<glm::dvec3>()) << std::endl;
    std::cout << "franklin: " << bg::within(p1, poly, bg::strategy::within::franklin<glm::dvec3>()) << std::endl;
    std::cout << "crosses: " << bg::crosses(link, poly) << std::endl;
    std::cout << "crosses lanes: " << bg::crosses(link, lane1) << ", " << bg::crosses(link, lane2) << std::endl;
    //std::cout << "default: " << bg::within(link, poly) << std::endl;
}

int main()
{
    const glm::dvec3 start = {0., 0., 0.};
    const glm::dvec3 end = {0., 1., 0.};

    glm::dvec3 vec = end - start;

    std::cout << "vec: " << vec << std::endl;

    const glm::dvec3 normal = {0., 0., 1.};
    //const glm::dvec3 perp = glm::perp(vec, normal);
    auto perp = glm::cross(vec, normal);

    std::cout << "perp: " << perp << std::endl;

    vec = glm::dvec3{1., 0., 0.};

    const glm::dvec3 up{0., 1., 0.};
    const glm::dvec3 down{0., -1., 0.};

    std::cout << "up : " << glm::cross(vec, up) << '\n';
    std::cout << "down : " << glm::cross(vec, down) << '\n';

    testWithin();

    return 0;
}
