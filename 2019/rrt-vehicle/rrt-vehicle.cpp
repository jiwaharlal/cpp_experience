#include <iostream>
#include <chrono>
//#include <random>
//#include <string>
#include <thread>
#include <algorithm>

#include <boost/algorithm/clamp.hpp>
#include <boost/bind.hpp>
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/box.hpp>
#include <boost/geometry/geometries/register/point.hpp>
#include <boost/range/algorithm.hpp>
#include <boost/range/algorithm_ext.hpp>
#include <boost/range/adaptor/formatted.hpp>
#include <boost/range/adaptor/transformed.hpp>
#include <boost/range/irange.hpp>
#include <glm/glm.hpp>
#include <opencv2/opencv.hpp>

#include "vehicle.hpp"

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

struct Field
{
    Box box;
    std::vector<Ring> obstacles;
};

Field createField()
{
    Field field;
    field.box = Box{glm::dvec2{0., 0}, glm::dvec2{200., 200}};

    //field.obstacles.push_back(Ring{{10., 10.}, {20., 10.}, {20., 20.}, {10., 20.}, {10., 10.}});
    //field.obstacles.push_back(Ring{{40., 40.}, {50., 40.}, {50., 50.}, {40., 50.}, {40., 40.}});
    //field.obstacles.push_back(
            //Ring{{0, 60}, {60, 60}, {60, 10}, {65, 10}, {65, 65}, {0, 65}, {0, 60}});
    //field.obstacles.push_back(
            //Ring{{100, 0}, {105, 0}, {105, 105}, {10, 105}, {10, 100}, {100, 100}, {100, 0}});

    return field;
}

Vehicle initVehicle()
{
    Vehicle v;
    v.dimensions.length = 10.;
    v.dimensions.width = 5.;

    v.origin = {1.5, 2.5};

    v.state.position = {100., 100.};
    v.state.orientation = M_PI / 6.;
    v.state.steering = 0.;

    return v;
}

cv::Point transformPoint(const glm::dvec2& point, const glm::dmat3& transform)
{
    const glm::dvec3 p_3d = {point.x, point.y, 1.};
    const auto transformed = transform * p_3d;

    return {static_cast<std::int32_t>(transformed.x), static_cast<std::int32_t>(transformed.y)};
}

std::ostream& operator <<(std::ostream& out, const cv::Point& p)
{
    return out << '(' << p.x << ", " << p.y << ')';
}

glm::dmat3 createFieldTransform(const cv::Mat& mat, const Field& field)
{
    const auto& box = field.box;
    auto transition = glm::transpose(glm::dmat3{
                    1., 0., -box.min_corner().x,
                    0., 1., -box.min_corner().y,
                    0., 0., 1.});

    auto box_diff = box.max_corner() - box.min_corner();
    double scale_x = mat.cols / box_diff.x;
    double scale_y = mat.rows / box_diff.y;

    glm::dmat3 scale = {scale_x, 0., 0.,
                        0., scale_y, 0.,
                        0., 0., 1.};

    auto transform = scale * transition;

    // on-screen transform, to flip vertically
    glm::dmat3 reflect = {1., 0., 0.,
                        0, -1, 0.,
                        0., 0., 1};

    transition = glm::transpose(glm::dmat3{
                1., 0., 0.,
                0., 1., -mat.rows,
                0., 0., 1.});

    transform = reflect * transition * transform;

    return transform;
}

std::vector<glm::dvec2> getVehicleContour(const Vehicle& v)
{
    std::vector<glm::dvec2> vehicle_points = {
        {0., 0.},
        {0., v.dimensions.width},
        {v.dimensions.length - v.dimensions.width / 4., v.dimensions.width},
        {v.dimensions.length + v.dimensions.width / 4., v.dimensions.width / 2.},
        {v.dimensions.length - v.dimensions.width / 4., 0.},
        {0., 0.}};

    for (auto& p : vehicle_points)
    {
        p -= v.origin;
    }

    return vehicle_points;
}

