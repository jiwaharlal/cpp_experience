#pragma once

#include <math.h>

struct Dimensions
{
    double length;
    double width;
    double height;
};

struct VehicleState
{
    glm::dvec2 position;
    double orientation; // Angle between longitudinal axis of vehicle and x axis, radians CCW
    double steering; // Steering angle, longitudinal axis is zero, CCW, radians
    double speed; // m/s, positive is forward, negative is backward
};

struct Constraints
{
    double max_steering = M_PI / 6;
    double max_delta_steering = M_PI / 4;
    double max_acceleration = 2.;
    double max_braking = 20.;
};

struct Vehicle
{
    Dimensions dimensions;
    glm::dvec2 origin;
    VehicleState state;
    Constraints constraints;
};
