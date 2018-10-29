#include <iostream>

#include <boost/geometry.hpp>
#include <boost/iterator/iterator_adaptor.hpp>
#include <boost/geometry/geometries/geometries.hpp>
#include <boost/geometry/geometries/register/point.hpp>
#include <glm/glm.hpp>
#include <boost/geometry/iterators/segment_iterator.hpp>

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

template <
    typename PointIterator,
    typename Reference = boost::use_default,
    typename Value = boost::use_default>
class SegmentIterator;

template <typename PointIterator, typename Reference, typename Value>
struct SegmentIteratorBase
{
private:
    using Point = typename boost::iterator_value<PointIterator>::type;
    using reference = bg::model::segment<Point>;
    using cv_value_type = typename boost::iterators::detail::ia_dflt_help<Value, boost::remove_reference<reference>>::type;

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
    : public SegmentIteratorBase<PointIterator, Reference, Value>::type
{
    typedef typename SegmentIteratorBase<PointIterator, Reference, Value>::type super_t;

    friend class boost::iterator_core_access;
public:
    SegmentIterator(const PointIterator& point_it)
        : super_t(point_it)
    {}

private:
    typename super_t::reference dereference() const
    {
        return {*this->base(), *std::next(this->base())};
    }
};

template <typename PointIterator>
SegmentIterator<PointIterator> makeSegmentIterator(const PointIterator& p_it)
{
    return SegmentIterator<PointIterator>(p_it);
}

namespace adaptors
{
    template <typename PointIterator>
    using SegmentRange = boost::iterator_range<SegmentIterator<PointIterator>>;

    template <typename PointIterator>
    SegmentRange<PointIterator> segments(const PointIterator& first, const PointIterator& last)
    {
        return SegmentRange<PointIterator>(first, last);
    }

    template <typename Geometry>
    boost::iterator_range<bg::segment_iterator<Geometry const>> segments(const Geometry& geom)
    {
        return boost::make_iterator_range(bg::segments_begin(geom), bg::segments_end(geom));
    }

    template <typename Range>
    typename boost::range_value<Range>::type front(const Range& r)
    {
        return *boost::begin(r);
    }

    template <typename Range>
    typename boost::range_value<Range>::type back(const Range& r)
    {
        return *std::prev(boost::end(r));
    }
}

int main()
{
    Linestring line = {{0., 0.}, {0., 1.}, {1., 2.}, {2., 2.}};

    ReferringSegment s1{line[0], line[1]};
    ReferringSegment s2{line[1], line[2]};
    ReferringSegment s3{line[2], line[3]};

    //std::cout << bg::wkt(s1) << ", " << bg::wkt(s3) << std::endl;
    //std::cout << bg::distance(s1, s3) << std::endl;

    //auto s_it = SegmentIterator<glm::dvec2>(line, 0);
    auto s_it = makeSegmentIterator(line.begin());
    Segment s = *s_it;
    //std::cout << bg::wkt(*s_it) << std::endl;

    line = Linestring{{0, 0}};

    auto segs = adaptors::segments(line);

    //for (const auto s : adaptors::segments(line))
    //{
        //std::cout << bg::wkt(s) << std::endl;
    //}

    for (auto it = bg::segments_begin(line), last = bg::segments_end(line); it != last; ++it)
    {
        std::cout << bg::wkt(*it) << std::endl;
    }


    //std::cout << bg::wkt(adaptors::back(adaptors::segments(line))) << std::endl;
    std::cout << boost::size(segs) << std::endl;

    return 0;
}
