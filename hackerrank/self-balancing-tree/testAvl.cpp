#include <cstdlib>
#include <iostream>
#include <iterator>

#include <TAvlSet.hpp>

void testLowerBound()
{
}

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

    TAvlSet<int> s3;
    srand(0);
    for (int i = 0; i < 200; ++i)
    {
        s3.insert(rand() % 50);
    }
    std::copy(s3.begin(), s3.end(), std::ostream_iterator<int>(std::cout, ", "));
    std::cout << std::endl;

    using reverse_avset_iterator = std::reverse_iterator<TAvlSet<int>::iterator>;
    reverse_avset_iterator rbegin(s3.end());
    reverse_avset_iterator rend(s3.begin());

    std::copy(rbegin, rend, std::ostream_iterator<int>(std::cout, ", "));
    std::cout << std::endl;

    return 0;
}
