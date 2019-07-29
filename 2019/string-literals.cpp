#include <string>
#include <iostream>

int main()
{
    const std::string s1 = R"(hello\world)";

    std::cout << s1 << std::endl;

    return 0;
}
