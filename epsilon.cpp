#include <iostream>
#include <limits>

int main()
{
    std::cout << 100.f * std::numeric_limits<double>::epsilon() << '\n';
    return 0;
}
