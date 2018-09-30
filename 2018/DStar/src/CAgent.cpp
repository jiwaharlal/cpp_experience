#include "CAgent.hpp"

CAgent::CAgent(
        const IntMat& field,
        const cv::Point& position,
        const cv::Point& goal,
        std::int32_t visibility_radius)
    : m_map(field)
    , m_position(position)
    , m_goal(goal)
    , m_visibility_radius(visibility_radius)
{
}

cv::Point CAgent::getPosition() const
{
    return m_position;
}

cv::Point CAgent::getGoal() const
{
    return m_goal;
}

double CAgent::getVisibilityRadius() const
{
    return m_visibility_radius;
}

const std::list<cv::Point>& CAgent::getPlannedPath() const
{
    return m_path;
}

const IntMat& CAgent::getMap() const
{
    return m_map;
}

void CAgent::move()
{
    if (!m_path.empty())
    {
        m_position = m_path.front();
        m_path.pop_front();
    }
}
