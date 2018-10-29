#include <iostream>

#include <boost/geometry.hpp>
#include <boost/iterator/iterator_adaptor.hpp>
#include <boost/geometry/geometries/geometries.hpp>
#include <boost/geometry/geometries/register/point.hpp>
#include <glm/glm.hpp>

namespace bg = boost::geometry;

BOOST_GEOMETRY_REGISTER_POINT_2D(
        glm::dvec2,
        double,
        bg::cs::cartesian,
        x,
        y);

using Linestring = bg::model::linestring<glm::dvec2>;
using Segment = bg::model::segment<glm::dvec2>;
using ReferringSegment = bg::model::referring_segment<const glm::dvec2>;

template <typename PointIterator, typename Reference = boost::use_default, typename Value = boost::use_default>
class SegmentIterator;

template <typename PointIterator, typename Reference, typename Value>
struct SegmentIteratorBase
{
private:
    using reference = boost::ia_dflt_help<Reference, result_of<>>;
    using cv_value_type = boost::ia_fdlt_help<Value, boost::remove_reference<reference>>::type;

public:
    using type = boost::iterator_adaptor<
        SegmentIterator<PointIterator, Reference, Value>,
        PointIterator,
        cv_value_type,
        boost::use_default,
        reference>;
};

template <typename PointIterator, typename Reference, typename Value>
class SegmentIterator
    : public boost::iterator_adaptor<
        SegmentIterator<PointIterator, Reference, Value>,
        PointIterator,
        >;
{
private:


public:
    SegmentIterator(const PointIterator& point_it)
        : m_first_it(point_it)
    {}

    ReferringSegment dereference()
    {
        return {*m_first_it, *std::next(m_first_it)};
    }

public: // types
    typedef boost::iterator_adaptor<
        SegmentIterator,
        PointIterator,


private:
    PointIterator m_first_it;
};

//boost::range::iterator_range<> segments(const Linestring& linestring)
//{
//}

int main()
{
    Linestring line = {{0., 0.}, {0., 1.}, {1., 2.}, {2., 2.}};

    ReferringSegment s1{line[0], line[1]};
    ReferringSegment s2{line[1], line[2]};
    ReferringSegment s3{line[2], line[3]};
    //Segment s3 = s1;

    std::cout << bg::wkt(s1) << ", " << bg::wkt(s3) << std::endl;
    std::cout << bg::distance(s1, s3) << std::endl;

    auto s_it = SegmentIterator(line, 0);
    std::cout << bg::wkt(*s_it) << std::endl;


    return 0;
}
