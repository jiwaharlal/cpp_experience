#include <iostream>

#include <TAvlSet.hpp>

int main()
{
    TAvlSet<int> s;
    s.insert(3);
    s.insert(5);
    s.insert(4);

    for (auto i : s)
    {
        std::cout << i << " ";
    }
    std::cout << std::endl;

    TAvlSet<int> s2;
    s2.insert(2);
    s2.insert(4);
    s2.insert(3);

    for (auto i : s2)
    {
        std::cout << i << " ";
    }
    std::cout << std::endl;

    return 0;
}
