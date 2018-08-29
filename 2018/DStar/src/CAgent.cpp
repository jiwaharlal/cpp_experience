#include "CAgent.hpp"

CAgent::CAgent(
        const Field& field,
        const Point& position,
        double visibility_radius,
        const Point& goal)
    : m_position(position)
    , m_goal(goal)
    , m_visibility_radius(visibility_radius)
{
}

Point CAgent::getPosition() const
{
    return m_position;
}

double CAgent::getVisibilityRadius() const
{
    return m_visibility_radius;
}

const std::vector<Point>& CAgent::getPlannedPath() const
{
    return m_path;
}

const Field& CAgent::getMap() const
{
    return m_map;
}

void CAgent::move()
{
    if (!m_path.emtpty())
    {
        m_position = m_path.front();
        m_path.pop_front();
    }
}
