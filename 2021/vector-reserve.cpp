#include <vector>
#include <iostream>

int main()
{
    std::vector<int> v;
    v.reserve(11);
    for (int i = 0; i < 14; ++i)
    {
        v.push_back(i);
        std::cout << "i=" << i << " capacity=" << v.capacity() << std::endl;
    }

    std::cout << "v capacity: " << v.capacity() << std::endl;

    return 0;
}
