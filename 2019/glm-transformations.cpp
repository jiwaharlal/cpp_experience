#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

std::ostream& operator <<(std::ostream& out, const glm::dvec3& p)
{
    return out << '(' << p.x << "; " << p.y << "; " << p.z << ')';
}

int main()
{
    glm::dvec3 p{20, 10, 1};

    glm::dmat3 reflect_y{1., 0., 0.,
                        0., -1, 0.,
                        0., 0., 1.};

    auto transit_y = glm::transpose(glm::dmat3{
                1., 0., 0.,
                0., 1., -100.,
                0., 0., 1.});
    //auto transit_y = glm::translate(glm::dmat3(), glm::dvec2(0., -200.));

    auto p2 = reflect_y * transit_y * p;
    std::cout << p2 << std::endl;

    return 0;
}
