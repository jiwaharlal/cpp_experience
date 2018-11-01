#include <iostream>
#include <iterator>
#include <vector>

#include <boost/range/adaptors.hpp>
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/geometries.hpp>
#include <boost/geometry/geometries/register/point.hpp>
#include <boost/range/algorithm.hpp>
#include <boost/tuple/tuple_io.hpp>
#include <glm/glm.hpp>

namespace bg = boost::geometry;

BOOST_GEOMETRY_REGISTER_POINT_2D(
        glm::dvec2,
        double,
        bg::cs::cartesian,
        x,
        y);

using Linestring = bg::model::linestring<glm::dvec2>;
using Ring = bg::model::ring<glm::dvec2>;
using Segment = bg::model::segment<glm::dvec2>;

namespace glm
{

std::ostream& operator <<(std::ostream& out, const dvec2& p)
{
    return out << boost::tie(p.x, p.y);
}

}

namespace std
{

ostream& operator <<(ostream& out, const vector<glm::dvec2>& v)
{
    boost::copy(v, ostream_iterator<glm::dvec2>(out, ", "));
    return out;
}

ostream& operator <<(ostream& out, const Segment& s)
{
    return out << boost::tie(s.first, s.second);
}

}

int main()
{
    Ring r{{1., 1.}, {2., 4.}, {3., 4.}, {3., 1.}, {1., 1.}};
    Segment s{{0., 3.}, {2., 3.}};
    Segment enter_cut{r[0], r[1]};

    std::vector<glm::dvec2> intersections;
    bg::intersection(s, enter_cut, intersections);

    std::cout << "Intersections: " << boost::adaptors::format(intersections) << std::endl;

    intersections.front().x -= std::numeric_limits<double>::epsilon() * 2.;
    Linestring l{intersections.front(), s.second};
    Linestring l2{{0., 0.}, {5., 5.}};
    Linestring l3{{0., 5.}, {5., 0.}};

    std::vector<Linestring> seg_intersections;
    bg::intersection(l3, l2, seg_intersections);

    std::cout << "seg_intersections: " << boost::adaptors::format(seg_intersections) << std::endl;

    std::cout << "Within: " << bg::within(l, r) << std::endl;

    return 0;
}
