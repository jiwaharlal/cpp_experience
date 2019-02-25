#include <iostream>

int main()
{
    char* pointers[10];
    for (const auto p : pointers)
    {
        std::cout << reinterpret_cast<std::size_t>(p) << std::endl;
    }

    return 0;
}
