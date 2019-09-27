#include <iostream>
#include <array>

int main()
{
    using ar_3_2 = std::array<std::array<double, 3>, 2>;

    ar_3_2 a{{0., 1., 2., 3., 4., 5.}};

    std::cout << a[1][1] << std::endl;

    return 0;
}
