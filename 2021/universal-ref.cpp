#include <iostream>

//struct {int result, bool is_success} generate()
//{
//}

void foo(const int&)
{
    std::cout << "foo(int)\n";
}

void foo(int&&)
{
    std::cout << "foo(int&&)\n";
}

int main()
{
    int a = 2, b = 3;
    auto&& c = a + b;

    std::cout << c << std::endl;

    int u = 23;
    auto&& ru = u;
    std::cout << ru << std::endl;

    foo(ru);
    foo(std::move(ru));

    return 0;
}
