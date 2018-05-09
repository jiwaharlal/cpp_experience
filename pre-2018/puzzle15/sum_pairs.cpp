#include <utility>
#include <iostream>

int main()
{
    auto p1 = std::make_pair(1, 2);
    auto p2 = std::make_pair(3, 4);

    auto p3 = p1 + p2;

    return 0;
}
