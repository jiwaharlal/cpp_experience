#include <string>
#include <chrono>
#include <thread>

#include <boost/geometry/geometries/register/point.hpp>
#include <boost/geometry/geometries/box.hpp>
#include <boost/geometry.hpp>
#include <boost/range/irange.hpp>
#include <glm/glm.hpp>
#include <opencv2/opencv.hpp>

namespace bg = boost::geometry;

BOOST_GEOMETRY_REGISTER_POINT_2D(
        glm::dvec2,
        double,
        bg::cs::cartesian,
        x,
        y);

using Box = bg::model::box<glm::dvec2>;
using Linestring = bg::model::linestring<glm::dvec2>;
using Segment = bg::model::segment<glm::dvec2>;
using Ring = bg::model::ring<glm::dvec2>;

struct Tree
{
    std::vector<glm::dvec2> vertices;
    std::vector<std::pair<std::size_t, std::size_t>> edges;
};

struct Field
{
    Box box;
    std::vector<Ring> obstacles;
};

cv::Point transformPoint(const glm::dvec2& point, const glm::dmat3& transform)
{
    const glm::dvec3 p_3d = {point.x, point.y, 0.};
    const auto transformed = transform * p_3d;

    return {static_cast<std::int32_t>(transformed.x), static_cast<std::int32_t>(transformed.y)};
}

void draw(cv::Mat& mat, const Field& field, const Tree& tree)
{
    const auto& box = field.box;
    glm::dmat3 transition = {1., 0., -box.min_corner().x,
                            0., 1., -box.min_corner().y,
                            0., 0., 1.};

    auto box_diff = box.max_corner() - box.min_corner();
    double scale_x = mat.cols / box_diff.x;
    double scale_y = mat.rows / box_diff.y;

    glm::dmat3 scale = {scale_x, 0., 0.,
                        0., scale_y, 0.,
                        0., 0., 1.};

    auto transform = scale * transition;

    for (const auto& obstacle : field.obstacles)
    {
        std::vector<cv::Point> points;
        points.reserve(obstacle.size());

        for (const auto& p : obstacle)
        {
            const glm::dvec3 p_3d = {p.x, p.y, 0.};
            const auto transformed = transform * p_3d;
            points.emplace_back(transformed.x, transformed.y);
        }

        cv::fillConvexPoly(mat, points.data(), points.size(), {0, 0xff, 0xff});
    }

    for (const auto& edge : tree.edges)
    {
        const auto p1 = tree.vertices[edge.first];
        const auto p2 = tree.vertices[edge.second];

        const auto src = transformPoint(p1, transform);
        const auto dst = transformPoint(p2, transform);
        cv::line(mat, src, dst, {0, 0xff, 0xff}, 1);
    }
}

glm::dvec2 getRandomPoint(const Box& box)
{
    auto range = box.max_corner() - box.min_corner();
    auto x = std::rand() % static_cast<std::int32_t>(range.x) + box.min_corner().x;
    auto y = std::rand() % static_cast<std::int32_t>(range.y) + box.min_corner().y;

    return {x, y};
}

std::size_t findNearest(const Tree& tree, const glm::dvec2& point)
{
    std::size_t nearest_idx = 0u;
    double min_dist = std::numeric_limits<double>::max();

    for (const auto i : boost::irange<std::size_t>(0, tree.vertices.size()))
    {
        auto dist = glm::distance(point, tree.vertices[i]);
        if (dist < min_dist)
        {
            min_dist = dist;
            nearest_idx = i;
        }
    }

    return nearest_idx;
}

void addBranch(Tree& tree, const Field& field, double step_size)
{
    auto rand_conf = getRandomPoint(field.box);
    auto nearest_idx = findNearest(tree, rand_conf);

    const auto src_vertex = tree.vertices[nearest_idx];
    auto new_point = src_vertex + glm::normalize(rand_conf - src_vertex) * step_size;

    tree.vertices.push_back(new_point);
    tree.edges.emplace_back(nearest_idx, tree.vertices.size() - 1);
}

void print(const Field& field, std::ostream& out)
{
    out << bg::wkt(field.box) << '\n';
    out << field.obstacles.size() << '\n';
    for (const auto& obstacle : field.obstacles)
    {
        out << bg::wkt(obstacle) << '\n';
    }
}

int main()
{
    std::srand(std::chrono::high_resolution_clock::now().time_since_epoch().count());

    std::int32_t height = 480;
    std::int32_t width = 640;
    std::string name = "grid";

    cv::Mat frame(cv::Mat::zeros(height, width, CV_8UC3));

    //cv::line(frame, cv::Point2d{10., 10.}, cv::Point2d{500., 200.}, {0xff, 0, 0}, 2);

    Field field;
    field.box = Box{glm::dvec2{0., 0}, glm::dvec2{200., 200}};
    field.obstacles.push_back(Ring{{10., 10.}, {20., 10.}, {20., 20.}, {10., 20.}, {10., 10.}});
    field.obstacles.push_back(Ring{{40., 40.}, {50., 40.}, {50., 50.}, {40., 50.}, {40., 40.}});

    print(field, std::cout);

    glm::dvec2 robot_pos{1., 1.};
    glm::dvec2 goal{190., 190.};

    Tree tree;
    tree.vertices.emplace_back(100, 100);

    for (const auto i : boost::irange(0, 1000))
    {
        addBranch(tree, field, 5.);
        draw(frame, field, tree);
        cv::imshow(name, frame);
        std::this_thread::sleep_for(std::chrono::milliseconds(3));
        cv::waitKey(1);
    }

    cv::waitKey(0);

    return 0;
}
