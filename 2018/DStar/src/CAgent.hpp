#pragma once

#include "Field.hpp"

class CAgent
{
public: // methods
    explicit CAgent(
            const Field& field,
            const Point& position,
            double visibility_radius,
            const Point& goal);

    Point getPosition() const;

    double getVisibilityRadius() const;

    const std::vector<Point>& getPlannedPath() const;

    const Field& getMap() const;

    void move();

private: // fields
    Field m_field;
    Point m_goal;
    Point m_position;
};
