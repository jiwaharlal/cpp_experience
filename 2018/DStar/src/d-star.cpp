#include <iostream>

#include "Field.hpp"

int main()
{
    const int c_field_width = 120;
    const int c_field_height = 80;

    auto field = createRandomField(c_field_width, c_field_height);
    Point agent_pos{0, 0};
    const double c_visibility_radius = 10.;
    Point goal{c_field_weight - 1, c_field_height - 1};
    CAgent agent(field, agent_pos, c_visibility_radius, goal);

    while (agent.getPosition() != goal)
    {
        agent.plan();

        draw(field, agent, goal);

        agent.move();
    }

    cv::waitKey(0);

    return 0;
}
