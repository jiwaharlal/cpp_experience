#pragma once

#include <list>

#include "Field.hpp"

class CAgent
{
public: // methods
    explicit CAgent(
            const IntMat& field,
            const cv::Point& position,
            const cv::Point& goal,
            std::int32_t visibility_radius);

    cv::Point getPosition() const;
    cv::Point getGoal() const;

    double getVisibilityRadius() const;

    const std::list<cv::Point>& getPlannedPath() const;

    const IntMat& getMap() const;

    void move();

private: // fields
    IntMat m_map;
    cv::Point m_position;
    cv::Point m_goal;
    std::int32_t m_visibility_radius;
    std::list<cv::Point> m_path;
};