void draw(cv::Mat& mat, const Field& field, const Vehicle& v)
{
    auto transform = createFieldTransform(mat, field);

    // draw vehicle
    double orientation_angle = v.state.orientation; // + M_PI / 2.;
    auto vehicle_rotation = glm::transpose(glm::dmat3{
        cos(orientation_angle),    -sin(orientation_angle), 0.,
        sin(orientation_angle),    cos(orientation_angle),  0.,
        0,                          0,                      1.});

    glm::dvec2 offset = v.state.position; // - v.origin;
    auto vehicle_transition = glm::transpose(glm::dmat3{
            1., 0., offset.x,
            0., 1., offset.y,
            0., 0., 1.});

    glm::dmat3 vehicle_transform = transform * vehicle_transition * vehicle_rotation;

    auto vehicle_points = getVehicleContour(v);
    auto cv_range = boost::adaptors::transform(
            vehicle_points,
            [&](const auto& p){ return transformPoint(p, vehicle_transform);});

    std::vector<cv::Point> cv_vehicle_points(cv_range.begin(), cv_range.end());

    cv::fillConvexPoly(mat, cv_vehicle_points, {0xff, 0xff, 0xff});

    // draw vehicle origin
    cv::circle(mat, transformPoint({0., 0.}, vehicle_transform), 2, {0, 0, 0xff}, CV_FILLED);

    // draw front wheels
    glm::dvec2 left_wheel_center(v.dimensions.length * .8, v.dimensions.width * .9);
    glm::dvec2 right_wheel_center(v.dimensions.length * .8, v.dimensions.width * .1);
    double wheel_radius = v.dimensions.length * .1;

    for (auto wheel_center : {left_wheel_center, right_wheel_center})
    {
        wheel_center -= v.origin;
        auto offset = glm::dvec2{cos(v.state.steering), sin(v.state.steering)} * wheel_radius;
        auto wheel_front = transformPoint(wheel_center + offset, vehicle_transform);
        auto wheel_back = transformPoint(wheel_center - offset, vehicle_transform);

        cv::line(mat, wheel_back, wheel_front, {0, 0, 0xff}, 2);
    }
}

void moveVehicle(Vehicle& v, double meters, double delta_steering = 0.)
{
    double lr = v.origin.y;
    double lf = v.dimensions.length - v.origin.y;

//    double beta = std::atan2(lr * tan(v.state.steering), lr + lf);

    double dx = meters * cos(v.state.orientation); // + beta);
    double dy = meters * sin(v.state.orientation); // + beta);

    //double delta_orientation =
        //meters * std::sin(beta) / ((lr + lf) * std::tan(v.state.steering));

    double delta_orientation = std::atan2(meters * std::tan(v.state.steering), v.dimensions.length);

    v.state.position += glm::dvec2{dx, dy};
    v.state.orientation += delta_orientation;
    v.state.steering += delta_steering;

    boost::algorithm::clamp(v.state.steering, -v.constraints.max_steering, v.constraints.max_steering);

    //double delta_speed = acceleration;

    // update position
    //glm::dvec2 delta_pos = {-meters * sin(v.state.orientation), meters * cos(v.state.orientation)};
    //v.state.position += delta_pos;


    // update orientation
    // d/dt[x]
    //      [y]
    //      [orientation] =
    //      [speed]
    //      [steering]

}

int main()
{
    std::int32_t height = 800;
    std::int32_t width = 800;
    std::string name = "grid";

    cv::Mat frame(cv::Mat::zeros(height, width, CV_8UC3));

    Field field = createField();
    auto v = initVehicle();

    draw(frame, field, v);
    cv::imshow(name, frame);
    cv::waitKey(1);

    //double delta_steering = 0.;
    //for (auto i : boost::irange(0, 100))
    while (bg::within(v.state.position, field.box))
    {
        //moveVehicle(v, 0.5, delta_steering);
        cv::Mat frame(cv::Mat::zeros(height, width, CV_8UC3));
        draw(frame, field, v);
        cv::imshow(name, frame);
        std::this_thread::sleep_for(std::chrono::milliseconds(30));

        double delta_steering = 0.;
        int key = cv::waitKey(1);
        if (key == 81)
        {
            delta_steering = 0.01;
        }
        else if (key == 83)
        {
            delta_steering = -0.01;
        }
        moveVehicle(v, 0.5, delta_steering);

        //std::cout << static_cast<int>(key) << std::endl;
    }

    cv::waitKey(0);

    return 0;
}
