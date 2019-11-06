#include <iostream>
#include <string>

int main()
{
    std::string s1{"hello"};
    std::cout << s1.size() << std::endl;
    std::cout << s1.capacity() << std::endl;

    std::string s2{s1.crbegin(), s1.crend()};
    std::cout << s2 << std::endl;

    return 0;
}
