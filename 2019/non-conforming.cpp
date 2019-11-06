#include <cmath>
#include <iostream>

struct Point
{
    double x;
    double y;
};

int main()
{
    Point p = {.x = .2, .y = 7.8};

    std::cout << p.y << std::endl;

    int i = std::round(p.y);

    switch (i)
    {
        case 1 ... 3:
            std::cout << "less than 4";
            break;
        case 4 ... 8:
            std::cout << "equal or more than 4";
            break;
        default:
            break;
    }

    std::cout << std::endl;

    return 0;
}
