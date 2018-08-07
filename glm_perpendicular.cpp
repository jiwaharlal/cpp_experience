#include <iostream>

#include <glm/vec3.hpp>
#include <glm/gtx/perpendicular.hpp>

std::ostream& operator <<(std::ostream& out, const glm::dvec3& v)
{
    return out << "(" << v.x << ", " << v.y << ", " << v.z << ")";
}

int main()
{
    const glm::dvec3 start = {0., 0., 0.};
    const glm::dvec3 end = {0., 1., 0.};

    glm::dvec3 vec = end - start;

    std::cout << "vec: " << vec << std::endl;

    const glm::dvec3 normal = {0., 0., 1.};
    //const glm::dvec3 perp = glm::perp(vec, normal);
    auto perp = glm::cross(vec, normal);

    std::cout << "perp: " << perp << std::endl;

    vec = glm::dvec3{1., 0., 0.};

    const glm::dvec3 up{0., 1., 0.};
    const glm::dvec3 down{0., -1., 0.};

    std::cout << "up : " << glm::cross(vec, up) << '\n';
    std::cout << "down : " << glm::cross(vec, down) << '\n';

    return 0;
}
